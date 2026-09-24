#!/usr/bin/env python3
"""Try m2c's draft of many functions at once and keep the ones that match.

A good share of small functions come out of m2c already matching: a wrapper,
a getter, a field copy. Finding them one at a time is slow, so this does the
whole loop in bulk:

  1. pick candidates from the engine's `.code`, smallest first, skipping any
     function already in C and any that reads a jump table (a `switch` makes
     the C emit its own .rodata, which still lives in asm);
  2. draft each with `m2c -t mips-gcc-c --valid-syntax`, unknown struct fields
     as M2C_FIELD (include/m2c_macros.h);
  3. compile every draft in one container run (tools/cc_func.sh, GNU as);
  4. compare each with the ROM, relocations masked, the way match_func.py does,
     and reject any whose object has data of its own (.rodata/.data/.bss),
     since that would add bytes the asm build does not have.

With --variants, each function is drafted several ways (m2c's --no-andor,
--void, --no-casts, register variables...) and the first variant that matches
wins; a draft that differs under one option set often matches under another.
Every miss is scored by how many words differ, in results.json, so hand work
can start from the closest.

With --accept, each match gets a `c` subsegment (tools/c_subsegment.py) and its
C file under src/us/code/. Run `make setup` and `make verify` afterwards; the
SHA1 decides, as always.

Usage:
    python3 tools/auto_match.py --max-size 64 --limit 300
    python3 tools/auto_match.py --max-size 64 --limit 300 --accept
"""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
import match_func  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
WORK = ROOT / "build" / "auto"
SRC = ROOT / "src" / "us" / "code"
ASM_GLOB = "us/asm/*.s"
IMAGE = match_func.IMAGE

ENGINE = (0x00200500, 0x002A5200)
REGS = match_func.REGS
HEADER = '#include "common.h"\n#include "m2c_macros.h"\n\n'
# Sections an object may carry without adding bytes the asm build lacks.
HARMLESS = {"", ".text", ".reginfo", ".MIPS.abiflags", ".pdr", ".gnu.attributes",
            ".mdebug", ".comment", ".symtab", ".strtab", ".shstrtab", ".rel.text",
            ".rela.text", ".note.GNU-stack"}


def functions_in_c():
    names = set()
    for path in SRC.parent.rglob("*.c"):
        text = re.sub(r"/\*.*?\*/", "", path.read_text(), flags=re.S)
        names.update(re.findall(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{", text, re.M))
    return names


def engine_functions():
    """[(vram, size, name)] for every sized function in .code."""
    out = []
    with match_func.ELF.open("rb") as fh:
        for sym in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if sym["st_info"]["type"] != "STT_FUNC" or not sym["st_size"]:
                continue
            v = sym["st_value"]
            if ENGINE[0] <= v < ENGINE[1] and sym.name.startswith("func_"):
                out.append((v, sym["st_size"], sym.name))
    return sorted(out)


def extract_asm():
    """name -> the function's asm with ABI register names, for m2c."""
    bodies = {}
    for path in sorted(ROOT.glob(ASM_GLOB)):
        text = path.read_text()
        for m in re.finditer(r"^\.globl (func_[0-9A-F]{8})\n.*?^\.end \1\n", text, re.S | re.M):
            body = m.group(0)
            body = body.replace(f".ent {m.group(1)}\n", "").replace(f".end {m.group(1)}\n", "")
            body = re.sub(r"\$(\d+)\b", lambda r: "$" + REGS[int(r.group(1))], body)
            bodies[m.group(1)] = ".set noat\n.set noreorder\n.section .text\n" + body
    return bodies


VARIANTS = [
    [],
    ["--no-andor"],
    ["--void"],
    ["--no-casts"],
    ["--no-unk-inference"],
    ["--reg-vars", "s0,s1,s2,s3,s4,s5,s6,s7"],
    ["--no-andor", "--no-casts"],
]


POW2 = {"2": 1, "4": 2, "8": 3, "16": 4, "32": 5, "64": 6, "128": 7, "256": 8}
SHIFT = re.compile(r"\b(\w+) \* (2|4|8|16|32|64|128|256)\b")
SWAP = re.compile(r"\((\w+) \+ \((\w+ \* \d+)\)\)")
CONTEXT = WORK / "m2c_context.h"
PROTOS = {}      # name -> prototype line, from include/functions.h
TYPEDEFS = ""


def write_context():
    """Load common.h's typedefs and include/functions.h's prototypes.

    With the callee prototypes known, m2c passes a method's `this` through
    instead of putting the one argument it sees set into $a0.
    """
    global TYPEDEFS
    TYPEDEFS = "\n".join(l for l in (ROOT / "include" / "common.h").read_text().splitlines()
                         if not l.startswith("#")) + "\n"
    for line in (ROOT / "include" / "functions.h").read_text().splitlines():
        m = re.match(r"^\w+ (func_[0-9A-F]{8})\(", line)
        if m:
            PROTOS[m.group(1)] = line
    CONTEXT.write_text(TYPEDEFS + "\n".join(PROTOS.values()) + "\n")


def draft(name, asm, opts=()):
    """m2c's draft of one function.

    The context holds every prototype. Given its own, m2c would type the
    function's parameters s32 even where the body dereferences them, so the
    function is drafted under a stand-in name and renamed back. One shared
    context file also lets m2c cache the parse, which matters at 4,000+
    prototypes and thousands of calls.
    """
    src = WORK / f"{name}.s"
    src.write_text(asm.replace(name, name + "_draft"))
    res = subprocess.run(
        [str(ROOT / ".venv" / "bin" / "m2c"), "-t", "mips-gcc-c", "--valid-syntax",
         "--context", str(CONTEXT), *opts, str(src)],
        capture_output=True, text=True)
    out = res.stdout.replace(name + "_draft", name)
    # "missing jr $ra" means the range is not a whole function: a fragment left
    # by a boundary splat or the seeding got wrong. Matching it would put the
    # wrong unit in C, so leave it for the segmentation work.
    if (res.returncode or "Decompilation failure" in out or "M2C_ERROR" in out
            or 'missing "jr $ra"' in out):
        return None
    # `&D_800A5858 + 4` is m2c for "four bytes past the symbol", but in C it
    # scales by the pointee (M2C_UNK is s32) and lands 16 bytes out.
    out = re.sub(r"&(\w+) \+ (0x[0-9A-Fa-f]+|\d+)\b", r"(void *)((s8 *)&\1 + \2)", out)
    out = fill_leading_params(name, out)
    out = declare_stack_vars(name, out)
    called = sorted(set(re.findall(r"\b(func_[0-9A-F]{8})\b", out)) - {name})
    decls = [PROTOS[c] for c in called if c in PROTOS and f" {c}(" not in out.split("{")[0]]
    if decls:
        out = "\n".join(decls) + "\n\n" + out
    return out


SIG = re.compile(r"^(?P<ret>[^\n(]*?\b)(?P<name>func_[0-9A-F]{8})\((?P<params>[^)]*)\) \{", re.M)


def fill_leading_params(name, src):
    """Declare the parameters m2c left out ahead of the ones it kept.

    m2c names a parameter by its register (`arg3` is $a3) but declares only
    the ones the body uses, so `s32 f(s32 arg3) { return arg3; }` compiles the
    value into $a0. A method that ignores `this` is the common case. Any gap
    below the highest `argN` becomes an unused s32 in its position.
    """
    m = next((m for m in SIG.finditer(src) if m.group("name") == name), None)
    if not m or m.group("params").strip() in ("", "void"):
        return src
    params = [p.strip() for p in m.group("params").split(",")]
    by_index = {}
    for p in params:
        pm = re.match(r"^(.*?)\barg(\d)$", p)
        if not pm:
            return src   # a parameter m2c named some other way; leave it
        by_index[int(pm.group(2))] = p
    top = max(by_index)
    if len(by_index) == top + 1:
        return src
    # A float parameter shifts the register assignment; do not guess there.
    if any("f32" in p or "f64" in p for p in params):
        return src
    full = [by_index.get(i, f"s32 arg{i}") for i in range(top + 1)]
    return src[:m.start("params")] + ", ".join(full) + src[m.end("params"):]


STACKVAR = re.compile(r"\b((?:unk)?sp[0-9A-F]+)\b")


def declare_stack_vars(name, src):
    """Declare the stack slots m2c uses but forgets to declare.

    m2c sometimes takes the address of a stack slot (`&sp1C`) without listing
    it among the locals, and the draft does not compile. An s32 in its place
    at least compiles; whether it matches is for the comparison to say.
    """
    m = next((m for m in SIG.finditer(src) if m.group("name") == name), None)
    if not m:
        return src
    body = src[m.end():]
    declared = set(re.findall(r"^\s+[\w\s\*]+?\b((?:unk)?sp[0-9A-F]+)(?:\[[^\]]*\])?;", body, re.M))
    missing = sorted(set(STACKVAR.findall(body)) - declared)
    if not missing:
        return src
    decl = "".join(f"    s32 {v};\n" for v in missing)
    return src[:m.end()] + "\n" + decl + body.lstrip("\n") if body.startswith("\n") else src[:m.end()] + "\n" + decl + body


def accept(chosen):
    """Give each matched function its C subsegment and its file."""
    SRC.mkdir(parents=True, exist_ok=True)
    wired = 0
    for name in sorted(chosen):
        res = subprocess.run([sys.executable, str(ROOT / "tools" / "c_subsegment.py"),
                              f"code/{name}", name, "--write"],
                             capture_output=True, text=True)
        if res.returncode:
            print(f"  skip {name}: {res.stderr.strip() or res.stdout.strip()}")
            continue
        (SRC / f"{name}.c").write_text(chosen[name])
        wired += 1
    print(f"wired into the build: {wired}  (now run make setup && make verify)")
    return 0


def own_data(obj):
    with open(obj, "rb") as fh:
        for sec in ELFFile(fh).iter_sections():
            if sec.name not in HARMLESS and sec["sh_flags"] & 2 and sec["sh_size"]:
                return sec.name
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--max-size", type=int, default=64, help="largest function, in bytes")
    ap.add_argument("--min-size", type=int, default=0)
    ap.add_argument("--limit", type=int, default=200, help="candidates to try")
    ap.add_argument("--accept", action="store_true", help="wire matches into the build")
    ap.add_argument("--variants", action="store_true", help="try several m2c option sets")
    ap.add_argument("--accept-existing", action="store_true",
                    help="wire in the matches of the last run (build/auto/results.json) without redoing it")
    ap.add_argument("--names", type=Path, help="only these functions (one per line, or a results.json)")
    args = ap.parse_args()

    if args.accept_existing:
        results = json.loads((WORK / "results.json").read_text())
        chosen = {n: (WORK / f"{n}__v{k}.c").read_text() for n, k in results["match"].items()}
        return accept(chosen)

    WORK.mkdir(parents=True, exist_ok=True)
    for old in WORK.glob("*"):
        old.unlink()

    subprocess.run([sys.executable, str(ROOT / "tools" / "gen_prototypes.py")], check=True,
                   capture_output=True)
    write_context()
    done = functions_in_c()
    bodies = extract_asm()
    cands = [f for f in engine_functions()
             if args.min_size <= f[1] <= args.max_size and f[2] not in done
             and f[2] in bodies and "jtbl_" not in bodies[f[2]]]
    if args.names:
        text = args.names.read_text()
        wanted = (set(json.loads(text).get("differ", {})) if args.names.suffix == ".json"
                  else {l.strip() for l in text.splitlines() if l.strip()})
        cands = [f for f in cands if f[2] in wanted]
    cands.sort(key=lambda f: (f[1], f[0]))
    cands = cands[:args.limit]
    print(f"candidates: {len(cands)}")

    variants = VARIANTS if args.variants else VARIANTS[:1]

    def draft_all(name):
        """Every variant of one function, deduplicated: {key: source}."""
        out, seen = {}, set()
        for k, opts in enumerate(variants):
            c = draft(name, bodies[name], opts)
            if c is None:
                continue
            forms = [c]
            if args.variants:
                # m2c writes every shift by a constant as a multiply, and
                # GCC 2.8 does not treat `i * 4` and `i << 2` the same: the
                # operand order of the addu that follows differs.
                sh = SHIFT.sub(lambda m: f"({m.group(1)} << {POW2[m.group(2)]})", c)
                # It also keeps the order of `a + b*4` into the addu, and m2c
                # always writes the base first.
                sw = SWAP.sub(r"((\2) + \1)", c)
                for form in (sh, sw, SWAP.sub(r"((\2) + \1)", sh)):
                    if form != c and form not in forms:
                        forms.append(form)
            for j, form in enumerate(forms):
                if form not in seen:
                    seen.add(form)
                    out[k * 10 + j] = HEADER + form
        return name, out

    # m2c is the slow step (~0.7 s a call); run it on every core.
    from concurrent.futures import ThreadPoolExecutor
    import os
    drafted = {}   # name -> {variant key: source}
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 8) as pool:
        for name, out in pool.map(draft_all, [c[2] for c in cands]):
            if out:
                drafted[name] = out
                for key, src in out.items():
                    (WORK / f"{name}__v{key}.c").write_text(src)
    print(f"m2c drafts: {sum(len(v) for v in drafted.values())} for {len(drafted)} functions")

    script = ("ls build/auto/*.c | xargs -P 8 -I{} bash -c "
              "'f={}; n=${f%.c}; tools/cc_func.sh $f $n.o cc1 -O2 >/dev/null 2>$n.err || true'")
    subprocess.run(["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
                    "-w", "/work", "-e", "ASSEMBLER=gas", IMAGE, "bash", "-c", script],
                   capture_output=True, text=True)

    import struct
    syms = match_func.rom_symbols()
    rom = match_func.ROM.read_bytes()
    results = {"match": {}, "differ": {}, "no_compile": [], "own_data": []}
    chosen = {}
    for name, drafts in drafted.items():
        off, size = syms[name]
        want = struct.unpack(f">{size // 4}I", rom[off:off + size])
        best = None
        compiled = False
        for k in sorted(drafts):
            obj = WORK / f"{name}__v{k}.o"
            if not obj.exists():
                continue
            funcs = match_func.object_functions(obj)
            if name not in funcs:
                continue
            compiled = True
            words, relocs = funcs[name]
            n = max(len(words), len(want))
            bad = sum(1 for i in range(n)
                      if i >= len(words) or i >= len(want)
                      or match_func.mask(words[i], relocs.get(i)) != match_func.mask(want[i], relocs.get(i))
                      or not match_func.reloc_target_ok(words[i], want[i], relocs.get(i)))
            if bad == 0:
                if own_data(obj):
                    best = ("own_data", k, 0)
                    continue
                best = ("match", k, 0)
                break
            if best is None or (best[0] == "differ" and bad < best[2]):
                best = ("differ", k, bad, n)
        if not compiled:
            results["no_compile"].append(name)
        elif best[0] == "match":
            results["match"][name] = best[1]
            chosen[name] = drafts[best[1]]
        elif best[0] == "own_data":
            results["own_data"].append(name)
        else:
            results["differ"][name] = [best[2], best[3], best[1]]

    for k, v in results.items():
        print(f"{k:11s} {len(v)}")
    if args.variants and results["match"]:
        by = {}
        for k in results["match"].values():
            by[k] = by.get(k, 0) + 1
        print("  matches by variant:", {(" ".join(VARIANTS[k // 10]) or "default") + (" +swap" if k % 10 else ""): c
                                        for k, c in sorted(by.items())})
    (WORK / "results.json").write_text(json.dumps(results, indent=1))

    if args.accept and results["match"]:
        accept(chosen)
    return 0


if __name__ == "__main__":
    sys.exit(main())
