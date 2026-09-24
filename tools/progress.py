#!/usr/bin/env python3
"""How much of the game's code is matched C.

A function counts as matched when a C file under src/ defines it in C rather
than pulling it in with INCLUDE_ASM, and the file is linked: the ROM only
builds byte-exact (`make verify`) if that C compiles to the original bytes, so
linked C is matched C by construction.

Bytes are the measure, not function counts, so a 30-byte wrapper and a 3 KB
AI routine weigh what they are. The total is every sized function in the
linked ELF, split by where it runs:

    engine   .code, useg 0x00200500-0x002A5200
    virtual  the paged VM modules, useg 0x00400000 and up
    libultra KSEG0 (the SDK; comes from ultralib rather than hand matching)

Usage:
    python3 tools/progress.py            # table
    python3 tools/progress.py --badge    # one line for the README badge
"""

import argparse
import re
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
ELF = ROOT / "build" / "turok2.us.elf"
LD_SCRIPT = ROOT / "us" / "turok2.ld"

DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{", re.M)


def region(addr):
    if addr >= 0x80000000:
        return "libultra"
    if addr < 0x002A5200:
        return "engine"
    return "virtual"


def linked_c_files():
    text = LD_SCRIPT.read_text()
    return sorted({ROOT / m for m in re.findall(r"build/(src/[^ ()]+)\.o", text)})


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--badge", action="store_true")
    args = ap.parse_args()

    sizes = {}
    total = {"engine": 0, "virtual": 0, "libultra": 0}
    with ELF.open("rb") as fh:
        for sym in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if sym["st_info"]["type"] != "STT_FUNC" or not sym["st_size"]:
                continue
            sizes[sym.name] = (sym["st_size"], region(sym["st_value"]))
            total[region(sym["st_value"])] += sym["st_size"]

    done = {"engine": 0, "virtual": 0, "libultra": 0}
    count = {"engine": 0, "virtual": 0, "libultra": 0}
    for path in linked_c_files():
        src = path.read_text()
        src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)
        for name in DEF.findall(src):
            if name in sizes:
                size, reg = sizes[name]
                done[reg] += size
                count[reg] += 1

    if args.badge:
        pct = 100 * done["engine"] / total["engine"]
        print(f"{pct:.2f}%")
        return 0

    print(f"{'':10s} {'matched':>10s} {'total':>10s} {'':>8s} {'functions':>9s}")
    for reg in ("engine", "virtual", "libultra"):
        t = total[reg] or 1
        print(f"{reg:10s} {done[reg]:10,d} {total[reg]:10,d} {100 * done[reg] / t:7.3f}% {count[reg]:9d}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
