#!/usr/bin/env python3
"""Give a run of functions its own C file in the splat config.

Matched C gets into the ROM through a `c` subsegment: splat writes a .c file
with INCLUDE_ASM for every function in the range, and the matched ones are
then replaced by real C. This inserts that subsegment for a run of functions
in `.code`, and closes it with an `asm` entry so the code after it stays asm.

Where the file begins and ends does not change the ROM. Turok 2's `.text` has
no padding between objects -- 2,852 of 2,879 functions in `.code` sit flush
against the one before, and no function ends in alignment zeros -- so cutting
at any function start links byte-for-byte the same. File boundaries only start
to matter when a file's `.rodata` moves into C too, and until then a file can
be as small as one function. See docs/MATCHING.md.

Usage:
    python3 tools/c_subsegment.py code/probe func_0020578C [func_...] [--write]

The functions must be contiguous. The file is src/us/<name>.c.
"""

import argparse
import re
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
YAML = ROOT / "versions" / "turok2.us.yaml"
ELF = ROOT / "build" / "turok2.us.elf"

ENTRY = re.compile(r"^(\s+)- \[(0x[0-9A-Fa-f]+),\s*(\w+)(?:,\s*([^\]\s]+))?\s*\](.*)$")


def function_ranges(names):
    """name -> (rom start, rom end) from the linked ELF."""
    want = set(names)
    out = {}
    with ELF.open("rb") as fh:
        elf = ELFFile(fh)
        loads = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        for sym in elf.get_section_by_name(".symtab").iter_symbols():
            if sym.name not in want or sym["st_info"]["type"] != "STT_FUNC":
                continue
            v = sym["st_value"]
            for seg in loads:
                if seg["p_vaddr"] <= v < seg["p_vaddr"] + seg["p_filesz"]:
                    rom = seg["p_paddr"] + v - seg["p_vaddr"]
                    out[sym.name] = (rom, rom + sym["st_size"])
                    break
    missing = want - set(out)
    if missing:
        sys.exit(f"not in the ELF: {', '.join(sorted(missing))}")
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("name", help="file name under src/us, without .c")
    ap.add_argument("functions", nargs="+")
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    ranges = sorted(function_ranges(args.functions).values())
    for (_, end), (start, _) in zip(ranges, ranges[1:]):
        if end != start:
            sys.exit("functions are not contiguous; give each run its own file")
    start, end = ranges[0][0], ranges[-1][1]

    lines = YAML.read_text().splitlines()
    # The `code` segment's subsegment list: from its `- name: code` to the next
    # top-level segment.
    seg = next(i for i, l in enumerate(lines) if l.strip() == "- name: code")
    stop = next((i for i in range(seg + 1, len(lines)) if lines[i].startswith("  - ")), len(lines))
    entries = [(i, ENTRY.match(lines[i])) for i in range(seg, stop)]
    entries = [(i, m) for i, m in entries if m]
    if not entries:
        sys.exit("no subsegments found under `- name: code`")
    indent = entries[0][1].group(1)

    starts = {int(m.group(2), 16): (i, m) for i, m in entries}
    before = [a for a in starts if a <= start]
    if not before:
        sys.exit(f"0x{start:X} is before the code segment's first subsegment")
    host_addr = max(before)
    host = starts[host_addr][1]
    if host.group(3) != "asm":
        sys.exit(f"0x{start:X} falls in a `{host.group(3)}` subsegment at 0x{host_addr:X}, not asm")
    nxt = min((a for a in starts if a > start), default=None)
    if nxt is not None and nxt < end:
        sys.exit(f"the run crosses the subsegment at 0x{nxt:X}")

    new = []
    if start != host_addr:
        new.append((start, f"{indent}- [0x{start:X}, c, {args.name}]"))
    else:
        # The run opens exactly where the asm subsegment did: take its line over.
        i = starts[host_addr][0]
        lines[i] = f"{indent}- [0x{start:X}, c, {args.name}]"
    if end != nxt:
        new.append((end, f"{indent}- [0x{end:X}, asm]"))

    at = starts[host_addr][0] + 1
    for _, text in new:
        lines.insert(at, text)
        at += 1

    print("\n".join(text for _, text in new) or f"(replaced 0x{start:X} in place)")
    print(f"  {args.name}: ROM 0x{start:X}-0x{end:X}, {len(ranges)} function(s)")
    if args.write:
        YAML.write_text("\n".join(lines) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
