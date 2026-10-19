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
    python3 tools/progress.py --badge --all   # the same over all the code

The `total` row adds the regions up: every byte of code in the game, the
engine and the VM modules alike.

Two refinements, so the numbers say what is left to do:

  - The paged VM modules are in the ROM twice: the second copy, 0x39000 (or
    0x38F20) after the first, is the first's bytes over again, and its
    absolute jumps still land in the first copy, so it cannot be C of its
    own. A VM function whose bytes equal the one at that distance before it
    counts in neither total nor matched: the code to decompile is the first
    copy.
  - `readable` is the matched code whose C has none of m2c's offset macros
    (M2C_FIELD and the like): C that names its fields, as opposed to C that
    only compiles to the right bytes.
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


VM_COPY = (0x39000, 0x38F20)     # where a VM module's second copy sits after the first


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
    ap.add_argument("--all", action="store_true", help="with --badge: all the code, not just the engine")
    args = ap.parse_args()

    sizes = {}
    total = {"engine": 0, "virtual": 0, "libultra": 0}
    copy = 0
    with ELF.open("rb") as fh:
        elf = ELFFile(fh)
        secs = [s for s in elf.iter_sections() if s["sh_type"] == "SHT_PROGBITS" and s["sh_addr"]]

        def body(addr, size):
            for s in secs:
                if s["sh_addr"] <= addr < s["sh_addr"] + s["sh_size"]:
                    return s.data()[addr - s["sh_addr"]:addr - s["sh_addr"] + size]
            return None

        funcs = {sym["st_value"]: (sym.name, sym["st_size"])
                 for sym in elf.get_section_by_name(".symtab").iter_symbols()
                 if sym["st_info"]["type"] == "STT_FUNC" and sym["st_size"]}
        for addr, (name, size) in funcs.items():
            if region(addr) == "virtual" and any(
                    funcs.get(addr - d, (None, -1))[1] == size and body(addr, size) == body(addr - d, size)
                    for d in VM_COPY):
                copy += size
                continue
            sizes[name] = (size, region(addr))
            total[region(addr)] += size

    done = {"engine": 0, "virtual": 0, "libultra": 0}
    count = {"engine": 0, "virtual": 0, "libultra": 0}
    readable = 0
    for path in linked_c_files():
        src = path.read_text()
        src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)
        plain = "M2C_" not in src.replace('#include "m2c_macros.h"', "")
        for name in DEF.findall(src):
            if name in sizes:
                size, reg = sizes[name]
                done[reg] += size
                count[reg] += 1
                readable += size if plain else 0

    all_done, all_total = sum(done.values()), sum(total.values())
    if args.badge:
        pct = (100 * all_done / all_total) if args.all else 100 * done["engine"] / total["engine"]
        print(f"{pct:.2f}%")
        return 0

    print(f"{'':10s} {'matched':>10s} {'total':>10s} {'':>8s} {'functions':>9s}")
    for reg in ("engine", "virtual", "libultra"):
        t = total[reg] or 1
        print(f"{reg:10s} {done[reg]:10,d} {total[reg]:10,d} {100 * done[reg] / t:7.3f}% {count[reg]:9d}")
    print(f"{'total':10s} {all_done:10,d} {all_total:10,d} {100 * all_done / all_total:7.3f}% {sum(count.values()):9d}")
    print(f"{'readable':10s} {readable:10,d} {all_total:10,d} {100 * readable / all_total:7.3f}%"
          f"   ({100 * readable / (all_done or 1):.0f}% of what is matched)")
    print(f"(not counted: {copy:,d} bytes of the VM modules' second copy)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
