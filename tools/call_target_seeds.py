#!/usr/bin/env python3
"""Seed the call targets the recompiler is going to ask for, before it asks.

N64Recomp stops at the first `jal` whose target has no function symbol, so
feeding its complaints back one at a time advances one symbol per full
splat/link/recompile cycle. The same addresses are derivable without it: an
unresolved call target is a `jal` target that `function_seed.py` dropped
because it failed the boundary test.

Seeding all of them is wrong, and measurably so -- doing that took the emitted
C from 2.0 MB down to 228 KB. A `jal` target is not reliably a function start
in this ROM. Two filters account for the ones that are not, and both are the
recompiler's own rules turned around:

1. The target is a delay slot. The instruction before it is a branch or jump,
   so the target is that branch's delay slot and cannot begin a function.

2. A branch inside the enclosing function crosses it. A branch cannot leave
   its function, so if one starting before the target lands after it, the
   target is in the middle of a function. This is exactly the condition
   N64Recomp reports as "Unhandled branch".

Of the 321 unresolved targets in the first run, 65 failed the first test and
60 the second. The surviving 195, seeded in one pass, reached the point twelve
feedback iterations had taken to reach.

Run it every iteration, not once: the set grows as the segmentation improves.
Seeding a function makes splat disassemble more of the region around it, which
exposes `jal`s that were sitting inside undecoded `.word` runs. Most of what
the loop asked for after the first batch was not in that batch at all.

Usage:
    python3 tools/call_target_seeds.py            # report
    python3 tools/call_target_seeds.py --write    # merge into recomp_seeds
"""

import argparse
import bisect
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SEEDS = ROOT / "versions" / "symbols_from_scan.us.txt"
OUT = ROOT / "versions" / "recomp_seeds.us.txt"
REJECTED = ROOT / "versions" / "rejected_boundaries.us.txt"

JAL_OPCODE = 0x03


def branch_target(word, vram):
    """Where a branch or jump at `vram` goes, or None if it is neither."""
    op = word >> 26
    if op in (0x02, 0x03):                       # j, jal
        return (vram & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
    off = struct.unpack(">h", struct.pack(">H", word & 0xFFFF))[0]
    if op in (0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17):
        return vram + 4 + off * 4                # beq, bne, blez, bgtz, +likely
    if op == 0x01 and ((word >> 16) & 0x1F) in (0, 1, 0x10, 0x11):
        return vram + 4 + off * 4                # bltz, bgez, and their -al
    if op == 0x11 and ((word >> 21) & 0x1F) == 8:
        return vram + 4 + off * 4                # bc1t, bc1f
    return None


def load_addresses(path):
    out = set()
    if not path.exists():
        return out
    for line in path.read_text(errors="replace").splitlines():
        m = re.search(r"0x([0-9A-Fa-f]+)", line)
        if m:
            out.add(int(m.group(1), 16))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true", help="merge into recomp_seeds")
    args = ap.parse_args()

    import sys
    sys.path.insert(0, str(ROOT / "tools"))
    from function_seed import load_code_segments, vram_to_segment, vram_to_rom, ROM

    rom = ROM.read_bytes()
    segments = load_code_segments()
    seeded = load_addresses(SEEDS)
    rejected = load_addresses(REJECTED)
    ordered = sorted(seeded)

    targets = set()
    for start, end, vram, _name, _owner in segments:
        end = min(end, len(rom) - 3)
        for off in range(start, end, 4):
            word = struct.unpack_from(">I", rom, off)[0]
            if (word >> 26) != JAL_OPCODE:
                continue
            pc = vram + (off - start)
            target = (pc & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
            if vram_to_segment(segments, target):
                targets.add(target)

    unresolved = targets - seeded - rejected
    delay_slot = crossed = 0
    keep = set()

    for target in unresolved:
        off = vram_to_rom(segments, target)
        if off is None or off < 4:
            continue

        if branch_target(struct.unpack_from(">I", rom, off - 4)[0], target - 4) is not None:
            delay_slot += 1
            continue

        i = bisect.bisect_left(ordered, target) - 1
        if i < 0:
            continue
        enclosing = ordered[i]
        base = vram_to_rom(segments, enclosing)
        if base is None or base >= off:
            continue

        for probe in range(base, off, 4):
            word = struct.unpack_from(">I", rom, probe)[0]
            if (word >> 26) in (0x02, 0x03):      # a tail call may leave; a branch may not
                continue
            dest = branch_target(word, enclosing + (probe - base))
            if dest is not None and dest >= target:
                crossed += 1
                break
        else:
            keep.add(target)

    print(f"jal targets landing in a code segment : {len(targets):,}")
    print(f"  already seeded or rejected          : {len(targets) - len(unresolved):,}")
    print(f"  unresolved                          : {len(unresolved):,}")
    print(f"    in a delay slot                   : {delay_slot:,}")
    print(f"    crossed by a branch               : {crossed:,}")
    print(f"    sound, worth seeding              : {len(keep):,}")

    if args.write and keep:
        have = load_addresses(OUT)
        merged = have | keep
        OUT.write_text("\n".join(f"0x{a:08X}" for a in sorted(merged)) + "\n")
        print(f"\nadded             : {len(merged) - len(have):,}")
        print(f"recomp_seeds now  : {len(merged):,}")


if __name__ == "__main__":
    main()
