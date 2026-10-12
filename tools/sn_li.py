#!/usr/bin/env python3
"""Expand cc1's macros the way asn64 does. Filter, stdin to stdout.

- To a float register (`li.s $f0, 6.0`): the constant goes into .rodata and
  is loaded with lui/lwc1 (ldc1 for li.d). That is what the ROM has, where gas
  would build it in registers with lui/mtc1.
- To an integer register (`li.s $5, 14.0`, a float argument passed in $a1
  when the first argument is an int): the float's bits are loaded as an
  integer, `lui $5, 0x4160`, with `li` picking lui, ori or both.

Literals are numbered in order of appearance, which is the order of the
function's pool.

A division (`div $3,$5,$2`, `rem`, `divu`, `remu`) is a macro too, with
checks for a zero divisor (break 7) and, signed, for INT_MIN / -1 (break
6). asn64 issues the div first and leaves the bnez's delay slot empty; gas
moves the div into it. All 137 divisions in the ROM have asn64's form.

A load or store with an offset too large for one instruction (`sb $2,
147425($4)`) is a macro too. asn64 builds the address in $at as `lui $at, hi;
addu $at, base, $at`; gas writes `addu $at, $at, base`, and for a load may
use the destination register instead. A symbol off a base register (`lw $2,
jtbl_X($2)`) is different: there the ROM has gas's order, so it is left alone.
"""

import re
import struct
import sys

LI = re.compile(r"^(\s*)li\.([sd])\s+\$(\w+)\s*,\s*([^\s#]+)")
MEM = re.compile(r"^(\s*)(lb|lbu|lh|lhu|lw|lwl|lwr|ld|sb|sh|sw|swl|swr|sd|lwc1|swc1|ldc1|sdc1|l\.s|s\.s|l\.d|s\.d)"
                 r"\s+(\$\w+)\s*,\s*([^\s(#]+)\((\$\w+)\)\s*(#.*)?$")


def expand_mem(line):
    """asn64's expansion of a load/store that needs $at, or None."""
    m = MEM.match(line)
    if not m:
        return None
    ind, op, rt, off, base = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
    try:
        n = int(off, 0)
    except ValueError:
        n = None
    if n is not None:
        if -0x8000 <= n <= 0x7FFF:
            return None
        hi = ((n + 0x8000) >> 16) & 0xFFFF
        lo = n - (((n + 0x8000) >> 16) << 16)
        return [f"{ind}lui $at,0x{hi:X}", f"{ind}addu $at,{base},$at", f"{ind}{op} {rt},{lo}($at)"]
    # A symbol off a base register (`lbu $2, D_X($4)`): the ROM has
    # `lui $at; addu $at, $at, base`. gas writes that order too, but for a
    # load it builds the address in the destination register instead of $at,
    # so loads are expanded here; stores already come out right from gas.
    if op.startswith(("s", "sw", "sd")) and not op.startswith("sub"):
        return None
    if not re.match(r"^[A-Za-z_.$][\w.$]*([+-](0x[0-9A-Fa-f]+|\d+))?$", off):
        return None
    return [f"{ind}lui $at,%hi({off})", f"{ind}addu $at,$at,{base}", f"{ind}{op} {rt},%lo({off})($at)"]


DIV = re.compile(r"^(\s*)(div|divu|rem|remu)\s+(\$\w+)\s*,\s*(\$\w+)\s*,\s*(\$\w+)\s*(#.*)?$")


def expand_div(line, n, noreorder):
    """asn64's expansion of a three-operand division macro, or None.
    cc1 only writes macros in reorder regions; one inside a noreorder region
    is expanded without switching modes."""
    m = DIV.match(line)
    if not m:
        return None
    ind, op, rd, rs, rt = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
    unsigned = op.endswith("u")
    ok = f".Lsn_div{n}"
    out = [f"{ind}.set noreorder", f"{ind}.set noat"] if not noreorder else [f"{ind}.set noat"]
    out += [
           f"{ind}{'divu' if unsigned else 'div'} $0,{rs},{rt}",
           f"{ind}bnez {rt},{ok}a", f"{ind}nop", f"{ind}break 7", f"{ok}a:"]
    if not unsigned:
        out += [f"{ind}li $1,-1", f"{ind}bne {rt},$1,{ok}b", f"{ind}lui $1,0x8000",
                f"{ind}bne {rs},$1,{ok}b", f"{ind}nop", f"{ind}break 6", f"{ok}b:"]
    out += [f"{ind}{'mfhi' if op.startswith('rem') else 'mflo'} {rd}", f"{ind}.set at"]
    if not noreorder:
        out.append(f"{ind}.set reorder")
    return out


def main():
    n = 0
    ndiv = 0
    noreorder = False
    out = []
    for line in sys.stdin.read().splitlines():
        ndiv += 1
        if re.match(r"^\s*\.set\s+noreorder", line):
            noreorder = True
        elif re.match(r"^\s*\.set\s+reorder", line):
            noreorder = False
        exp = expand_div(line, ndiv, noreorder) or expand_mem(line)
        if exp:
            out += exp
            continue
        m = LI.match(line)
        if not m:
            out.append(line)
            continue
        ind, kind, reg, value = m.groups()
        if reg.startswith("f"):
            n += 1
            lab = f".Lsn_lit{n}"
            if kind == "d":
                out += ["\t.section .rodata", "\t.align 3", f"{lab}:", f"\t.double {value}", "\t.text",
                        f"\tlui $at,%hi({lab})", f"\tldc1 ${reg},%lo({lab})($at)"]
            else:
                out += ["\t.section .rodata", "\t.align 2", f"{lab}:", f"\t.float {value}", "\t.text",
                        f"\tlui $at,%hi({lab})", f"\tlwc1 ${reg},%lo({lab})($at)"]
        else:
            if kind != "s":
                out.append(line)   # li.d into an integer pair: not seen yet; leave to gas
                continue
            bits = struct.unpack(">I", struct.pack(">f", float(value)))[0]
            out.append(f"{ind}li ${reg},0x{bits:08X}")
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
