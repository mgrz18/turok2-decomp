#!/usr/bin/env python3
"""Place the .rodata of second-module C over the first module's pool.

The second VM module was linked against the first module's .rodata: its code
reads the first module's literals, strings and even jump tables (whose
entries point at the first module's code), and the ROM holds that data once
(docs/VMASM.md). A C file for a second-module function still emits its own
.rodata, laid out like the first module's for the same source. Each line of
versions/shared_rodata.us.txt

    func_00451408 0x800C0E84    # the strings "  %02d" and "none"

names such a file and the address of the first-module data its .rodata
stands for. `make setup` (this script, no arguments) gives each one an output
section of its own at that address, NOLOAD: the code's relocations resolve
into the first module's pool, and the bytes come from the first module's
object alone.

`--check` (run by `make verify`) holds every such object's .rodata against
the built ROM at its address, word by word: what the C says has to be what is there. A
jump-table entry names the second module's own label in the object and the
first module's in the ROM; the two copies of a function are laid out alike,
so each entry has to keep its distance from the table's first entry.
"""

import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LIST = ROOT / "versions" / "shared_rodata.us.txt"
LD = ROOT / "us" / "turok2.ld"
ELF = ROOT / "build" / "turok2.us.elf"
Z64 = ROOT / "baserom.us.z64"  # the reference; make verify runs this once the build equals it
MARK = "    /* versions/shared_rodata.us.txt */\n"


def entries():
    out = []
    for line in LIST.read_text().splitlines():
        line = line.split("#")[0].strip()
        if line:
            stem, addr = line.split()
            out.append((stem, int(addr, 16)))
    return out


def obj(stem):
    return f"build/src/us/code/{stem}.c.o"


def write_ld():
    t = LD.read_text()
    if MARK in t:
        t = t[:t.index(MARK)] + t[t.index("    /DISCARD/ :"):]
    block = [MARK]
    for stem, addr in entries():
        if f"{obj(stem)}(.rodata)" in t:
            sys.exit(f"{stem}: its .rodata is already placed by the yaml")
        if f"{obj(stem)}(.text)" not in t:
            sys.exit(f"{stem}: not linked (no c subsegment in the yaml)")
        block.append(f"    .shared_rodata_{stem} 0x{addr:08X} (NOLOAD) :\n    {{\n"
                     f"        {obj(stem)}(.rodata);\n    }}\n")
    i = t.index("    /DISCARD/ :")
    LD.write_text(t[:i] + "".join(block) + t[i:])


def check():
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection

    rom = Z64.read_bytes()
    with ELF.open("rb") as fh:
        elf = ELFFile(fh)
        loads = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD" and s["p_filesz"]]

        def rom_of(vma):
            for s in loads:
                if s["p_vaddr"] <= vma < s["p_vaddr"] + s["p_filesz"]:
                    return s["p_paddr"] + vma - s["p_vaddr"]
            return None

        bad = 0
        for stem, addr in entries():
            with (ROOT / obj(stem)).open("rb") as oh:
                o = ELFFile(oh)
                sec = o.get_section_by_name(".rodata")
                data = sec.data() if sec else b""
                rel = set()
                for s in o.iter_sections():
                    if isinstance(s, RelocationSection) and s.name in (".rel.rodata", ".rela.rodata"):
                        rel |= {r["r_offset"] & ~3 for r in s.iter_relocations()}
            off = rom_of(addr)
            if off is None:
                print(f"{stem}: 0x{addr:08X} is in no loaded segment")
                bad += 1
                continue
            word = lambda b, i: struct.unpack(">I", b[i:i + 4])[0]
            base = None
            for i in range(0, len(data) - len(data) % 4, 4):
                if i in rel:
                    # A jump-table entry: the ROM's names the first module's
                    # label, the object's (its addend) the copy's own. The two
                    # copies of the code are laid out alike, so the entries
                    # must keep their distances from the table's first one.
                    if base is None:
                        base = i
                    elif (word(rom, off + i) - word(rom, off + base)) != (word(data, i) - word(data, base)):
                        print(f"{stem}: jump-table entry 0x{addr + i:08X} is 0x{word(rom, off + i):08X}, "
                              f"which is not where the C's case goes")
                        bad += 1
                    continue
                base = None
                if data[i:i + 4] != rom[off + i:off + i + 4]:
                    print(f"{stem}: 0x{addr + i:08X} is {rom[off + i:off + i + 4].hex()}, "
                          f"the C has {data[i:i + 4].hex()}")
                    bad += 1
    if bad:
        sys.exit(1)
    print(f"shared .rodata: {len(entries())} objects agree with the ROM")


if __name__ == "__main__":
    if "--check" in sys.argv:
        check()
    else:
        write_ld()
