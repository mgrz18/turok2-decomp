#!/usr/bin/env python3
"""Linker-script fixes splat cannot express, applied by `make setup` (round 27, p3).

versions/ld_fill.us.txt, one fix a line (`#` comments):

  <splat path> <hex byte> ...
      Linker padding that does not start on 4. The original linker padded a module's `.rodata` up
      to the next object's alignment with stale bytes; splat can only give them a subsegment of
      their own on a 4-byte boundary (and the script's SUBALIGN(4) aligns every input section), so
      where an object's `.rodata` ends off 4 (a string) the bytes up to the next 4 are written as
      BYTE() statements right after that object's `(.rodata)` line in us/turok2.ld; the rest of the
      padding stays an unnamed `rodata` subsegment.

  <splat path> .data after <splat piece>
      A C file's `.data` inside a segment that splat otherwise fills with `.rodata` pieces
      (code_rodata: the engine's initialised data follows its .rodata). splat writes the line
      `<path>.c.o(.data)` in that segment's DATA group, ahead of every `.rodata` line, and again in
      the C file's own segment (the .c subsegment's auto-linked .data); the one line kept is moved
      to right after the line of the piece before it in ROM (`build/asm/data/<piece>.o(.rodata)`).

`--check` says whether us/turok2.ld has every fix.
"""
import re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LD = ROOT / "us" / "turok2.ld"
LIST = ROOT / "versions" / "ld_fill.us.txt"
MARK = "/* ld_fill */"


def fixes():
    fills, moves = {}, []
    for l in LIST.read_text().splitlines():
        l = l.split("#", 1)[0].strip()
        if not l or l.startswith("//"):
            continue
        tok = l.split()
        if len(tok) == 4 and tok[1] == ".data" and tok[2] == "after":
            moves.append((f"build/src/us/{tok[0]}.c.o(.data);", f"build/asm/data/{tok[3]}.o(.rodata);"))
        else:
            fills[tok[0]] = [int(b, 16) for b in tok[1:]]
    return fills, moves


def apply(lines, fills, moves):
    """-> (new lines, problems)."""
    probs = []
    for src, after in moves:
        # splat writes the line twice: in the C file's own segment (its .c subsegment's auto-linked
        # .data) and in the segment of the .data subsegment; one is kept, after the piece before it
        idx = [i for i, l in enumerate(lines) if l.strip() == src]
        if not idx:
            probs.append(f"no line `{src}`"); continue
        ind = lines[idx[-1]][: len(lines[idx[-1]]) - len(lines[idx[-1]].lstrip())]
        for i in reversed(idx):
            del lines[i]
        at = [i for i, l in enumerate(lines) if l.strip() == after]
        if len(at) != 1:
            probs.append(f"{len(at)} lines `{after}`"); continue
        lines.insert(at[0] + 1, ind + src + "\n")
    out, seen = [], set()
    i = 0
    while i < len(lines):
        l = lines[i]
        if MARK in l:
            i += 1; continue
        out.append(l)
        m = re.match(r"(\s*)build/src/us/(\S+)\.c\.o\(\.rodata\);", l)
        if m and m.group(2) in fills:
            seen.add(m.group(2))
            out.append(m.group(1) + " ".join(f"BYTE(0x{b:02X});" for b in fills[m.group(2)]) + " " + MARK + "\n")
        i += 1
    probs += [f"no `{p}.c.o(.rodata)` line" for p in sorted(set(fills) - seen)]
    return out, probs


def main():
    check = "--check" in sys.argv
    fills, moves = fixes()
    lines = LD.read_text().splitlines(keepends=True)
    out, probs = apply(list(lines), fills, moves)
    for p in probs:
        print(f"ld_fill: {p} in {LD.relative_to(ROOT)}", file=sys.stderr)
    if check:
        if out != lines:
            print("ld_fill: us/turok2.ld lacks fixes from versions/ld_fill.us.txt (run make setup)", file=sys.stderr)
            sys.exit(1)
        sys.exit(1 if probs else 0)
    LD.write_text("".join(out))
    print(f"ld_fill: {len(fills)} fill(s), {len(moves)} move(s) in {LD.relative_to(ROOT)}")
    if probs:
        sys.exit(1)


if __name__ == "__main__":
    main()
