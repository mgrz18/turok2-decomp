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
    python3 tools/progress.py --badge --raw   # either, counting asm-by-origin code too

The `total` row adds the regions up: every byte of code in the game, the
engine and the VM modules alike.

Two refinements, so the numbers say what is left to do:

  - Much of the second VM module is the first's bytes over again: it is
    linked at the first one's addresses (0x39000 below where splat names it,
    docs/VMASM.md), and a function of it whose bytes equal the first
    module's function at its link address (or 0xE0 on) counts in neither
    total nor matched: the code to decompile is the first copy.
  - Code that is not C by origin (hand-written asm: the libultra .s files,
    SN64's SNMAIN/STVM, the RNC unpacker, the exception/debug stubs) and
    fragments that are not whole functions are listed, with a reason each, in
    versions/asm_origin.us.txt (evidence: docs/ASM_ORIGIN.md). They get their
    own row, `asm (by origin)`, and are left out of the per-region, `total`
    and `readable` percentages, which are therefore over C-origin code only
    and reach 100% when every C function is matched. The `all code` row
    keeps the old figure: matched C over every byte, asm included.
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
ASM_ORIGIN = ROOT / "versions" / "asm_origin.us.txt"

# The return type may be on the line before the name (libgcc's style).
DEF = re.compile(r"^(?:[A-Za-z_][\w \t\*]*?\b)?(\w+)\s*\([^;{]*\)\s*\{", re.M)


VM2_NAMED = (0x0043CC14, 0x00475A00)   # the second VM module, at the addresses splat names it
VM2_SHIFT = 0x39000                    # ... which are this far above where it is linked
OLD_COPY = (0x39000, 0x38F20)   # the same distances, between code linked where it is named
VM_COPY = (0, 0xE0)     # where the first module's copy of a second-module function sits,
                        # from the second's link address (named 0x39000 or 0x38F20 apart)


def region(addr):
    if addr >= 0x80000000:
        return "libultra"
    if addr < 0x002A5200:
        return "engine"
    return "virtual"


def asm_origin():
    """name -> kind ("asm" or "fragment"), from versions/asm_origin.us.txt."""
    out = {}
    for line in ASM_ORIGIN.read_text().splitlines():
        line = line.split("#", 1)[0].split()
        if line:
            out[line[0]] = line[1]
    return out


def linked_c_files():
    text = LD_SCRIPT.read_text()
    return sorted({ROOT / m for m in re.findall(r"build/(src/[^ ()]+)\.o", text)})


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--badge", action="store_true")
    ap.add_argument("--all", action="store_true", help="with --badge: all the code, not just the engine")
    ap.add_argument("--raw", action="store_true",
                    help="with --badge: count asm-by-origin code in the total too (the old figure)")
    args = ap.parse_args()
    not_c = asm_origin()

    sizes = {}
    total = {"engine": 0, "virtual": 0, "libultra": 0}
    copy = 0
    with ELF.open("rb") as fh:
        elf = ELFFile(fh)
        secs = [s for s in elf.iter_sections() if s["sh_type"] == "SHT_PROGBITS" and s["sh_addr"]]

        data = {s.name: (s["sh_addr"], s.data()) for s in secs}

        def body(sec, addr, size):
            base, d = data[sec]
            return d[addr - base:addr - base + size]

        # The second VM module (.virtual_1) is linked over the first, so an
        # address can name two functions: key them by section too.
        funcs = {}
        for sym in elf.get_section_by_name(".symtab").iter_symbols():
            if sym["st_info"]["type"] != "STT_FUNC" or not sym["st_size"]:
                continue
            if isinstance(sym["st_shndx"], int):
                sec = elf.get_section(sym["st_shndx"]).name
            elif VM2_NAMED[0] <= sym["st_value"] < VM2_NAMED[1]:   # absolute, by its name in the second module
                funcs[(".virtual_1", sym["st_value"] - VM2_SHIFT)] = (sym.name, sym["st_size"])
                continue
            else:   # defined absolutely: the section that covers it
                sec = next((n for n, (a, d) in data.items() if a <= sym["st_value"] < a + len(d)), None)
            if sec in data:
                funcs[(sec, sym["st_value"])] = (sym.name, sym["st_size"])
        by_addr = {addr: (sec, size) for (sec, addr), (_, size) in funcs.items() if sec != ".virtual_1"}

        def same(sec, addr, size, sec2, addr2):
            return sec2 is not None and body(sec, addr, size) == body(sec2, addr2, size)

        for (sec, addr), (name, size) in funcs.items():
            if sec == ".virtual_1":
                twin = [(".virtual", addr + d) for d in VM_COPY]
            elif region(addr) == "virtual":
                twin = [(by_addr.get(addr - d, (None, 0))[0], addr - d) for d in OLD_COPY]
            else:
                twin = []
            if any(funcs.get(k, (None, -1))[1] == size and same(sec, addr, size, *k) for k in twin):
                copy += size
                continue
            sizes[name] = (size, region(addr))
            total[region(addr)] += size

    done = {"engine": 0, "virtual": 0, "libultra": 0}
    count = {"engine": 0, "virtual": 0, "libultra": 0}
    readable = 0
    matched_names = set()
    for path in linked_c_files():
        src = path.read_text()
        src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)
        plain = "M2C_" not in src.replace('#include "m2c_macros.h"', "")
        for name in DEF.findall(src):
            # A helper several files define (a static copy of a global one,
            # func_0027BC34 in three files) is counted once.
            if name in sizes and name not in matched_names:
                matched_names.add(name)
                size, reg = sizes[name]
                done[reg] += size
                count[reg] += 1
                readable += size if plain else 0

    # Code that is not C by origin: out of the C totals, into its own row.
    asm = {"engine": 0, "virtual": 0, "libultra": 0}
    asm_n = 0
    for name in not_c:
        if name in matched_names:
            print(f"progress.py: {name} is listed in {ASM_ORIGIN.name} but linked C defines it",
                  file=sys.stderr)
            continue
        if name in sizes:   # (a listed name may already be out, as the VM modules' second copy)
            size, reg = sizes[name]
            asm[reg] += size
            asm_n += 1
    c_total = {reg: total[reg] - asm[reg] for reg in total}

    all_done, all_total = sum(done.values()), sum(total.values())
    all_c, all_asm = sum(c_total.values()), sum(asm.values())
    if args.badge:
        tot = total if args.raw else c_total
        pct = (100 * all_done / sum(tot.values())) if args.all else 100 * done["engine"] / tot["engine"]
        print(f"{pct:.2f}%")
        return 0

    def pct(a, b):
        return f"{100 * a / b:7.3f}%" if b else f"{'-':>8s}"

    print(f"{'':16s} {'matched':>10s} {'total':>10s} {'':>8s} {'functions':>9s}")
    for reg in ("engine", "virtual", "libultra"):
        print(f"{reg:16s} {done[reg]:10,d} {c_total[reg]:10,d} {pct(done[reg], c_total[reg])} {count[reg]:9d}")
    print(f"{'total':16s} {all_done:10,d} {all_c:10,d} {pct(all_done, all_c)} {sum(count.values()):9d}"
          f"   (C-origin code)")
    print(f"{'asm (by origin)':16s} {'':>10s} {all_asm:10,d} {'':>8s} {asm_n:9d}"
          f"   ({ASM_ORIGIN.relative_to(ROOT)}: never C)")
    print(f"{'all code':16s} {all_done:10,d} {all_total:10,d} {pct(all_done, all_total)}"
          f"   (asm included, the old figure)")
    print(f"{'readable':16s} {readable:10,d} {all_c:10,d} {pct(readable, all_c)}"
          f"   ({100 * readable / (all_done or 1):.0f}% of what is matched)")
    print(f"(not counted: {copy:,d} bytes of the VM modules' second copy)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
