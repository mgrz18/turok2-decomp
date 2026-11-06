#!/usr/bin/env python3
"""Drop linker-script symbol definitions that the objects already provide.

splat writes `undefined_funcs_auto` for every function it references but does
not define, and the Makefile feeds that to `ld -T`. But `tools/fix_asm.py`
promotes splat's inner `.globl` labels into real functions at assembly time,
which splat knows nothing about. The two then collide: ld's script definition
wins, the symbol comes out `ABS` instead of section-relative, and N64Recomp
rejects it ("No function found for jal target") because an absolute symbol is
not a function inside a section.

So: compute what the assembled objects actually define, and emit filtered
copies of the symbol files with those removed.

The compiled C objects count too. splat lists in `undefined_funcs_auto` some
named functions that a C file defines, at their named address; for the second
VM module (named 0x39000 above where it is linked) that address is wrong, and
the script definition would move every call to it.

Usage:
    python3 tools/filter_defined_syms.py <out-dir> <sym-file> [<sym-file> ...]
"""

import os
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
ASM_DIRS = (ROOT / "us" / "asm", ROOT / "src" / "us" / "asm")
C_OBJ_DIR = ROOT / "build" / "src"
NM = os.environ.get("NM", "mips-linux-gnu-nm")

GLOBL_RE = re.compile(r"^\s*\.globl\s+(\S+)\s*$")
ASSIGN_RE = re.compile(r"^\s*([A-Za-z_.$][\w.$]*)\s*=")


def defined_symbols():
    """Every symbol the asm sources declare global, indented or not."""
    names = set()
    for directory in ASM_DIRS:
        if not directory.exists():
            continue
        for path in directory.rglob("*.s"):
            with path.open(errors="replace") as fh:
                for line in fh:
                    m = GLOBL_RE.match(line)
                    if m:
                        names.add(m.group(1))
    return names | c_defined_symbols()


def c_defined_symbols():
    """Every global symbol the compiled C objects define."""
    objs = sorted(str(p) for p in C_OBJ_DIR.rglob("*.c.o")) if C_OBJ_DIR.exists() else []
    names = set()
    for i in range(0, len(objs), 500):
        out = subprocess.run([NM, "-g", "--defined-only", *objs[i:i + 500]],
                             check=True, capture_output=True, text=True).stdout
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 3:
                names.add(parts[2])
    return names


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)

    out_dir = pathlib.Path(sys.argv[1])
    out_dir.mkdir(parents=True, exist_ok=True)
    defined = defined_symbols()

    total_dropped = 0
    for arg in sys.argv[2:]:
        src = pathlib.Path(arg)
        if not src.exists():
            continue
        kept, dropped = [], 0
        for line in src.read_text(errors="replace").splitlines():
            m = ASSIGN_RE.match(line)
            if m and m.group(1) in defined:
                dropped += 1
                continue
            kept.append(line)
        (out_dir / src.name).write_text("\n".join(kept) + "\n")
        total_dropped += dropped
        print(f"  {src.name}: -{dropped} ya definidos", file=sys.stderr)

    print(f"  simbolos definidos por los objetos: {len(defined):,}", file=sys.stderr)
    print(f"  quitados de los scripts de ld     : {total_dropped:,}", file=sys.stderr)


if __name__ == "__main__":
    main()
