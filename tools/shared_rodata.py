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

A merged second-module file (tools/merge_module.py: one C file per module) is
one object, but its original files' .rodata went to different places of the
first module's pool. Its lines name the part of the file that each original
file was, by the first function that file emitted:

    MpMenu_2:FragTagMenu_TimeLimitLabel_2 0x800C0E84

`--split STEM` (tools/sn_as.sh, between cc1 and the assembler) then moves the
.rodata cc1 writes for the functions from that one up to the next part's into
a section `.rodata.<function>`: a string or literal written before a function
goes with it, a jump table with the function it is in, a literal pool of
tools/sn_li.py with the function it was flushed after. Each part gets its own
output section as a file did, and `--check` holds each part against the ROM.
"""

import re
import struct
import sys
from pathlib import Path

import srcfiles

ROOT = Path(__file__).resolve().parent.parent
LIST = ROOT / "versions" / "shared_rodata.us.txt"
LD = ROOT / "us" / "turok2.ld"
ELF = ROOT / "build" / "turok2.us.elf"
Z64 = ROOT / "baserom.us.z64"  # the reference; make verify runs this once the build equals it
MARK = "    /* versions/shared_rodata.us.txt */\n"


def entries():
    """(stem, part, address): part is None for a file placed whole, else the first function of the part."""
    out = []
    for line in LIST.read_text().splitlines():
        line = line.split("#")[0].strip()
        if line:
            stem, addr = line.split()
            stem, _, part = stem.partition(":")
            out.append((stem, part or None, int(addr, 16)))
    return out


def obj(stem):
    return srcfiles.obj(stem) or f"build/src/us/code/{stem}.c.o"


def section(part):
    return f".rodata.{part}" if part else ".rodata"


RODATA = re.compile(r"^\s*(\.section\s+\.r(o)?data\b[^#]*|\.rdata\b)\s*(#.*)?$")
DIRECTIVE = re.compile(r"^\s*\.(section|text|data|rdata|bss|sdata|sbss|previous)\b")
ENT = re.compile(r"^\s*\.ent\s+(\S+)")
END = re.compile(r"^\s*\.end\s+(\S+)")


def split(stem):
    """Filter (stdin to stdout) for the assembler input of a merged file with parts: each .rodata chunk
    goes to the section of its part (see the module docstring)."""
    starts = [p for s, p, _ in entries() if s == stem and p]
    lines = sys.stdin.read().splitlines()
    if not starts:
        sys.stdout.write("\n".join(lines) + "\n")
        return
    ents = [(n, ENT.match(l).group(1)) for n, l in enumerate(lines) if ENT.match(l)]
    order = [f for _, f in ents]
    missing = [p for p in starts if p not in order]
    if missing:
        sys.exit(f"{stem}: parts name functions the file does not emit: {missing}")
    group, cur = {}, None
    for f in order:
        cur = f if f in starts else cur
        group[f] = cur
    out, inside, last, sec = [], None, None, None
    for n, l in enumerate(lines):
        m = ENT.match(l)
        if m:
            inside = last = m.group(1)
        elif END.match(l):
            inside = None
        if RODATA.match(l):
            if "sn_li" in l:            # a literal pool flushed at the next directive: the function it was written in
                owner = last
            elif inside:                # a jump table
                owner = inside
            else:                       # strings and constants written before the function that uses them
                owner = next((f for k, f in ents if k > n), last)
            part = group.get(owner)
            if part is None:
                sys.exit(f"{stem}: .rodata at line {n + 1} ({owner}) is before the file's first part")
            out.append(f"\t.section {section(part)}")
            continue
        out.append(l)
    sys.stdout.write("\n".join(out) + "\n")


def write_ld():
    t = LD.read_text()
    if MARK in t:
        t = t[:t.index(MARK)] + t[t.index("    /DISCARD/ :"):]
    block = [MARK]
    for stem, part, addr in entries():
        if f"{obj(stem)}(.rodata)" in t:
            sys.exit(f"{stem}: its .rodata is already placed by the yaml")
        if f"{obj(stem)}(.text)" not in t:
            sys.exit(f"{stem}: not linked (no c subsegment in the yaml)")
        name = f"{stem}_{part}" if part else stem
        block.append(f"    .shared_rodata_{name} 0x{addr:08X} (NOLOAD) :\n    {{\n"
                     f"        {obj(stem)}({section(part)});\n    }}\n")
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
        for stem, part, addr in entries():
            sname = section(part)
            if part:
                stem = f"{stem}:{part}"
            with (ROOT / obj(stem.split(":")[0])).open("rb") as oh:
                o = ELFFile(oh)
                sec = o.get_section_by_name(sname)
                data = sec.data() if sec else b""
                rel = set()
                for s in o.iter_sections():
                    if isinstance(s, RelocationSection) and s.name in (".rel" + sname, ".rela" + sname):
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
    elif "--split" in sys.argv:
        split(sys.argv[sys.argv.index("--split") + 1])
    else:
        write_ld()
