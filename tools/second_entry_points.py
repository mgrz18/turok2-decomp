#!/usr/bin/env python3
"""Declare the addresses that are a function's *second* entry point.

Some calls land inside a function rather than at its start. `0x00426898` is
called from five separate sites in `virtual_1` and is also the delay slot of
the `jal` at `0x00426894`, inside `func_00426814`, which reaches past it to
`0x004268BC`. That is legal MIPS and ordinary in hand-written or heavily
optimised code: one body, two ways in.

A symbol table has one name per address, so seeding these splits the function
that contains them and the pieces then branch out of themselves. Measured, that
took the emitted C from 2.0 MB to 228 KB.

N64Recomp solves this itself when caller and callee share a section --
`resolve_jal` returns CreateStatic and emits a static function at the address.
Across sections it declines, because with overlays one vram can belong to
several sections, and the static would also be registered under the caller's
section index. That matters more than it sounds: `main.cpp` computes a static's
ROM address as `addr - section.ram_addr + section.rom_addr` from whichever
section it was filed under, so filing it under the wrong one produces a
function built from the wrong bytes. It would compile and misbehave.

`manual_funcs` avoids all of it. Each entry names its own section, so the ROM
address is computed from the right one, and the function is registered in
`functions_by_vram`, which makes the call resolve as an ordinary match. The
enclosing function is left alone -- the new one simply overlaps it, which the
recompiler is fine with, and its jump tables still size against its unchanged
extent.

Sizes come from the ELF symbol table: a second entry runs to the end of the
function it sits in.

Usage:
    python3 tools/second_entry_points.py           # report
    python3 tools/second_entry_points.py --toml    # emit a manual_funcs block
"""

import argparse
import bisect
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ELF = ROOT / "build" / "turok2.us.elf"
IMAGE = "turok2-build"


def elf_functions():
    """[(vram, size, name)] for every sized function symbol, sorted."""
    out = subprocess.run(
        ["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
         IMAGE, "mips-linux-gnu-nm", "-S", "--defined-only", str(ELF.relative_to(ROOT))],
        capture_output=True, text=True).stdout
    funcs = []
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] in ("t", "T"):
            funcs.append((int(parts[0], 16), int(parts[1], 16), parts[3]))
    funcs.sort()
    return funcs


def elf_sections():
    """[(name, vram, size)] for the loadable code sections."""
    out = subprocess.run(
        ["docker", "run", "--platform=linux/amd64", "--rm", "-v", f"{ROOT}:/work",
         IMAGE, "mips-linux-gnu-readelf", "-S", str(ELF.relative_to(ROOT))],
        capture_output=True, text=True).stdout
    secs = []
    for line in out.splitlines():
        m = re.match(r"\s*\[\s*\d+\]\s+(\S+)\s+PROGBITS\s+([0-9a-f]+)\s+[0-9a-f]+\s+([0-9a-f]+)", line)
        if m:
            secs.append((m.group(1), int(m.group(2), 16), int(m.group(3), 16)))
    return secs


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--toml", action="store_true", help="emit a manual_funcs block")
    args = ap.parse_args()

    sys.path.insert(0, str(ROOT / "tools"))
    from call_target_seeds import branch_target, load_addresses
    from function_seed import load_code_segments, vram_to_segment, vram_to_rom, ROM

    rom = ROM.read_bytes()
    segments = load_code_segments()
    seeded = load_addresses(ROOT / "versions" / "symbols_from_scan.us.txt")
    ordered = sorted(seeded)

    # Every call target, by `jal` or by a `j` that leaves its own function.
    targets = set()
    for start, end, vram, _name, _owner in segments:
        end = min(end, len(rom) - 3)
        for off in range(start, end, 4):
            word = struct.unpack_from(">I", rom, off)[0]
            op = word >> 26
            if op not in (0x02, 0x03):
                continue
            pc = vram + (off - start)
            target = (pc & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
            if not vram_to_segment(segments, target):
                continue
            if op == 0x02:
                i = bisect.bisect_right(ordered, pc) - 1
                if i < 0:
                    continue
                home = ordered[i]
                j = bisect.bisect_right(ordered, home)
                nxt = ordered[j] if j < len(ordered) else 1 << 32
                if home <= target < nxt:
                    continue          # a jump inside its own function
            targets.add(target)

    funcs = elf_functions()
    starts = [f[0] for f in funcs]
    sections = elf_sections()
    by_name = {name for _v, _s, name in funcs}

    entries = []
    unplaced = 0
    for target in sorted(targets - seeded):
        # It only needs declaring if no function starts there.
        i = bisect.bisect_right(starts, target) - 1
        if i < 0:
            continue
        vram, size, _name = funcs[i]
        if vram == target:
            continue
        if target >= vram + size:
            unplaced += 1        # inside no function at all
            continue

        section = next((n for n, sv, ss in sections if sv <= target < sv + ss), None)
        if section is None:
            unplaced += 1
            continue

        name = f"entry_{target:08X}"
        if name in by_name:
            continue
        entries.append((name, section, target, vram + size - target))

    if args.toml:
        print("manual_funcs = [")
        for name, section, vram, size in entries:
            print(f'    {{ name = "{name}", section = "{section}", '
                  f'vram = 0x{vram:08X}, size = {size} }},')
        print("]")
        return

    print(f"call targets                       : {len(targets):,}")
    print(f"  already a function start         : {len(targets & seeded):,}")
    print(f"  second entry into a function     : {len(entries):,}")
    print(f"  in no function or section        : {unplaced:,}")
    if entries:
        print("\nfirst few:")
        for name, section, vram, size in entries[:6]:
            print(f"  {name}  {section:<18} 0x{vram:08X}  {size} bytes")


if __name__ == "__main__":
    main()
