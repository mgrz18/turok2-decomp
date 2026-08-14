#!/usr/bin/env python3
"""Find jump-table targets so they are never mistaken for function starts.

A `switch` compiles to a table of code addresses in rodata plus a `jr` through
it. Those addresses are branch targets *inside* a function, never the start of
one — but they look exactly like function entries to an address scan, and
seeding them splits the enclosing function into pieces its own jumps then leave.

That is what N64Recomp reports as "Failed to determine size of jump table":
it validates each entry against the function's declared bounds, and if the
function has been chopped up, every entry falls outside it and the table reads
as empty. func_0029C270 came out 40 bytes against a table whose seven entries
span 0x290.

Detection: a run of consecutive 32-bit words in a data region that are all
valid code addresses. Three in a row is already far past chance — code
addresses occupy a narrow slice of the 32-bit space.

Usage:
    python3 tools/jumptable_targets.py            # report
    python3 tools/jumptable_targets.py --write    # merge into bad_boundaries
"""

import argparse
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ROM = ROOT / "baserom.us.z64"
BAD = ROOT / "versions" / "bad_boundaries.us.txt"

MIN_RUN = 3


def load_regions():
    """(rom_start, rom_end, vram) for every segment, split into code and data."""
    import yaml
    with (ROOT / "versions" / "turok2.us.yaml").open() as fh:
        doc = yaml.safe_load(fh)
    tops = []
    for seg in doc["segments"]:
        tops.append({"start": seg[0]} if isinstance(seg, list) else seg)
    tops.sort(key=lambda s: s["start"])

    code, data = [], []
    for i, seg in enumerate(tops):
        if "vram" not in seg:
            continue
        seg_end = tops[i + 1]["start"] if i + 1 < len(tops) else seg["start"]
        subs = seg.get("subsegments")
        if not subs:
            continue
        starts = [s[0] if isinstance(s, list) else s["start"] for s in subs]
        for j, sub in enumerate(subs):
            if not isinstance(sub, list):
                continue
            start = sub[0]
            kind = sub[1] if len(sub) > 1 else None
            end = starts[j + 1] if j + 1 < len(starts) else seg_end
            vram = seg["vram"] + (start - seg["start"])
            (code if kind in ("asm", "hasm") else data).append((start, end, vram))
    return code, data


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true", help="merge into bad_boundaries")
    args = ap.parse_args()

    rom = ROM.read_bytes()
    code, data = load_regions()

    def is_code_addr(a):
        return any(v <= a < v + (e - s) for s, e, v in code)

    targets = set()
    tables = 0
    for start, end, _vram in data:
        run = []
        for off in range(start, min(end, len(rom) - 3), 4):
            word = struct.unpack_from(">I", rom, off)[0]
            if is_code_addr(word):
                run.append(word)
                continue
            if len(run) >= MIN_RUN:
                tables += 1
                targets.update(run)
            run = []
        if len(run) >= MIN_RUN:
            tables += 1
            targets.update(run)

    print(f"jump tables found : {tables:,}")
    print(f"target addresses  : {len(targets):,}")

    if args.write:
        have = set()
        if BAD.exists():
            for line in BAD.read_text().split():
                try:
                    have.add(int(line, 16))
                except ValueError:
                    pass
        merged = have | targets
        BAD.write_text("\n".join(f"0x{a:08X}" for a in sorted(merged)) + "\n")
        print(f"added             : {len(merged) - len(have):,}")
        print(f"bad_boundaries now: {len(merged):,}")


if __name__ == "__main__":
    main()
