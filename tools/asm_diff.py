#!/usr/bin/env python3
"""Compare the compiled code of two source trees, with every symbol resolved.

tools/match_func.py masks the relocated fields of an unlinked object: a
`sym + 4` that should have been `sym + 8` (an array of the wrong element type,
a struct view one field off) masks equal there and only fails `make verify`.
This closes that gap for edits that should not change the code at all (names,
declarations, header moves): it compiles every C file of a base revision and of
the working tree to assembly (the Makefile's cpp and cc1, with the file's own
`/* cc1-flags: */`), replaces every `symbol +/- offset` operand by the absolute
address it links to (the symbol table of the linked ELF), and reports the files
whose normalized assembly differs. A whole-tree run takes about ten seconds.

Renaming a global, moving its declaration to a header, giving it its real type
or reaching it as `gScene.m_Field` instead of `gScene_m_Field` all compile to
the same addresses, so they compare equal; anything that moves an access, a
load order or a register shows as a diff. Symbols the ELF does not know
(a new alias, defined in versions/symbols_manual.us.txt) are looked up there
too; anything else stays a name, so rebuild the ELF once after adding names.

Usage:
    python3 tools/asm_diff.py                     working tree vs HEAD, all of src/
    python3 tools/asm_diff.py --base ecba2b71     against another revision
    python3 tools/asm_diff.py src/us/code/func_0027ECF0.c ...   only these files
    python3 tools/asm_diff.py -v                  show the differing lines

Exit status: 0 when every file compiles and matches, 1 otherwise.
"""

import argparse
import difflib
import os
import re
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
CC1 = ROOT / "tools" / "sn64" / "cc1"
CPP_FLAGS = ["-P", "-undef", "-Wall", "-lang-c", "-D_LANGUAGE_C", "-DF3DEX_GBI_2",
             "-D__GNUC__=2", '-DGAME_VERSION="us"', "-nostdinc"]
CC_FLAGS = "-quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64"
FLAGS_LINE = re.compile(r"^/\* cc1-flags: (.*) \*/$", re.M)
OPERAND = re.compile(r"\b([A-Za-z_][A-Za-z0-9_.]*)((?:\s*[+-]\s*(?:0x[0-9a-fA-F]+|\d+))*)")
MANUAL = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;", re.M)


def symbols(elf_path):
    out = {}
    with open(elf_path, "rb") as fh:
        for sym in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if sym.name and sym.name not in out:
                out[sym.name] = sym["st_value"]
    for m in MANUAL.finditer((ROOT / "versions" / "symbols_manual.us.txt").read_text()):
        out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def compile_one(tree, rel, out_dir):
    """tree/rel -> out_dir/rel.s; returns None or the error text."""
    src = tree / rel
    out = out_dir / (rel + ".s")
    out.parent.mkdir(parents=True, exist_ok=True)
    inc = ["-I" + str(tree), "-I" + str(tree / "include"), "-I" + str(ROOT / "us" / "include")]
    with tempfile.TemporaryDirectory() as tmp:
        i = Path(tmp) / "f.i"
        r = subprocess.run(["cpp", *CPP_FLAGS, *inc, "-o", str(i), str(src)],
                           capture_output=True, text=True)
        if r.returncode:
            return "cpp: " + r.stderr.strip()
        m = FLAGS_LINE.search(src.read_text(errors="replace"))
        flags = (m.group(1) if m else CC_FLAGS).split()
        r = subprocess.run([str(CC1), *flags, "-o", str(out), str(i)], capture_output=True, text=True)
        if r.returncode:
            out.unlink(missing_ok=True)
            return "cc1: " + "\n".join(l for l in r.stderr.splitlines() if "warning" not in l)[:2000]
    return None


def normalize(path, syms):
    lines = []
    for line in path.read_text(errors="replace").splitlines():
        s = line.strip()
        if not s or s.startswith((".file", ".ident", "#")):
            continue

        def resolve(m):
            name = m.group(1)
            if name not in syms:
                return m.group(0)
            off = sum(int(t.replace(" ", ""), 0) for t in re.findall(r"[+-]\s*(?:0x[0-9a-fA-F]+|\d+)", m.group(2)))
            return "@%08X" % ((syms[name] + off) & 0xFFFFFFFF)
        lines.append(OPERAND.sub(resolve, s))
    return lines


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("files", nargs="*", help="C files (default: every .c under src/)")
    ap.add_argument("--base", default="HEAD", help="revision to compare against (default HEAD)")
    ap.add_argument("--elf", default=str(ROOT / "build" / "turok2.us.elf"),
                    help="linked ELF whose symbol table resolves the operands")
    ap.add_argument("-j", type=int, default=os.cpu_count(), help="parallel compiles")
    ap.add_argument("-v", "--verbose", action="store_true", help="print the differing lines")
    args = ap.parse_args()

    files = [str(Path(f).resolve().relative_to(ROOT)) for f in args.files] or \
        sorted(str(p.relative_to(ROOT)) for p in (ROOT / "src").rglob("*.c"))
    syms = symbols(args.elf)
    work = Path(tempfile.mkdtemp(prefix="asm_diff."))
    try:
        base = work / "base"
        base.mkdir()
        archive = subprocess.run(["git", "-C", str(ROOT), "archive", args.base, "src", "include"],
                                 capture_output=True, check=True).stdout
        subprocess.run(["tar", "-x", "-C", str(base)], input=archive, check=True)
        jobs = [(base, f, work / "a") for f in files if (base / f).exists()] + \
               [(ROOT, f, work / "b") for f in files]
        with ThreadPoolExecutor(args.j) as pool:
            errs = list(pool.map(lambda j: compile_one(*j), jobs))
        failed = {(str(j[0]), j[1]): e for j, e in zip(jobs, errs) if e}
        bad = 0
        for f in files:
            if (str(ROOT), f) in failed:
                print(f"FAIL {f}\n    " + failed[(str(ROOT), f)].replace("\n", "\n    "))
                bad += 1
                continue
            a, b = work / "a" / (f + ".s"), work / "b" / (f + ".s")
            if not a.exists():
                print(f"NEW  {f}" + (" (does not compile at the base)" if (base / f).exists() else ""))
                continue
            na, nb = normalize(a, syms), normalize(b, syms)
            if na != nb:
                bad += 1
                diff = [l for l in difflib.unified_diff(na, nb, lineterm="", n=0)
                        if not l.startswith(("---", "+++"))]
                print(f"DIFF {f} ({len(diff)} lines)")
                if args.verbose:
                    print("\n".join("    " + l for l in diff[:40]))
        print(f"{len(files)} files against {args.base}: {bad} differ or fail")
        return 1 if bad else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
