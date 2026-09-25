#!/usr/bin/env python3
"""Expand cc1's li.s / li.d the way asn64 does. Filter, stdin to stdout.

- To a float register (`li.s $f0, 6.0`): the constant goes into .rodata and
  is loaded with lui/lwc1 (ldc1 for li.d). That is what the ROM has, where gas
  would build it in registers with lui/mtc1.
- To an integer register (`li.s $5, 14.0`, a float argument passed in $a1
  when the first argument is an int): the float's bits are loaded as an
  integer, `lui $5, 0x4160`, with `li` picking lui, ori or both.

Literals are numbered in order of appearance, which is the order of the
function's pool.
"""

import re
import struct
import sys

LI = re.compile(r"^(\s*)li\.([sd])\s+\$(\w+)\s*,\s*([^\s#]+)")


def main():
    n = 0
    out = []
    for line in sys.stdin.read().splitlines():
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
