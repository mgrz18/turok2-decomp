#!/usr/bin/env python3
"""Bring another decomp's C for a function into Turok 2, as a standalone file.

tools/xmatch_ref.py finds functions whose code in another decomp of the same
engine (Turok: Rage Wars, AbuN64/ragewars-decomp) compiles to the same words
as a pending Turok 2 function, relocations masked. This turns each into a
file of its own under build/manual/<tag>/:

  1. the reference's file, preprocessed, cut down to that one function: other
     function definitions become prototypes and variable definitions become
     extern declarations, so the file adds no code or data of its own;
  2. the function renamed to its Turok 2 name, and every symbol it calls or
     reads renamed to the Turok 2 symbol at the same instruction (from the
     relocation in the compiled object and the operand in Turok 2's asm);
  3. a header comment crediting the source.

Each file is then checked with tools/match_func.py; the ones that match are
listed in build/manual/<tag>/MATCHED for tools/accept_manual.py.

Usage:
    tools/import_ref.py references/rw_abu --tag abu \\
        --include include src --define VERSION_US_REV1 \\
        --credit "AbuN64/ragewars-decomp (Turok: Rage Wars)" [--names a b ...]
"""

import argparse
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import auto_match  # noqa: E402
import match_func  # noqa: E402

IDENT = re.compile(r"[A-Za-z_]\w*")


def top_level(text):
    """Split a preprocessed C file into top-level items (declarations ending
    in `;`, function definitions ending in their closing brace)."""
    items, start, depth, i, n = [], 0, 0, 0, len(text)
    while i < n:
        c = text[i]
        if c in "\"'":
            q, i = c, i + 1
            while i < n and text[i] != q:
                i += 2 if text[i] == "\\" else 1
        elif c in "{(":
            depth += 1
        elif c in "})":
            depth -= 1
            if depth == 0 and c == "}":
                j = i + 1
                while j < n and text[j] in " \t\r\n":
                    j += 1
                if j < n and text[j] == ";":        # struct {...}; or x = {...};
                    i = j
                    items.append(text[start:i + 1].strip())
                    start = i + 1
                elif is_function(text[start:i + 1]):
                    items.append(text[start:i + 1].strip())
                    start = i + 1
        elif c == ";" and depth == 0:
            items.append(text[start:i + 1].strip())
            start = i + 1
        i += 1
    return [x for x in items if x]


def is_function(item):
    head = item.split("{", 1)[0]
    return "(" in head and "=" not in head and not head.lstrip().startswith(("typedef", "struct", "union", "enum"))


def func_name(item):
    head = item.split("{", 1)[0]
    m = re.search(r"(\w+)\s*\([^()]*(\([^()]*\)[^()]*)*\)\s*$", head.strip())
    return m.group(1) if m else None


def reduce(text, keep):
    """Only `keep`'s definition stays; nothing else defines code or data."""
    out = []
    for item in top_level(text):
        if item.endswith("}") and is_function(item):
            name = func_name(item)
            if name == keep:
                # Turok 2's other code calls it: it cannot stay file-local.
                out.append(re.sub(r"^\s*static\s+", "", item))
            elif re.search(r"\b(__)?inline(__)?\b", item.split("{", 1)[0]):
                # An inline helper: Turok 2 has it inlined into the function,
                # so its body has to come along. Kept file-local, so that a
                # copy the compiler does not inline is not a second global.
                head, body = item.split("{", 1)
                head = re.sub(r"^\s*((static|extern)\s+|(__)?inline(__)?\s+)*", "", head)
                out.append("static __inline__ " + head.strip() + " {" + body)
            else:
                out.append(re.sub(r"^\s*(static\s+|inline\s+)*", "", item.split("{", 1)[0]).strip() + ";")
            continue
        head = item.lstrip()
        if "(" in head and func_name(head.rstrip(";").rstrip()) == keep:
            out.append(re.sub(r"^\s*static\s+", "", item))   # its prototype: the first declaration sets linkage
            continue
        if head.startswith(("typedef", "extern")) or re.match(r"^(struct|union|enum)\b[^=]*\}\s*;$", head, re.S) \
                or re.match(r"^(struct|union|enum)\s+\w+\s*;$", head) or "(" in head.split("=", 1)[0] and "=" not in head:
            out.append(item)                    # a type, a prototype or a declaration already
            continue
        # A variable definition: declare it instead.
        decl = item.split("=", 1)[0].rstrip().rstrip(";").rstrip()
        decl = re.sub(r"^\s*(static\s+|const\s+(?=static))*", "", decl)
        out.append(f"extern {decl};")
    return "\n".join(prune(out, keep)) + "\n"


KEYWORDS = set("""auto break case char const continue default do double else enum extern float for goto if
int long register return short signed sizeof static struct switch typedef union unsigned void volatile while
inline""".split())


def declared(item):
    """Names an item declares: typedef names, struct/union/enum tags, the
    function or variable, enum constants."""
    names = set(re.findall(r"\b(?:struct|union|enum)\s+(\w+)\s*\{", item))
    head = item.split("{", 1)[0] if not item.lstrip().startswith(("typedef", "struct", "union", "enum")) else item
    if item.lstrip().startswith("typedef"):
        m = re.search(r"(\w+)\s*(\[[^]]*\]\s*)*;\s*$", item) or re.search(r"\(\s*\*\s*(\w+)\s*\)", item)
        if m:
            names.add(m.group(1))
    elif re.match(r"^\s*(struct|union|enum)\s+(\w+)\s*;$", item):
        names.add(re.match(r"^\s*(struct|union|enum)\s+(\w+)", item).group(2))
    elif not re.match(r"^\s*(struct|union|enum)\b[^;]*\}\s*;$", item, re.S):
        f = func_name(head.rstrip(";").rstrip()) if "(" in head else None
        if f:
            names.add(f)
        else:
            m = re.search(r"(\w+)\s*(\[[^]]*\]\s*)*;?\s*$", head.split("=", 1)[0].rstrip().rstrip(";"))
            if m:
                names.add(m.group(1))
    if re.search(r"\benum\b", item):
        body = item.split("{", 1)[1] if "{" in item else ""
        names |= set(re.findall(r"(?:^|,)\s*(\w+)\s*(?:=|,|\})", body))
    return names


def prune(items, keep):
    """Keep only what `keep` needs, transitively, in the original order."""
    decl = [declared(it) for it in items]
    uses = [set(IDENT.findall(it)) - KEYWORDS for it in items]
    want = {keep}
    chosen = set()
    changed = True
    while changed:
        changed = False
        for k, it in enumerate(items):
            if k not in chosen and decl[k] & want:
                chosen.add(k)
                want |= uses[k]
                changed = True
    return [it for k, it in enumerate(items) if k in chosen]


def asm_symbols(body):
    """Turok 2 asm: instruction index -> the symbol its operand names."""
    syms, idx = {}, 0
    for line in body.splitlines():
        s = line.strip()
        if not s or s.startswith((".", "#")) or s.endswith(":"):
            continue
        m = re.search(r"%(?:hi|lo)\(([A-Za-z_]\w*)\s*(\+\s*0x[0-9A-Fa-f]+|\+\s*\d+)?\)|\b(?:jal|j)\s+([A-Za-z_]\w*)", s)
        if m:
            syms[idx] = (m.group(1) or m.group(3), (m.group(2) or "").replace(" ", ""))
        idx += 1
    return syms


def object_relocs(obj, fname):
    """Compiled reference: instruction index -> (symbol, relocation type,
    the instruction word, whose low half is the addend for a LO16)."""
    res = subprocess.run(["mips-linux-gnu-objdump", "-dr", str(obj)], capture_output=True, text=True).stdout
    out, inside, base, word = {}, False, 0, 0
    for line in res.splitlines():
        m = re.match(r"^([0-9a-f]+) <(\S+)>:", line)
        if m:
            inside, base = m.group(2) == fname, int(m.group(1), 16)
            continue
        if inside:
            w = re.match(r"^\s+[0-9a-f]+:\s+([0-9a-f]{8})\s", line)
            if w:
                word = int(w.group(1), 16)
            r = re.match(r"^\s+([0-9a-f]+): (R_MIPS_\w+)\s+(\S+)", line)
            if r:
                sym = re.sub(r"\+0x[0-9a-f]+$", "", r.group(3))
                out[(int(r.group(1), 16) - base) // 4] = (sym, r.group(2), word)
    return out


ELF_SYMS = {}


def sym_addr(name):
    m = re.match(r"^[DB]_([0-9A-F]{8})$", name)
    return int(m.group(1), 16) if m else ELF_SYMS.get(name)


def build(target, m, args, bodies):
    src = args.repo / m["file"]
    i_file = ROOT / "build" / f"import_{args.tag}" / f"{target}.i"
    i_file.parent.mkdir(parents=True, exist_ok=True)
    incs = [f"-I{args.repo / d}" for d in args.include] + [f"-I{src.parent}"]
    cpp = (["cpp", "-x", "c", "-P", "-undef", "-lang-c", "-D_LANGUAGE_C", "-DF3DEX_GBI_2", "-D__GNUC__=2", "-nostdinc"]
           + [f"-D{d}" for d in args.define] + incs + [str(src), "-o", str(i_file)])
    if subprocess.run(cpp, capture_output=True).returncode:
        return target, "cpp failed"
    text = reduce(i_file.read_text(errors="replace"), m["ref"])
    obj = ROOT / "build" / f"xmatch_{args.tag}"
    import hashlib
    obj = obj / (hashlib.sha1(src.as_posix().encode()).hexdigest()[:16] + ".o")
    if not obj.exists():
        obj = obj.parent / (hashlib.sha1(str(src.relative_to(ROOT)).encode()).hexdigest()[:16] + ".o")
    relocs = object_relocs(obj, m["ref"]) if obj.exists() else {}
    t2 = asm_symbols(bodies.get(target, ""))
    rename = {m["ref"]: target}
    bases, new_syms = {}, {}
    for i, (sym, rtype, word) in relocs.items():
        if sym.startswith(".") or i not in t2:
            continue
        tsym, off = t2[i]
        if rtype == "R_MIPS_26" or not off and rtype != "R_MIPS_LO16":
            if not off and rename.get(sym, tsym) != tsym and sym not in bases:
                return target, f"{sym} maps to both {rename[sym]} and {tsym}"
            rename.setdefault(sym, tsym)
            continue
        # Data through %hi/%lo: the reference reads sym+addend where Turok 2's
        # asm names tsym+off, so sym itself sits at tsym+off-addend. Splat may
        # have cut that array into several symbols, or started one 4 bytes in.
        if rtype != "R_MIPS_LO16":
            continue
        addr = sym_addr(tsym)
        if addr is None:
            return target, f"{tsym} has no address"
        addend = (word & 0xFFFF) - 0x10000 if word & 0x8000 else word & 0xFFFF
        base = addr + int(off or "0", 0) - addend
        if bases.setdefault(sym, base) != base:
            return target, f"{sym} reads at two bases"
    for sym, base in bases.items():
        name = next((n for n, a in ELF_SYMS.items() if a == base and re.match(r"^[DB]_", n)), None)
        name = name or f"D_{base:08X}"
        if sym_addr(name) is None or name not in ELF_SYMS:
            new_syms[name] = base
        rename[sym] = name
    text = IDENT.sub(lambda x: rename.get(x.group(0), x.group(0)), text)
    out = ROOT / "build" / "manual" / args.out_tag / f"{target}.c"
    out.parent.mkdir(parents=True, exist_ok=True)
    if new_syms:
        (out.parent / f"{target}.syms").write_text("".join(f"{n} = 0x{a:08X};\n" for n, a in new_syms.items()))
    flags = f"/* cc1-flags: {args.cc_flags} */\n" if args.cc_flags else ""
    out.write_text(f"/* From {args.credit}: {m['file']}, {m['ref']}.\n"
                   f" * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */\n"
                   + flags + "\n" + text)
    res = subprocess.run([sys.executable, str(ROOT / "tools" / "match_func.py"), str(out)],
                         capture_output=True, text=True, env={**os.environ, "MATCH_ELF": "build/ref.elf"})
    line = next((l for l in res.stdout.splitlines() if re.match(rf"^\s+{target}\s", l)), res.stdout[-200:])
    return target, line.split(None, 1)[1].strip() if line.strip().startswith(target) else line.strip()


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("repo", type=Path)
    ap.add_argument("--tag", required=True)
    ap.add_argument("--include", nargs="*", default=["include"])
    ap.add_argument("--define", nargs="*", default=[])
    ap.add_argument("--credit", required=True)
    ap.add_argument("--cc-flags", help="cc1 flags the file is built with, written as `/* cc1-flags: */`")
    ap.add_argument("--names", nargs="*")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 8)
    ap.add_argument("--near", type=float, help="take the near ones (>= this ratio, < 1) instead, as permuter "
                    "drafts, into build/manual/<tag>_near/")
    args = ap.parse_args()
    if args.near:
        args.out_tag = f"{args.tag}_near"
    else:
        args.out_tag = args.tag
    found = json.loads((ROOT / "build" / f"xmatch_{args.tag}" / "matches.json").read_text())
    lo, hi = (args.near, 1.0) if args.near else (1.0, 1.01)
    todo = {n: m for n, m in found.items() if lo <= m["ratio"] < hi and (not args.names or n in args.names)}
    bodies = auto_match.extract_asm()
    from elftools.elf.elffile import ELFFile
    with open(ROOT / "build" / "ref.elf", "rb") as fh:
        for sym in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if sym.name and not sym.name.startswith("."):
                ELF_SYMS.setdefault(sym.name, sym["st_value"])
    with ThreadPoolExecutor(args.jobs) as pool:
        results = list(pool.map(lambda kv: build(kv[0], kv[1], args, bodies), sorted(todo.items())))
    ok = sorted(t for t, r in results if r == "MATCH")
    (ROOT / "build" / "manual" / args.out_tag).mkdir(parents=True, exist_ok=True)
    (ROOT / "build" / "manual" / args.out_tag / "MATCHED").write_text("".join(f"{t}\n" for t in ok))
    from collections import Counter
    why = Counter(re.sub(r"\d+ of \d+ words differ.*", "words differ", r)[:60] for t, r in results if r != "MATCH")
    print(f"{len(todo)} {'near' if args.near else 'identical'} functions: {len(ok)} match standalone")
    for k, v in why.most_common(8):
        print(f"  {v:4d}  {k}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
