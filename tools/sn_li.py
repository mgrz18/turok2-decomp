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
    # A symbol off a base register (`lw $2, jtbl_X($2)`) is left to gas: the
    # ROM has `addu $at, $at, base` there, which is gas's order already.
    # Only large numeric offsets take the reversed order.
    return None


def main():
    n = 0
    out = []
    for line in sys.stdin.read().splitlines():
        exp = expand_mem(line)
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
