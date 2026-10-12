#!/usr/bin/env python3
"""Find the structs functions receive as their first argument.

m2c drafts `arg0->field` as M2C_FIELD(arg0, s32 *, 0x50C) because it does not
know what arg0 points to. If many functions take the same struct, its layout
shows in their asm: the offsets they load and store through the register that
holds $a0, and with which instruction (lw/lh/lhu/lb/lbu/lwc1...), i.e. the
field's width, signedness and whether it is a float.

This follows $a0 through a straight pass over each function (moves into
s-registers included) and records every access through it. Functions are then
grouped by the offsets they share, and each group's fields are printed with
the access kinds seen at each offset.

Usage:
    python3 tools/struct_scan.py [--region engine|virtual] [--min-funcs 20]
"""

import argparse
import collections
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import auto_match as A  # noqa: E402

ACCESS = re.compile(r"^\s+(lw|lh|lhu|lb|lbu|lwc1|ldc1|sw|sh|sb|swc1|sdc1)\s+\$(\w+),\s*(-?0x[0-9A-Fa-f]+|-?\d+)\(\$(\w+)\)")
MOVE = re.compile(r"^\s+(?:addu|or|daddu)\s+\$(\w+),\s*\$(\w+),\s*\$(?:0|zero)\s*$")
WRITE = re.compile(r"^\s+[a-z][a-z0-9.]*\s+\$(\w+)")
KIND = {"lw": "w", "sw": "w", "lh": "h", "lhu": "hu", "sh": "h", "lb": "b", "lbu": "bu", "sb": "b",
        "lwc1": "f", "swc1": "f", "ldc1": "d", "sdc1": "d"}
REG = {str(i): n for i, n in enumerate(A.REGS)}


def name(r):
    return REG.get(r, r)


def arg0_accesses(body):
    """{offset: Counter(kind)} for accesses through the register holding $a0."""
    holders = {"a0"}
    out = collections.defaultdict(collections.Counter)
    for line in body.splitlines():
        m = ACCESS.match(line)
        if m:
            op, rt, off, base = m.group(1), name(m.group(2)), int(m.group(3), 0), name(m.group(4))
            if base in holders and off >= 0:
                out[off][KIND[op]] += 1
            if op.startswith("l") and rt in holders and not op.endswith("c1"):
                holders.discard(rt)          # the holder was overwritten by a load
            continue
        m = MOVE.match(line)
        if m:
            dst, src = name(m.group(1)), name(m.group(2))
            if src in holders:
                holders.add(dst)
            elif dst in holders:
                holders.discard(dst)
            continue
        if re.match(r"^\s+jal\b", line):
            holders &= {r for r in holders if r.startswith("s")}   # a0-a3, t, v die at a call
            continue
        m = WRITE.match(line)
        if m and not re.match(r"^\s+(?:s[bhwd]|swc1|sdc1|b\w*|j|jr|jalr|mtc1|c\.)", line):
            holders.discard(name(m.group(1)))
        if not holders:
            break
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--region", choices=sorted(A.REGIONS), default="engine")
    ap.add_argument("--min-funcs", type=int, default=20)
    args = ap.parse_args()
    lo, hi = A.REGIONS[args.region]
    bodies = {n: b for n, b in A.extract_asm().items() if lo <= int(n[5:], 16) < hi}
    per_func = {n: arg0_accesses(b) for n, b in bodies.items()}
    per_func = {n: acc for n, acc in per_func.items() if acc}

    # How many functions touch each offset; the rare, high offsets identify a struct.
    users = collections.defaultdict(set)
    for n, acc in per_func.items():
        for off in acc:
            users[off].add(n)
    print(f"{len(bodies)} functions, {len(per_func)} read or write through arg0\n")

    # Group: functions sharing a distinctive offset (>= 0x40, used by many) are
    # taken to receive the same struct. Union them over the common offsets.
    marks = sorted((o for o in users if o >= 0x40 and len(users[o]) >= args.min_funcs),
                   key=lambda o: -len(users[o]))
    parent = {}

    def find(x):
        while parent.setdefault(x, x) != x:
            x = parent[x]
        return x

    for o in marks:
        fs = sorted(users[o])
        for f in fs[1:]:
            parent[find(f)] = find(fs[0])
    groups = collections.defaultdict(set)
    for o in marks:
        for f in users[o]:
            groups[find(f)].add(f)

    for root, fs in sorted(groups.items(), key=lambda kv: -len(kv[1]))[:4]:
        fields = collections.defaultdict(collections.Counter)
        for f in fs:
            for off, kinds in per_func[f].items():
                fields[off].update(kinds)
        size = max(fields) + 4
        print(f"== group of {len(fs)} functions, fields up to 0x{size:X}")
        for off in sorted(fields):
            n = sum(1 for f in fs if off in per_func[f])
            if n * 20 < len(fs) and n < 5:
                continue
            kinds = ", ".join(f"{k}x{c}" for k, c in fields[off].most_common())
            print(f"   0x{off:04X}  in {n:4d} funcs   {kinds}")
        print()
    return 0


if __name__ == "__main__":
    sys.exit(main())


# --- Fitting functions to a known layout -----------------------------------

PRIM = {"f32": ("f", 4), "f64": ("d", 8), "s32": ("w", 4), "u32": ("w", 4), "s16": ("h", 2),
        "u16": ("h", 2), "s8": ("b", 1), "u8": ("b", 1)}
FIELD = re.compile(r"/\*\s*0x([0-9A-Fa-f]+)\s*\*/\s*(?:struct\s+)?(\w+)\s*(\*?)\s*\*?\s*(\w+)((?:\[[^\]]+\])*)\s*;")


def parse_header(text):
    """{struct name: [(offset, type, is_pointer, dims)]} from /* 0xNN */ field lines."""
    structs, cur, fields = {}, None, []
    for line in text.splitlines():
        if re.match(r"^typedef struct", line):
            cur, fields = True, []
            continue
        m = re.match(r"^\}\s*(\w+)\s*;", line)
        if m and cur:
            structs[m.group(1)] = fields
            cur = None
            continue
        m = FIELD.search(line)
        if m and cur:
            dims = [int(d, 0) for d in re.findall(r"\[([^\]]+)\]", m.group(5))]
            fields.append((int(m.group(1), 16), m.group(2), bool(m.group(3)) or "*" in line.split("*/")[1].split(m.group(4))[0], dims))
    structs["CMtxF"] = [(0, "f32", False, [16])]
    return structs


def layout(structs, name, base=0, out=None):
    """{absolute offset: access kind} for every primitive slot of struct `name`."""
    out = {} if out is None else out
    for off, typ, ptr, dims in structs[name]:
        count = 1
        for d in dims:
            count *= d
        if ptr:
            for i in range(count):
                out[base + off + 4 * i] = "w"
        elif typ in PRIM:
            kind, size = PRIM[typ]
            for i in range(count):
                out[base + off + size * i] = kind
        elif typ in structs:
            sub = layout(structs, typ)
            span = max(sub) + 4
            for i in range(count):
                for o, k in sub.items():
                    out[base + off + span * i + o] = k
        else:
            out[base + off] = "w"   # an unknown named type: a pointer-sized slot
    return out


def fits(acc, slots, size):
    """True if every access lies on a slot of the right kind (and some are deep)."""
    if not acc or max(acc) >= size:
        return False
    for off, kinds in acc.items():
        want = slots.get(off)
        if want is None:
            return False
        for k in kinds:
            k = k.rstrip("u")
            if k != want and not (want == "w" and k == "f" and False):
                return False
    return sum(1 for o in acc if o >= 0x18) >= 2
