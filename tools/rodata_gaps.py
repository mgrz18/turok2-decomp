#!/usr/bin/env python3
"""What lies between a module directory's .rodata slices (docs/MODULES.md, "Round 25, part n2").

For each unnamed yaml `rodata` subsegment between the directory's first and last `.rodata` slice:

  pad       zeros shorter than 16 bytes: alignment inside one object (merge_module.py takes it along)
  PADDING   non-zero bytes shorter than 16, no symbol in them, before a slice on a 16-byte boundary, and a
            16-aligned function start in range: SN64's linker padding (stale buffer words). The next slice
            is the first of another object; the object starts at one of the listed functions
  DATA      anything else: the module's data that its C reads as an extern (a named string, a float word)
            or the strings of a .data table: a merged file must write it as literals / C data
  OTHER     another directory's slice: one object over two directories (the engine and the VM)

    python3 tools/rodata_gaps.py code/LevelEvent vm1/NameEntryMenu
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ENTRY = re.compile(r"^\s+- \[(0x[0-9A-Fa-f]+),\s*(\.?\w+)(?:,\s*([^\]\s]+))?\s*\]")
RO_VRAM = 0x7FFFF400          # rom -> vram of .rodata, the engine's and the VM's alike


def main():
    ents = [(int(m.group(1), 16), m.group(2), m.group(3))
            for l in (ROOT / "versions/turok2.us.yaml").read_text().splitlines() for m in [ENTRY.match(l)] if m]
    rom = (ROOT / "baserom.us.z64").read_bytes()
    syms = {}
    for f in (ROOT / "versions").glob("symbol*.txt"):
        for l in f.read_text().splitlines():
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)", l)
            if m:
                syms.setdefault(int(m.group(2), 16), m.group(1))
    dirof = lambda p: p.rsplit("/", 1)[0] if p else None
    stem = lambda p: p.rsplit("/", 1)[1]
    for d in sys.argv[1:]:
        d = d.rstrip("/").removeprefix("src/us/")
        txt = [e[2] for e in ents if e[1] == "c" and dirof(e[2]) == d]
        tix = {stem(p): i for i, p in enumerate(txt)}
        ro = [k for k, e in enumerate(ents) if e[1] == ".rodata" and dirof(e[2]) == d]
        print(f"== {d}: {len(txt)} files, {len(ro)} .rodata slices")
        if not ro:
            continue
        last = None
        for k in range(ro[0], ro[-1]):
            a, kind, p = ents[k]
            if dirof(p) == d:
                last = stem(p)
                continue
            nxt, b = ents[k + 1][0], rom[a:ents[k + 1][0]]
            names = [syms[x] for x in sorted(syms) if a + RO_VRAM <= x < nxt + RO_VRAM]
            cls = None
            if p:
                cls = f"OTHER {p}"
            elif nxt - a < 16 and not any(b):
                cls = "pad"
            elif nxt - a < 16 and nxt % 16 == 0 and not names and dirof(ents[k + 1][2]) == d:
                j, i = tix[last], tix[stem(ents[k + 1][2])]
                starts = [stem(q) for q in txt[j + 1:i + 1] if int(stem(q).rsplit("_", 1)[1], 16) % 16 == 0]
                if starts:
                    cls = f"PADDING: a new object starts at {' or '.join(starts)}"
            if cls is None:
                cls = f"DATA {', '.join(names) or '(no symbol)'}"
            print(f"  {a:06X} {nxt - a:4X} {cls}  {b[:16].hex()}")


if __name__ == "__main__":
    main()
