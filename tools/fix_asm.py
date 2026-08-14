#!/usr/bin/env python3
"""Adjust splat's asm so GNU as emits the original instructions verbatim.

Filter: reads a .s on stdin, writes the corrected .s on stdout.

Two problems, both of which silently changed the output bytes:

1. `div $a, $b` / `divu $a, $b`. GNU as reads the two-operand form as a MACRO
   and expands it into the division plus divide-by-zero and overflow checks,
   which also needs $at. The original SN64 toolchain emitted the bare
   instruction. Writing it three-operand with $0 as the destination
   (`div $0, $a, $b`) is how you ask gas for the plain opcode.

   This was worth 4,412 extra bytes across 162 sites. The first divergence
   from the baserom showed up at ROM 0x160B as a `beq +0x15` that had become
   `beq +0x18` — three instructions of macro expansion in between.

2. No `.set nomacro`. Without it any other pseudo-op can expand unnoticed.
   gas requires it to come after `.set noreorder`, so it is inserted there
   rather than next to `.set noat`.
"""

import re
import sys

DIV_RE = re.compile(r"^(\s*)(divu?)\s+(\$\w+),\s*(\$\w+)\s*$")


def fix(lines):
    for line in lines:
        stripped = line.rstrip("\n")

        m = DIV_RE.match(stripped)
        if m:
            indent, op, rs, rt = m.groups()
            yield f"{indent}{op} $0, {rs}, {rt}\n"
            continue

        yield line
        if stripped.strip() == ".set noreorder":
            yield ".set nomacro\n"


def main():
    sys.stdout.writelines(fix(sys.stdin))


if __name__ == "__main__":
    main()
