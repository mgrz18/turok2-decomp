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

import pathlib
import re
import sys

DIV_RE = re.compile(r"^(\s*)(divu?)\s+(\$\w+),\s*(\$\w+)\s*$")

# Addresses N64Recomp has told us are not function boundaries: it reported a
# branch leaving the function that starts there, which means the label sits
# inside a larger function rather than beginning one. Promoting them anyway is
# what produced the "Unhandled branch" class of failure. The list is generated
# by running the recompiler, so it corrects itself as the segmentation improves.
_BAD = pathlib.Path(__file__).resolve().parent.parent / "versions" / "rejected_boundaries.us.txt"
BAD_BOUNDARIES = set()
if _BAD.exists():
    for _line in _BAD.read_text().split():
        try:
            BAD_BOUNDARIES.add(int(_line, 16))
        except ValueError:
            pass


def _addr_of(name):
    m = re.search(r"([0-9A-Fa-f]{8})$", name)
    return int(m.group(1), 16) if m else None
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
            # Promote only at a real boundary — the two preceding instructions
            # being a return and its delay slot.
            #
            # Promoting the rest was working against the recompiler rather than
            # with it. N64Recomp's resolve_jal already handles a call landing
            # inside a known function: when the target is in the current
            # section and no function starts exactly there, it returns
            # CreateStatic and emits one itself. Splitting the function to
            # provide that entry point instead leaves the parent too small, so
            # its own jump tables end up pointing outside it and sizing fails
            # ("Failed to determine size of jump table"). func_0029C270 came
            # out 0x28 bytes against a body of 0x658 that way.
            if not JR_RA_RE.match(prev_prev_insn):
                yield line
                continue

            # The list above was loaded and never consulted, so every address
            # the recompiler rejected got promoted again on the next pass. That
            # is why feeding its complaints back changed nothing: the loop kept
            # re-creating the boundary it had just been told was wrong, and the
            # same `.L0021EC4C` came back ten runs in a row.
            addr = _addr_of(name)
            if addr is not None and addr in BAD_BOUNDARIES:
                yield line
                continue

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
