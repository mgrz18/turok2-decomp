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


def draft(name, asm):
    src = WORK / f"{name}.s"
    src.write_text(asm)
    res = subprocess.run(
        [str(ROOT / ".venv" / "bin" / "m2c"), "-t", "mips-gcc-c", "--valid-syntax", str(src)],
        capture_output=True, text=True)
    out = res.stdout
    # "missing jr $ra" means the range is not a whole function: a fragment left
    # by a boundary splat or the seeding got wrong. Matching it would put the
    # wrong unit in C, so leave it for the segmentation work.
    if (res.returncode or "Decompilation failure" in out or "M2C_ERROR" in out
            or 'missing "jr $ra"' in out):
        return None
    # `&D_800A5858 + 4` is m2c for "four bytes past the symbol", but in C it
    # scales by the pointee (M2C_UNK is s32) and lands 16 bytes out.
    out = re.sub(r"&(\w+) \+ (0x[0-9A-Fa-f]+|\d+)\b", r"(void *)((s8 *)&\1 + \2)", out)
    return out


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
    args = ap.parse_args()

    WORK.mkdir(parents=True, exist_ok=True)
    for old in WORK.glob("*"):
        old.unlink()

    done = functions_in_c()
    bodies = extract_asm()
    cands = [f for f in engine_functions()
             if args.min_size <= f[1] <= args.max_size and f[2] not in done
             and f[2] in bodies and "jtbl_" not in bodies[f[2]]]
    cands.sort(key=lambda f: (f[1], f[0]))
    cands = cands[:args.limit]
    print(f"candidates: {len(cands)}")

    drafted = {}
    for _v, _s, name in cands:
        c = draft(name, bodies[name])
        if c is not None:
            drafted[name] = HEADER + c
            (WORK / f"{name}.c").write_text(drafted[name])
    print(f"m2c drafts: {len(drafted)}")

    script = "for f in build/auto/*.c; do n=${f%.c}; tools/cc_func.sh $f $n.o cc1 -O2 >/dev/null 2>$n.err || true; done"
    subprocess.run(["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
                    "-w", "/work", "-e", "ASSEMBLER=gas", IMAGE, "bash", "-c", script],
                   capture_output=True, text=True)

    syms = match_func.rom_symbols()
    rom = match_func.ROM.read_bytes()
    results = {"match": [], "differ": [], "no_compile": [], "own_data": []}
    for name in drafted:
        obj = WORK / f"{name}.o"
        if not obj.exists():
            results["no_compile"].append(name)
            continue
        funcs = match_func.object_functions(obj)
        if name not in funcs:
            results["no_compile"].append(name)
            continue
        words, relocs = funcs[name]
        off, size = syms[name]
        want = rom[off:off + size]
        import struct
        want = struct.unpack(f">{size // 4}I", want)
        ok = len(words) == len(want) and all(
            match_func.mask(g, relocs.get(i)) == match_func.mask(w, relocs.get(i))
            and match_func.reloc_target_ok(g, w, relocs.get(i))
            for i, (g, w) in enumerate(zip(words, want)))
        if not ok:
            results["differ"].append(name)
        elif own_data(obj):
            results["own_data"].append(name)
        else:
            results["match"].append(name)

    for k, v in results.items():
        print(f"{k:11s} {len(v)}")
    (WORK / "results.json").write_text(json.dumps(results, indent=1))

    if args.accept and results["match"]:
        SRC.mkdir(parents=True, exist_ok=True)
        wired = 0
        for name in sorted(results["match"]):
            res = subprocess.run([sys.executable, str(ROOT / "tools" / "c_subsegment.py"),
                                  f"code/{name}", name, "--write"],
                                 capture_output=True, text=True)
            if res.returncode:
                print(f"  skip {name}: {res.stderr.strip() or res.stdout.strip()}")
                continue
            (SRC / f"{name}.c").write_text(drafted[name])
            wired += 1
        print(f"wired into the build: {wired}  (now run make setup && make verify)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
