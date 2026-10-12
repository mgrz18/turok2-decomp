#!/usr/bin/env python3
"""Type the first argument of matched functions with a known struct.

A matched draft reads its argument as M2C_FIELD(arg0, f32 *, 0x18). Once the
struct behind it is known (include/structs/), the same function can read
arg0->m_vVelocity.x: GCC emits the same instructions for both, so the
function still matches, and it reads as code. This rewrites every file under
src/us/code/ whose function

  - takes `void *arg0` (or `s32 arg0`) as its first parameter,
  - uses arg0 only through M2C_FIELD(arg0, T *, off) and as a bare value,
  - and has every (off, T) land on a field of exactly type T

to `TYPE *arg0` with field paths. Each rewritten file is then checked with
tools/match_func.py and put back as it was unless it still matches; run
make verify afterwards, as always.

Usage:
    python3 tools/apply_struct.py structs/game_object.h CGameObjectInstance [--dry-run]
"""

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import struct_scan as S  # noqa: E402

SRC = ROOT / "src" / "us" / "code"
PRIMS = set(S.PRIM)
FIELD_USE = re.compile(r"M2C_FIELD\(arg0, ([\w ]+?\*), (0x[0-9A-Fa-f]+|\d+)\)")


def paths(structs, name, prefix="", base=0, out=None):
    """{offset: (C access path, C type)} for every primitive slot of `name`."""
    out = {} if out is None else out
    for off, typ, ptr, dims in structs[name]:
        field = [f for f in structs[name] if f[0] == off][0]
        fname = FIELD_NAMES[(name, off)]
        count = 1
        for d in dims:
            count *= d

        def index(i):
            idx, rest = [], i
            for d in reversed(dims):
                idx.append(rest % d)
                rest //= d
            return "".join(f"[{x}]" for x in reversed(idx))

        if ptr:
            for i in range(count):
                out[base + off + 4 * i] = (f"{prefix}{fname}{index(i) if dims else ''}", "ptr")
        elif typ in PRIMS:
            size = S.PRIM[typ][1]
            for i in range(count):
                out[base + off + size * i] = (f"{prefix}{fname}{index(i) if dims else ''}", typ)
        elif typ == "CMtxF":
            for i in range(16):
                out[base + off + 4 * i] = (f"{prefix}{fname}[{i // 4}][{i % 4}]", "f32")
        elif typ in structs:
            sub = S.layout(structs, typ)
            span = max(sub) + 4
            for i in range(count):
                paths(structs, typ, f"{prefix}{fname}{index(i) if dims else ''}.", base + off + span * i, out)
        else:
            out[base + off] = (f"{prefix}{fname}", "ptr")
    return out


FIELD_NAMES = {}


def load(header, typename):
    text = (ROOT / "include" / header).read_text()
    if header != "structs/game_object.h":
        text = (ROOT / "include" / "structs" / "game_object.h").read_text() + "\n" + text
    structs = S.parse_header(text)
    # parse_header keeps types and offsets; collect the names here.
    cur = None
    for line in text.splitlines():
        if line.startswith("typedef struct"):
            cur = []
            continue
        m = re.match(r"^\}\s*(\w+)\s*;", line)
        if m and cur is not None:
            for off, name in cur:
                FIELD_NAMES[(m.group(1), off)] = name
            cur = None
            continue
        m = S.FIELD.search(line)
        if m and cur is not None:
            cur.append((int(m.group(1), 16), m.group(4)))
    structs["CMtxF"] = [(0, "f32", False, [16])]
    structs.setdefault("Mtx", [(0, "u8", False, [0x40])])
    FIELD_NAMES.setdefault(("Mtx", 0), "m")
    return paths(structs, typename)


def norm(t):
    return re.sub(r"\s+", " ", t.strip())


MIN_OFFSET = 0


def rewrite(text, slots, typename, header):
    """The file rewritten, or None if the function does not fit."""
    m = re.search(r"^(\w[\w\s\*]*?)\b(func_[0-9A-F]{8})\((void \*|s32 )arg0\b", text, re.M)
    if not m:
        return None
    uses = FIELD_USE.findall(text)
    # One field can land on a slot of the right type by chance (another
    # struct with a word at the same offset); two distinct ones rarely do.
    # A single field deep into the struct (0x80 on) is distinctive enough.
    offs = {int(off, 0) for _, off in uses}
    if len(offs) < 2 and max(offs, default=0) < 0x80:
        return None
    if max(offs, default=0) < MIN_OFFSET:
        return None
    for t, off in uses:
        slot = slots.get(int(off, 0))
        if slot is None:
            return None
        want = slot[1]
        t = norm(t)
        if want == "ptr":
            if t.count("*") < 2:          # M2C_FIELD(arg0, X **, off): a pointer field
                return None
        elif t != f"{want} *":
            return None
    # arg0 must not be used any other way than through the fields or as a bare value.
    rest = FIELD_USE.sub("", text)
    if re.search(r"\barg0\s*[-+\[]|\(\w+ \*\)\s*arg0|&arg0", rest):
        return None
    new = FIELD_USE.sub(lambda u: f"arg0->{slots[int(u.group(2), 0)][0]}", text)
    new = new.replace(m.group(0), f"{m.group(1)}{m.group(2)}({typename} *arg0", 1)
    new = new.replace('#include "m2c_macros.h"\n', f'#include "m2c_macros.h"\n#include "{header}"\n', 1)
    return new


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("header")
    ap.add_argument("typename")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--min-offset", type=lambda x: int(x, 0), default=0,
                    help="require a field at this offset or deeper (CPlayer starts with a "
                         "CGameObjectInstance, so 0x510 tells the two apart)")
    args = ap.parse_args()
    global MIN_OFFSET
    MIN_OFFSET = args.min_offset
    slots = load(args.header, args.typename)
    size = max(slots) + 4
    done = kept = 0
    for path in sorted(SRC.glob("func_*.c")):
        text = path.read_text()
        if args.typename in text:
            continue
        new = rewrite(text, slots, args.typename, args.header)
        if new is None or new == text:
            continue
        done += 1
        if args.dry_run:
            print(f"  would type {path.stem}")
            continue
        path.write_text(new)
        res = subprocess.run([sys.executable, str(ROOT / "tools" / "match_func.py"), str(path)],
                             capture_output=True, text=True)
        if f"{path.stem}" in res.stdout and "MATCH" in res.stdout.split(path.stem, 1)[1].splitlines()[0]:
            kept += 1
            print(f"  typed {path.stem}")
        else:
            path.write_text(text)
            print(f"  reverted {path.stem}: {res.stdout.strip().splitlines()[-1] if res.stdout.strip() else res.stderr.strip()[-200:]}")
    print(f"{kept} of {done} candidates typed ({args.typename}, 0x{size:X} bytes)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
