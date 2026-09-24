#!/usr/bin/env python3
"""Turn a data file's `.align` lines into `.org` at the exact offset.

gas computes `.align 3` from the start of the section in the file, assuming
the section itself lands 8-aligned. A function's slice of `.rdata` can end on
a 4-byte boundary (floats pack at 4 within a source file; only jump tables
and file boundaries go to 8), so the asm that resumes after it starts at
4 mod 8 and every `.align 3` in it pads by the wrong amount. Splat also relies
on `.align` for some padding, so dropping the lines is not an option either:
that shortened the ROM by 2,288 bytes.

Splat puts each `.align` right before a label whose name carries its address
(`D_800A7E60`, `jtbl_800A7E60`), and the file's first such label is its base.
MIPS gas also auto-aligns `.double` from the file's start, which `.align 0`
turns off. So each `.align` followed by an address label becomes `.org label - base`,
which is what `.align` gives when the base is aligned, and the right thing
when it is not. Lines this cannot resolve pass through unchanged.

Usage: filter, stdin to stdout.
"""

import re
import sys

LABEL = re.compile(r"^(?:\.globl\s+)?(?:D|jtbl)_([0-9A-F]{8}):?\s*$")


def main():
    lines = sys.stdin.read().splitlines()
    base = None
    for line in lines:
        m = LABEL.match(line.strip())
        if m:
            base = int(m.group(1), 16)
            break
    out = []
    for i, line in enumerate(lines):
        if base is not None and line.startswith(".section"):
            # MIPS gas also aligns `.double`/`.word` data automatically, from
            # the file's start like `.align`; `.align 0` turns that off. The
            # `.org` lines below carry the padding that is really there.
            out.append(line)
            out.append(".align 0")
            continue
        if base is not None and re.match(r"^\s*\.align\s+\d+\s*$", line):
            # The label this `.align` pads for: the next one, provided no data
            # comes first (an anonymous string between would own the padding).
            target = None
            for nxt in lines[i + 1:i + 7]:
                t = nxt.strip()
                m = LABEL.match(t)
                if m:
                    target = int(m.group(1), 16)
                    break
                if t and not t.startswith(".globl") and not t.startswith(".align"):
                    break
            if target is not None and target >= base:
                out.append(f".org 0x{target - base:X}")
                continue
            # No address label to aim at: keep it (a `.align 2` after a string
            # is right from any 4-aligned base), but any `.align` turns gas's
            # auto-alignment back on, so switch it off again.
            out.append(line)
            out.append(".align 0")
            continue
        out.append(line)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
