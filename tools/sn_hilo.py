#!/usr/bin/env python3
"""Place multiplies and divides around delay slots the way asn64 does.
Filter, stdin to stdout, run on cc1's output after tools/sn_li.py.

asn64 keeps the VR4300's HI/LO hazard (an mfhi/mflo must be two instructions
ahead of the next mult/div, mthi or mtlo) everywhere, in noreorder regions too,
and does not let an integer multiply or divide sit in a delay slot. Probed with
wine asn64.exe on small inputs:

- A mult/multu/div/divu (and the d- forms) that cc1 put in a delay slot under
  `.set noreorder` is moved in front of its branch or jump, and the slot gets
  a nop. That holds for every kind of branch, likely ones and `jr $31`
  included, and a label on the branch stays on the moved instruction. The ROM
  has no integer multiply or divide in any delay slot.
- It is not moved when the instruction before the branch is an mfhi/mflo (the
  slot needs a hazard nop there); asn64 then puts that nop before the branch
  and leaves the slot alone.
- The move is not checked again for the hazard: `mflo; addu; bgez; mult`
  becomes `mflo; addu; mult; bgez; nop`, one instruction short of the
  distance asn64 keeps everywhere else. gas would pad it, so the moved
  instruction is assembled with `.set arch=vr5500`, whose HI/LO interlocks
  switch gas's check off for it.
- The mul.s/mul.d -> multiply nop of the VR4300 erratum is still put in when
  the moved instruction lands right after a mul.s/mul.d.
- Inside a noreorder region asn64 still pads the hazard (`mflo; mult` gets two
  nops, a slot's go before its branch); gas pads only when the mfhi/mflo comes
  before the `.set noreorder`, so the ones inside are padded here.
"""

import re
import sys

HILO_READ = {"mfhi", "mflo"}
HILO_WRITE = {"mult", "multu", "dmult", "dmultu", "div", "divu", "ddiv", "ddivu", "mthi", "mtlo"}
MOVABLE = {"mult", "multu", "dmult", "dmultu", "div", "divu", "ddiv", "ddivu"}
BRANCH = {"b", "bal", "beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez", "bltzal", "bgezal",
          "beql", "bnel", "beqzl", "bnezl", "blezl", "bgtzl", "bltzl", "bgezl", "bltzall", "bgezall",
          "bc1t", "bc1f", "bc1tl", "bc1fl", "j", "jal", "jr", "jalr"}


def mnemonic(line):
    """The instruction's mnemonic, or None for a label, directive, comment or blank."""
    s = line.split("#", 1)[0].strip()
    while True:
        m = re.match(r"^[$.\w]+:\s*", s)
        if not m:
            break
        s = s[m.end():]
    if not s or s.startswith("."):
        return None
    return s.split()[0]


def movable(line):
    """A real (two-operand, or `div $0, rs, rt`) multiply or divide."""
    op = mnemonic(line)
    if op not in MOVABLE:
        return False
    args = [a.strip() for a in line.split("#", 1)[0].strip()[len(op):].split(",")]
    return len(args) == 2 or (len(args) == 3 and args[0] in ("$0", "$zero"))


def main():
    lines = sys.stdin.read().splitlines()
    out = []
    noreorder = False
    region = []      # mnemonics emitted since the last `.set noreorder` (with nops)
    prev = None      # last instruction emitted, any region
    i = 0
    while i < len(lines):
        line = lines[i]
        if re.match(r"^\s*\.set\s+noreorder\b", line):
            noreorder, region = True, []
        elif re.match(r"^\s*\.set\s+reorder\b", line):
            noreorder = False
        op = mnemonic(line)
        if op is None:
            out.append(line)
            i += 1
            continue
        if noreorder and op in BRANCH and i + 1 < len(lines) and mnemonic(lines[i + 1]) in HILO_WRITE:
            slot = lines[i + 1]
            if movable(slot) and prev not in HILO_READ:
                if prev in ("mul.s", "mul.d"):
                    out.append("\tnop")
                sop = mnemonic(slot)
                if "div" in sop and slot.split("#", 1)[0].count(",") == 1:
                    # gas reads a two-operand div as the checking macro
                    slot = re.sub(rf"\b{sop}\s+", f"{sop} $0,", slot, count=1)
                out += ["\t.set push", "\t.set arch=vr5500", slot, "\t.set pop", line, "\tnop"]
                region += [mnemonic(slot), op, "nop"]
                prev = "nop"
                i += 2
                continue
            # The slot stays: its hazard nops go before the branch.
            need = 1 if region[-1:] and region[-1] in HILO_READ else 0
            out += ["\tnop"] * need
            region += ["nop"] * need + [op, mnemonic(slot)]
            out += [line, slot]
            prev = mnemonic(slot)
            i += 2
            continue
        if noreorder and op in HILO_WRITE:
            need = 0
            if region[-1:] and region[-1] in HILO_READ:
                need = 2
            elif region[-2:-1] and region[-2] in HILO_READ:
                need = 1
            out += ["\tnop"] * need
            region += ["nop"] * need
        out.append(line)
        region.append(op)
        prev = op
        i += 1
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
