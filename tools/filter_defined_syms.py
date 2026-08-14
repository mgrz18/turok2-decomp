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

Usage:
    python3 tools/filter_defined_syms.py <out-dir> <sym-file> [<sym-file> ...]
"""

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
ASM_DIRS = (ROOT / "us" / "asm", ROOT / "src" / "us" / "asm")

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
