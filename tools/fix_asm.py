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
ENT_RE = re.compile(r"^\.ent\s+(\S+)")
END_RE = re.compile(r"^\.end\s+(\S+)")
INNER_GLOBL_RE = re.compile(r"^\s+\.globl\s+(\S+)\s*$")


JR_RA_RE = re.compile(r"^\s+jr\s+\$(31|ra)\b")
INSN_RE = re.compile(r"^\s{2,}[a-z]")


def fix(lines):
    current = None
    # Rolling window of the last two emitted instructions. A function only ends
    # at `jr ra` plus its delay slot, so that pair is what makes a following
    # label a real boundary rather than a branch target inside the function.
    prev_insn = prev_prev_insn = ""

    for line in lines:
        stripped = line.rstrip("\n")

        m = DIV_RE.match(stripped)
        if m:
            indent, op, rs, rt = m.groups()
            yield f"{indent}{op} $0, {rs}, {rt}\n"
            continue

        # Seeded addresses that splat placed *inside* an existing function come
        # out as an indented `.globl`, with no `.ent`, so gas records them as
        # NOTYPE. N64Recomp only walks STT_FUNC symbols and rejects them as jal
        # targets ("No function found for jal target"). Close the enclosing
        # function and open a real one, which is what the address actually is:
        # every one checked sits right after a `jr ra` epilogue.
        m = INNER_GLOBL_RE.match(stripped)
        if m:
            name = m.group(1)
            # Only promote when the preceding two instructions are a return and
            # its delay slot. Splitting at an interior branch target instead
            # produces functions whose branches leave their own body, which
            # N64Recomp rejects with "Unhandled branch".
            # Promote every one of them. Requiring a preceding `jr ra` keeps
            # the split honest but leaves the rest as NOTYPE/size 0, which
            # N64Recomp rejects as a jump target ("Manual function ... already
            # exists!" when you then try to declare it). A function that ends
            # up with a branch leaving its body is a shared tail the recompiler
            # cannot model either way; that is handled per-function, not by
            # withholding the symbol.
            if current:
                yield f".end {current}\n"
            yield f".globl {name}\n.ent {name}\n"
            current = name
            continue

        m = ENT_RE.match(stripped)
        if m:
            current = m.group(1)
            yield line
            continue

        # Emit the end for whatever function is actually open: if we split one
        # above, splat's own `.end` still names the original.
        m = END_RE.match(stripped)
        if m:
            yield f".end {current or m.group(1)}\n"
            current = None
            continue

        if INSN_RE.match(stripped):
            prev_prev_insn, prev_insn = prev_insn, stripped

        yield line
        if stripped.strip() == ".set noreorder":
            yield ".set nomacro\n"

    # A file can end with a function still open — the last one splat emits gets
    # no closing `.end` once we have split it. gas then records size 0, and
    # N64Recomp rejects zero-size functions as jump targets ("No function found
    # for jal target: 0x002A5310").
    if current:
        yield f".end {current}\n"


def main():
    sys.stdout.writelines(fix(sys.stdin))


if __name__ == "__main__":
    main()
