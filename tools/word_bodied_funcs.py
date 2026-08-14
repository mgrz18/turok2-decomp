#!/usr/bin/env python3
"""Reject boundaries whose body splat refused to disassemble.

The after-a-return rule assumes whatever follows `jr ra` and its delay slot
begins a function. Usually true, and sometimes the thing that follows is data:

    0x0016CC  03E00008  jr ra
    0x0016D0  00000000  nop            <- delay slot
    0x0016D4  436F6C6F  "Colo"         <- seeded as func_00200AD4
    0x0016D8  72190000  "r..."
    0x0016DC  00960800
    0x0016E0  27BDFFE8  addiu sp, sp, -0x18   <- the real function

A twelve-byte blob holding the string "Color" sits between two functions, and
the rule labelled it. splat then says so plainly: it emits the body as `.word`
rather than instructions, because it is not code.

So the asm is the check. A `.ent`/`.end` block containing no instruction at all
is not a function, whatever our scan believed. This is worth having as its own
pass rather than folded into the recompiler feedback, because it needs no run
to find -- splat has already made the judgement and written it down.

These also come out sized 0 in the ELF, since nothing emits their `.end`, and
N64Recomp skips a zero-sized symbol when resolving a call ("No function found
for jal target"). Rejecting them fixes both at once.

Usage:
    python3 tools/word_bodied_funcs.py            # report
    python3 tools/word_bodied_funcs.py --write    # merge into rejected_boundaries
"""

import argparse
import glob
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ASM = ROOT / "us" / "asm"
REJECTED = ROOT / "versions" / "rejected_boundaries.us.txt"

ENT = re.compile(r"^\.ent\s+(\S+)")
END = re.compile(r"^\.end\s+(\S+)")
LABEL = re.compile(r"^\S+:")
WORD = re.compile(r"^\s+\.word\b")
INSN = re.compile(r"^\s{2,}[a-z]")
TOP_GLOBL = re.compile(r"^\.globl\b")


def scan():
    """[(name, words, path)] for every .ent block holding no instruction."""
    found = []
    for path in sorted(glob.glob(str(ASM / "*.s"))):
        name = None
        words = insns = 0
        for line in open(path, errors="replace"):
            stripped = line.rstrip("\n")

            m = ENT.match(stripped)
            if m:
                name, words, insns = m.group(1), 0, 0
                continue

            if name is None:
                continue

            # A block runs to its `.end`, or -- since splat emits none for a
            # body it could not disassemble, which is the whole point here --
            # to whatever starts the next one. That is a top-level label or a
            # top-level `.globl`; missing the `.globl` case ran the block on
            # into the next function, found its instructions, and cleared the
            # very flag being tested.
            if (END.match(stripped)
                    or TOP_GLOBL.match(stripped)
                    or (LABEL.match(stripped) and not stripped.startswith(name))):
                if words and not insns:
                    found.append((name, words, Path(path).name))
                name = None
                continue

            if WORD.match(stripped):
                words += 1
            elif INSN.match(stripped):
                insns += 1
    return found


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--write", action="store_true",
                    help="merge into rejected_boundaries")
    args = ap.parse_args()

    found = scan()
    print(f"boundaries whose body is only `.word`: {len(found):,}")
    for name, words, path in found[:10]:
        print(f"  {name:<20} {words:>4} words   {path}")
    if len(found) > 10:
        print(f"  ... and {len(found) - 10:,} more")

    addrs = set()
    for name, _words, _path in found:
        m = re.search(r"([0-9A-Fa-f]{8})$", name)
        if m:
            addrs.add(int(m.group(1), 16))

    if args.write and addrs:
        have = set()
        if REJECTED.exists():
            for line in REJECTED.read_text().split():
                try:
                    have.add(int(line, 16))
                except ValueError:
                    pass
        merged = have | addrs
        REJECTED.write_text("\n".join(f"0x{a:08X}" for a in sorted(merged)) + "\n")
        print(f"\nadded             : {len(merged) - len(have):,}")
        print(f"rejected now      : {len(merged):,}")


if __name__ == "__main__":
    main()
