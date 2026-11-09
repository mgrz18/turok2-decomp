#!/usr/bin/env python3
"""Byte-order spec checker: applies tools/assets/swap_spec.json to every load unit of the ROM.

The port keeps the archive in ROM byte order (big-endian, RNC unpacked) and swaps each unit once,
in T2Asset_Read, when the host is little-endian (docs/assets/PORT.md 5.3, docs/assets/BYTEORDER.md).
swap_spec.json says, per unit type (romfs node type = CCache description), which bytes are fields
of which width. This tool interprets it on the real data:

  * every byte of every unit gets one class:
        swap   a 2/4/8-byte field (byte-reversed on a little-endian host)
        byte   a 1-byte field (u8/s8/char, packed byte streams): no swap
        rdp    16-bit texels / TLUTs that Fast3D reads as big-endian bytes: no swap
        opaque bit/byte streams that a decoder reads byte-wise (VADPCM frames, I4/CI texels): no swap
        pad    known padding / alignment / unused bytes
        guess  a field whose width was inferred from the data, not from the C (counted as covered, listed)
        unk    in the spec, but the width is not known yet (NOT covered)
        none   not reached by any rule (NOT covered; a spec bug or a gap)
  * a byte claimed twice is an error (overlapping rules), so are structures that do not parse;
  * --apply swaps every unit and checks that swapping twice gives the ROM bytes back and that the
    swap plan never touches a non-swap byte.

  python tools/assets/swapcheck.py baserom.us.z64 [--spec tools/assets/swap_spec.json]
        [--type "grid section"] [--unit 0x17618BC] [--apply] [--json OUT.json] [--plan KEY]

The output table gives, per type: units, bytes, % covered (swap+byte+rdp+opaque+pad+guess) and the
class split. --plan KEY prints the swap plan of one unit (runs of (offset, width, count) and the
transposed-plane swaps) in the form a C generator (port/t2swap.c) would consume.

Spec format (JSON): see the "doc" key of swap_spec.json; summary:
  rule := "@name"                                       a named rule from "rules"
        | {"op":"struct", "fields": FIELDS, "repeat": bool}
        | {"op":"cstruct", "name": C_STRUCT, "from": off, "to": off, "repeat": bool}
        | {"op":"array", "type": PRIM}                  fill the range with PRIM elements
        | {"op":"iset", "entries": [rule...], "each": rule, "cycle": [rule...]}
        | {"op":"index"}                                ISet header only (an index load unit)
        | {"op":"uset", "elem": rule|FIELDS, "transposed": bool}
        | {"op":"seq", "parts": [{"size": n|"rest"|"FIELDS", "rule": rule}...]}
        | {"op":"switch", "on": "size" | {"at": off, "type": PRIM}, "cases": {"v": rule}, "default": rule}
        | {"op":"byte"|"rdp"|"opaque"|"pad"|"unk", "note": str}
        | {"op":"graph", ...} (pointer graph: the audio control block) | {"op":"graphicset"}
  FIELDS := "type[count]:name ..." with type in u8 s8 char u16 s16 u32 s32 f32 u64 f64 ptr
            pad unk rdp16 opaque, optional trailing "?" (= guessed width), count = number,
            hex, or an expression over earlier fields ("@order*@npredictors*8").
"""
import argparse
import json
import os
import re
import struct
import sys
import time
from array import array

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import romfs  # noqa: E402

NONE, BYTE, SWAP, RDP, OPAQUE, PAD, GUESS, UNK = range(8)
CLASS_NAMES = ["none", "byte", "swap", "rdp", "opaque", "pad", "guess", "unk"]
COVERED = (BYTE, SWAP, RDP, OPAQUE, PAD, GUESS)

PRIM = {"u8": 1, "s8": 1, "char": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "f32": 4,
        "u64": 8, "f64": 8, "ptr": 4}
SIGNED = {"s8", "s16", "s32"}
FMT = {"u8": "B", "s8": "b", "char": "B", "u16": "H", "s16": "h", "u32": "I", "s32": "i",
       "f32": "f", "u64": "Q", "f64": "d", "ptr": "I"}


class SpecError(Exception):
    pass


# ------------------------------------------------------------------ C struct reader
C_TYPES = {"u8": "u8", "s8": "s8", "char": "char", "u16": "u16", "s16": "s16", "u32": "u32",
           "s32": "s32", "f32": "f32", "f64": "f64", "u64": "u64", "s64": "u64", "int": "s32",
           "ALMicroTime": "s32", "ALPan": "u8"}
C_COMPOUND = {"CVector3": [("f32", 3)], "CQuaternion": [("f32", 4)], "CMtxF": [("f32", 16)],
              "ADPCM_STATE": [("s16", 16)]}
_member = re.compile(r"/\*\s*(0x[0-9A-Fa-f]+|\d+)[^*]*\*/\s*(?:const\s+)?(?:struct\s+)?(\w+)\s*(\**)\s*(\w+)\s*((?:\[[^\]]+\])*)\s*;")


def read_cstructs(paths):
    """{name: (size, [(offset, prim, count, fieldname)])} from the offset-commented headers
    (include/structs/*.h use `/* 0xNN */ type name[n];` lines; tools/check_structs.py checks them).
    Pointers become ptr; u8 arrays named pad*/unk* become unk; unions take their first member."""
    out = {}
    for p in paths:
        src = open(p).read()
        for m in re.finditer(r"typedef struct\s*\w*\s*\{(.*?)\n\}\s*(?:__attribute__\(\([^)]*\)\)\s*)?(\w+)\s*;([^\n]*)", src, re.S):
            body, name, tail = m.group(1), m.group(2), m.group(3)
            fields = []
            depth = 0
            for line in body.split("\n"):
                if re.search(r"\bunion\b|\bstruct\s*\{", line):
                    depth += 1
                    if depth == 1:
                        um = re.search(r"/\*\s*(0x[0-9A-Fa-f]+)", line)
                        union_off = int(um.group(1), 16) if um else None
                        union_first = True
                    continue
                if depth and re.match(r"\s*\}", line):
                    depth -= 1
                    continue
                mm = _member.search(line)
                if depth and not mm:
                    mm2 = re.match(r"\s*(?:const\s+)?(\w+)\s*(\**)\s*(\w+)\s*((?:\[[^\]]+\])*)\s*;", line)
                    if mm2 and union_first and union_off is not None:
                        typ, star, fname, dims = mm2.groups()
                        fields.append((union_off, typ, star, fname, dims))
                        union_first = False
                    continue
                if not mm:
                    continue
                off = int(mm.group(1), 0)
                fields.append((off, mm.group(2), mm.group(3), mm.group(4), mm.group(5)))
            sm = re.search(r"size\s+(0x[0-9A-Fa-f]+|\d+)", tail)
            size = int(sm.group(1), 0) if sm else None
            out[name] = (size, fields)
    res = {}

    def resolve(name, seen=()):
        if name in res:
            return res[name]
        if name not in out or name in seen:
            raise SpecError("C struct %s not found" % name)
        size, raw = out[name]
        flist = []
        for off, typ, star, fname, dims in raw:
            n = 1
            for d in re.findall(r"\[([^\]]+)\]", dims or ""):
                n *= int(eval(d, {"__builtins__": {}}))
            if star:
                flist.append((off, "ptr", n, fname))
            elif typ in C_TYPES:
                t = C_TYPES[typ]
                if t in ("u8", "s8", "char") and re.match(r"_?pad", fname):
                    t = "pad"       # filler the decomp's code never reads: no swap needed
                elif t in ("u8", "s8", "char") and re.match(r"_?unk", fname):
                    t = "unk"       # read or not, its width is not known yet
                flist.append((off, t, n, fname))
            elif typ in C_COMPOUND:
                o = off
                for _ in range(n):
                    for t, c in C_COMPOUND[typ]:
                        flist.append((o, t, c, fname))
                        o += PRIM[t] * c
            else:
                sub_size, sub = resolve(typ, seen + (name,))
                for k in range(n):
                    for so, st, sc, sn in sub:
                        flist.append((off + k * sub_size + so, st, sc, fname + "." + sn))
        if size is None and flist:
            last = flist[-1]
            size = last[0] + PRIM.get(last[1], 1) * last[2]
            size = (size + 3) & ~3 if any(PRIM.get(f[1], 1) >= 4 for f in flist) else size
        res[name] = (size, flist)
        return res[name]

    for name in out:
        try:
            resolve(name)
        except (SpecError, SyntaxError, NameError, ValueError):
            pass
    return res


# ------------------------------------------------------------------ field strings
_tok = re.compile(r"^(\w+)(\?)?(?:\[([^\]]+)\])?(?::([\w.]+))?$")


def parse_fields(s):
    out = []
    for t in s.split():
        m = _tok.match(t)
        if not m:
            raise SpecError("bad field token %r" % t)
        typ, guess, cnt, name = m.groups()
        if typ not in PRIM and typ not in ("pad", "unk", "rdp16", "opaque", "byte"):
            raise SpecError("unknown field type %r in %r" % (typ, s))
        out.append((typ, bool(guess), cnt or "1", name))
    return out


def eval_count(expr, env):
    # Counts like "@order*@npredictors*8" come from the repo's own spec file; eval runs with no
    # builtins and only the already-read field values in scope (a dev tool, not untrusted input).
    expr = expr.replace("@", "")
    return int(eval(expr, {"__builtins__": {}}, env))


# ------------------------------------------------------------------ the interpreter
class Unit:
    def __init__(self, data, key, typ):
        self.data = data
        self.key, self.type = key, typ
        self.cls = bytearray(len(data))
        self.plan = []            # (offset, width, count)
        self.planes = []          # (offset, count, [(plane, width)...])  transposed tables
        self.errors = []
        self.warnings = []
        self.guesses = set()
        self.unknown = set()

    def mark(self, a, b, c, why=None):
        if a >= b:
            return
        if a < 0 or b > len(self.data):
            self.err("range 0x%X-0x%X outside the unit (0x%X)" % (a, b, len(self.data)))
            a, b = max(a, 0), min(b, len(self.data))
            if a >= b:
                return
        seg = self.cls[a:b]
        if seg.count(0) != b - a:
            self.err("overlap at 0x%X-0x%X (%s)" % (a, b, why or CLASS_NAMES[c]))
            return
        self.cls[a:b] = bytes([c]) * (b - a)
        if c == UNK and why:
            self.unknown.add(why)

    def err(self, msg):
        if len(self.errors) < 20:
            self.errors.append(msg)

    def u32(self, o):
        return struct.unpack_from(">I", self.data, o)[0]


class Spec:
    def __init__(self, path, root):
        self.path = path
        with open(path) as f:
            self.j = json.load(f)
        self.rules = self.j.get("rules", {})
        self.types = self.j["types"]
        hdrs = [os.path.join(root, p) for p in self.j.get("c_headers", [])]
        self.c = read_cstructs(hdrs)
        self._fields_cache = {}

    def rule(self, r):
        while isinstance(r, str):
            if r.startswith("@"):
                if r[1:] not in self.rules:
                    raise SpecError("no rule %s" % r)
                r = self.rules[r[1:]]
            else:
                r = {"op": "struct", "fields": r}
        return r

    def fields(self, s):
        if s not in self._fields_cache:
            self._fields_cache[s] = parse_fields(s)
        return self._fields_cache[s]

    # ---- size of a fixed rule (struct / cstruct), None if variable
    def fixed_size(self, r):
        r = self.rule(r)
        op = r["op"]
        if op == "struct":
            n = 0
            for typ, g, cnt, name in self.fields(r["fields"]):
                if "@" in cnt:
                    return None
                n += (PRIM.get(typ, 1) if typ != "rdp16" else 2) * int(cnt, 0)
            return n
        if op == "cstruct":
            size, _ = self.c_get(r["name"])
            return r.get("to", size) - r.get("from", 0)
        return None

    def c_get(self, name):
        if name not in self.c:
            raise SpecError("C struct %s not found in c_headers" % name)
        return self.c[name]

    # ---- apply a rule to u.data[a:b]
    def apply(self, r, u, a, b, ctx=""):
        r = self.rule(r)
        op = r["op"]
        fn = getattr(self, "op_" + op, None)
        if fn is None:
            raise SpecError("unknown op %s" % op)
        return fn(r, u, a, b, ctx)

    def op_byte(self, r, u, a, b, ctx):
        u.mark(a, b, BYTE)

    def op_rdp(self, r, u, a, b, ctx):
        u.mark(a, b, RDP)

    def op_opaque(self, r, u, a, b, ctx):
        u.mark(a, b, OPAQUE)

    def op_pad(self, r, u, a, b, ctx):
        u.mark(a, b, PAD)

    def op_unk(self, r, u, a, b, ctx):
        u.mark(a, b, UNK, ctx + ": " + r.get("note", "unknown"))

    def _field(self, u, o, typ, guess, n, name, ctx, end):
        if typ in PRIM:
            w = PRIM[typ]
            size = w * n
            if o + size > end:
                u.err("%s: field %s runs past the range" % (ctx, name))
                return end
            if w == 1:
                u.mark(o, o + size, GUESS if guess else BYTE, name)
            else:
                u.mark(o, o + size, GUESS if guess else SWAP, name)
                u.plan.append((o, w, n))
            if guess:
                u.guesses.add("%s.%s" % (ctx, name))
            return o + size
        if typ == "rdp16":
            u.mark(o, o + 2 * n, RDP, name)
            return o + 2 * n
        c = {"pad": PAD, "unk": UNK, "opaque": OPAQUE, "byte": BYTE}[typ]
        u.mark(o, o + n, c, "%s.%s" % (ctx, name or typ))
        return o + n

    def op_struct(self, r, u, a, b, ctx):
        flds = self.fields(r["fields"])
        size = self.fixed_size(r)
        reps = 1
        if r.get("repeat"):
            if not size:
                raise SpecError("%s: repeat needs a fixed size" % ctx)
            reps, rem = divmod(b - a, size)
            if rem:
                u.err("%s: 0x%X bytes is not a multiple of 0x%X" % (ctx, b - a, size))
        o = a
        for _ in range(reps):
            env = {}
            for typ, guess, cnt, name in flds:
                n = eval_count(cnt, env) if "@" in cnt else int(cnt, 0)
                if typ in PRIM and n == 1 and name and o + PRIM[typ] <= len(u.data):
                    env[name] = struct.unpack_from(">" + FMT[typ], u.data, o)[0]
                o = self._field(u, o, typ, guess, n, name, ctx, b)
        if not r.get("repeat") and o < b and r.get("tail"):
            self.apply(r["tail"], u, o, b, ctx + ".tail")
            o = b
        return o

    def op_cstruct(self, r, u, a, b, ctx):
        size, flds = self.c_get(r["name"])
        lo, hi = r.get("from", 0), r.get("to", size)
        rec = hi - lo
        reps = 1
        if r.get("repeat"):
            reps, rem = divmod(b - a, rec)
            if rem:
                u.err("%s: 0x%X bytes is not a multiple of %s (0x%X)" % (ctx, b - a, r["name"], rec))
        for k in range(reps):
            base = a + k * rec
            cur = lo
            for off, typ, n, name in flds:
                if off < lo or off >= hi:
                    continue
                if off > cur:
                    u.mark(base + cur - lo, base + off - lo, PAD, "%s gap" % r["name"])
                cur = self._field(u, base + off - lo, typ, False, n, name, ctx + "." + r["name"], base + rec) - base + lo
            if cur < hi:
                u.mark(base + cur - lo, base + rec, PAD, "%s tail" % r["name"])
        if not r.get("repeat") and a + rec < b:
            self.apply(r.get("tail", {"op": "pad"}), u, a + rec, b, ctx + ".tail")
        return a + rec * reps

    def op_array(self, r, u, a, b, ctx):
        typ = r["type"]
        w = PRIM[typ]
        n, rem = divmod(b - a, w)
        self._field(u, a, typ, r.get("guess", False), n, r.get("name", typ + "[]"), ctx, b)
        if rem:
            self.apply(r.get("tail", {"op": "pad"}), u, a + n * w, b, ctx + ".tail")

    def op_index(self, r, u, a, b, ctx):
        """An index unit: the ISet header only (count, count+1 offsets), padded to 8."""
        n = u.u32(a)
        if 8 + 4 * n > b - a:
            u.err("%s: index count %d does not fit 0x%X" % (ctx, n, b - a))
            u.mark(a, b, UNK, ctx)
            return
        u.mark(a, a + 8 + 4 * n, SWAP)
        u.plan.append((a, 4, 2 + n))
        u.mark(a + 8 + 4 * n, b, PAD)

    def op_iset(self, r, u, a, b, ctx):
        if b - a < 8:
            u.err("%s: ISet of 0x%X bytes" % (ctx, b - a))
            u.mark(a, b, UNK, ctx)
            return
        offs = romfs.parse_iset(u.data, a, b - a)
        if offs is None:
            u.err("%s: not an ISet at 0x%X" % (ctx, a))
            u.mark(a, b, UNK, ctx + " (bad ISet)")
            return
        n = len(offs) - 1
        u.mark(a, a + 8 + 4 * n, SWAP)
        u.plan.append((a, 4, 2 + n))
        u.mark(a + 8 + 4 * n, a + offs[0], PAD)
        if "count" in r and r["count"] != n:
            u.err("%s: ISet count %d, expected %d" % (ctx, n, r["count"]))
        entries, each, cycle = r.get("entries", []), r.get("each"), r.get("cycle")
        for i in range(n):
            ea, eb = a + offs[i], a + offs[i + 1]
            if i < len(entries):
                sub = entries[i]
            elif cycle:
                sub = cycle[i % len(cycle)]
            else:
                sub = each
            if sub is None:
                u.mark(ea, eb, UNK, "%s[%d]" % (ctx, i))
                continue
            if ea < eb:
                self.apply(sub, u, ea, eb, "%s[%d]" % (ctx, i))
        u.mark(a + offs[n], b, PAD)

    def op_uset(self, r, u, a, b, ctx):
        if b - a < 8:
            u.err("%s: USet of 0x%X bytes" % (ctx, b - a))
            u.mark(a, b, UNK, ctx)
            return
        es, n = struct.unpack_from(">II", u.data, a)
        if 8 + es * n > b - a:
            u.err("%s: USet %d x 0x%X does not fit 0x%X" % (ctx, n, es, b - a))
            u.mark(a, b, UNK, ctx + " (bad USet)")
            return
        u.mark(a, a + 8, SWAP)
        u.plan.append((a, 4, 2))
        d = a + 8
        elem = r["elem"]
        if isinstance(elem, str) and elem in PRIM:
            elem = {"op": "array", "type": elem}
        size = self.fixed_size(elem) if not (isinstance(elem, dict) and elem.get("op") == "array") else PRIM[elem["type"]]
        if n and size is not None and size != es and not r.get("elem_tail"):
            u.err("%s: element size 0x%X, spec says 0x%X" % (ctx, es, size))
        if r.get("transposed"):
            self._transposed(elem, u, d, es, n, ctx)
        elif isinstance(elem, dict) and elem.get("op") == "array":
            self.apply(elem, u, d, d + es * n, ctx)
        else:
            for i in range(n):
                end = self.apply(elem, u, d + i * es, d + (i + 1) * es, "%s[]" % ctx)
                if isinstance(end, int) and end < d + (i + 1) * es:
                    self.apply(r.get("elem_tail", {"op": "pad"}), u, end, d + (i + 1) * es, ctx + "[].tail")
        u.mark(d + es * n, b, PAD)

    def _layout(self, elem):
        """[(offset, prim-or-class, count)] of a fixed element (for transposed tables)."""
        elem = self.rule(elem)
        out = []
        if elem["op"] == "struct":
            o = 0
            for typ, guess, cnt, name in self.fields(elem["fields"]):
                n = int(cnt, 0)
                out.append((o, typ, n, guess, name))
                o += (PRIM.get(typ, 1) if typ != "rdp16" else 2) * n
        elif elem["op"] == "cstruct":
            size, flds = self.c_get(elem["name"])
            cur = 0
            for off, typ, n, name in flds:
                if off > cur:
                    out.append((cur, "pad", off - cur, False, "gap"))
                out.append((off, typ, n, False, name))
                cur = off + (PRIM.get(typ, 1)) * n
            if cur < size:
                out.append((cur, "pad", size - cur, False, "tail"))
        else:
            raise SpecError("transposed element must be a struct")
        return out

    def _transposed(self, elem, u, d, es, n, ctx):
        """{width, count} + width planes of count bytes: byte k of every record, then k+1...
        A w-byte field at record offset o is planes o..o+w-1; swapping it = reversing those planes."""
        swaps = []
        for off, typ, cnt, guess, name in self._layout(elem):
            if typ not in PRIM and typ != "rdp16":
                c = {"pad": PAD, "unk": UNK, "opaque": OPAQUE, "byte": BYTE}[typ]
                u.mark(d + off * n, d + min(off + cnt, es) * n, c, "%s.%s" % (ctx, name))
                continue
            w = PRIM.get(typ, 2)
            for k in range(cnt):
                p = off + k * w
                if p + w > es:
                    break
                if typ in PRIM:
                    c = GUESS if guess else (BYTE if w == 1 else SWAP)
                    if w > 1:
                        swaps.append((p, w))
                else:
                    c = RDP
                u.mark(d + p * n, d + (p + w) * n, c, "%s.%s" % (ctx, name))
                if guess:
                    u.guesses.add("%s.%s" % (ctx, name))
        if swaps:
            u.planes.append((d, n, swaps))

    def op_seq(self, r, u, a, b, ctx):
        o = a
        for k, part in enumerate(r["parts"]):
            size = part.get("size", "rest")
            if size == "rest":
                e = b
            elif isinstance(size, str) and size.startswith("@"):
                e = o + eval_count(size, {"u32_0": u.u32(a)})
            elif isinstance(size, str):
                e = o + int(size, 0)
            else:
                e = o + size
            e = min(e, b)
            res = self.apply(part["rule"], u, o, e, "%s.%d" % (ctx, k))
            o = res if (size == "auto" and isinstance(res, int)) else e
        if o < b:
            u.mark(o, b, PAD)

    def op_switch(self, r, u, a, b, ctx):
        on = r["on"]
        if on == "size":
            v = b - a
        else:
            at, typ = on["at"], on.get("type", "u32")
            if a + at + PRIM[typ] > b:
                v = None
            else:
                v = struct.unpack_from(">" + FMT[typ], u.data, a + at)[0]
        sub = None
        for k, rule in r["cases"].items():
            if v is not None and (k.startswith("<=") and v <= int(k[2:], 0) or not k.startswith("<=") and int(k, 0) == v):
                sub = rule
                break
        if sub is None:
            sub = r.get("default")
        if sub is None:
            u.err("%s: no case for %r" % (ctx, v))
            u.mark(a, b, UNK, ctx + " (switch)")
            return
        return self.apply(sub, u, a, b, "%s{%s}" % (ctx, v))

    # ---- graphicset: frame table + tiled blocks (TEXTURES.md, Hud_GetImageFrame)
    def op_graphicset(self, r, u, a, b, ctx):
        d = u.data
        n = struct.unpack_from(">h", d, a)[0]
        self._field(u, a, "s16", False, 1, "frameCount", ctx, b)
        self._field(u, a + 2, "u16", False, 1, "id", ctx, b)
        u.mark(a + 4, a + 8, PAD)
        for k in range(n):
            t = a + 8 + 8 * k
            off, opaque = struct.unpack_from(">IB", d, t)
            self._field(u, t, "u32", False, 1, "offset", ctx, b)
            u.mark(t + 4, t + 5, BYTE)
            u.mark(t + 5, t + 8, PAD)
            p = a + off
            cols, rows = struct.unpack_from(">HH", d, p)
            self._field(u, p, "u16", False, 4, "grid", ctx, b)
            p += 8
            for _ in range(cols * rows):
                bw, bh = struct.unpack_from(">II", d, p)
                self._field(u, p, "u32", False, 2, "blockSize", ctx, b)
                p += 8
                if opaque:
                    u.mark(p, p + bw * bh // 2, OPAQUE, "I4 texels")
                    p += bw * bh // 2
                else:
                    u.mark(p, p + bw * bh * 2, RDP, "RGBA16 texels")
                    u.mark(p + bw * bh * 2, p + bw * bh * 2 + bw * bh // 2, OPAQUE, "I4 mask")
                    p += bw * bh * 2 + bw * bh // 2
            # gaps between frames: alignment
        gaps = [i for i in range(a, b) if u.cls[i] == NONE]
        for i in gaps:
            u.cls[i] = PAD

    # ---- particle effects: block 0 is a pool of sub-records the templates (block 2) point into
    def op_particles(self, r, u, a, b, ctx):
        d = u.data
        offs = romfs.parse_iset(d, a, b - a)
        if offs is None or len(offs) != 4:
            u.err("%s: not an ISet of 3" % ctx)
            u.mark(a, b, UNK, ctx)
            return
        u.mark(a, a + 20, SWAP)
        u.plan.append((a, 4, 5))
        u.mark(a + 20, a + offs[0], PAD)
        p0, p1 = a + offs[0], a + offs[1]
        self.apply("@s32set", u, a + offs[1], a + offs[2], ctx + "[1]")
        self.apply({"op": "uset", "elem": r["template"]}, u, a + offs[2], a + offs[3], ctx + "[2]")
        u.mark(a + offs[3], b, PAD)
        es, n = struct.unpack_from(">II", d, a + offs[2])
        slots = r["slots"]          # [[template offset, record rule], ...]
        claims = {}
        for i in range(n):
            t = a + offs[2] + 8 + i * es
            for at, rule in slots:
                v = struct.unpack_from(">i", d, t + at)[0]
                claims.setdefault(v, []).append(rule)
        starts = sorted(claims) + [p1 - p0]
        for j, v in enumerate(starts[:-1]):
            rules = claims[v]
            sizes = [(self.fixed_size(x) or 0, x) for x in rules]
            size, rule = max(sizes, key=lambda z: z[0])
            end = min(p0 + v + size, p0 + starts[j + 1])
            if len(set(json.dumps(x, sort_keys=True) for x in rules)) > 1 and any(d[p0 + v:end]):
                u.warnings.append("%s: record at 0x%X has several types and non-zero data" % (ctx, v))
            if p0 + v + size > end:
                u.mark(p0 + v, end, UNK, ctx + "[0] truncated record")
                continue
            self.apply(rule, u, p0 + v, end, ctx + "[0]")
        i = p0
        while i < p1:
            if u.cls[i] == NONE:
                j = i
                while j < p1 and u.cls[j] == NONE:
                    j += 1
                u.mark(i, j, PAD if not any(d[i:j]) else UNK, ctx + "[0] unreferenced bytes")
                i = j
            else:
                i += 1

    # ---- pointer graph (the audio control block)
    def op_graph(self, r, u, a, b, ctx):
        structs = r["structs"]
        base = r["vaddr"]           # pointer value of unit byte 0
        seen = set()
        todo = [(r["root"], a)]
        while todo:
            sname, o = todo.pop()
            if (sname, o) in seen:
                continue
            seen.add((sname, o))
            if not (a <= o < b):
                u.err("%s: %s pointer outside the unit (0x%X)" % (ctx, sname, o))
                continue
            env = {}
            for tok in structs[sname].split():
                m = re.match(r"^(\w+)(?:\[([^\]]+)\])?(?::(\w+))?(?:>(\w+))?$", tok)
                typ, cnt, name, target = m.groups()
                n = eval_count(cnt, env) if cnt and not cnt.isdigit() and not cnt.startswith("0x") else int(cnt or "1", 0)
                if typ == "align":
                    o2 = (o + n - 1) & ~(n - 1)
                    u.mark(o, o2, PAD)
                    o = o2
                    continue
                if typ in PRIM and n == 1 and name:
                    env[name] = struct.unpack_from(">" + FMT[typ], u.data, o)[0]
                if target:
                    for k in range(n):
                        pv = u.u32(o + 4 * k)
                        if pv:
                            todo.append((target, pv - base + a))
                o = self._field(u, o, typ, False, n, name, ctx + "." + sname, b)
        rest = r.get("rest", {"op": "pad"})
        i = a
        while i < b:
            if u.cls[i] == NONE:
                j = i
                while j < b and u.cls[j] == NONE:
                    j += 1
                self.apply(rest, u, i, j, ctx + ".gap")
                i = j
            else:
                i += 1


# ------------------------------------------------------------------ swap application
def apply_plan(data, plan, planes):
    out = bytearray(data)
    for o, w, n in plan:
        seg = out[o:o + w * n]
        if w == 2:
            arr = array("H", bytes(seg))
        elif w == 4:
            arr = array("I", bytes(seg))
        else:
            arr = array("Q", bytes(seg))
        arr.byteswap()
        out[o:o + w * n] = arr.tobytes()
    for d, n, swaps in planes:
        for p, w in swaps:
            chunk = [bytes(out[d + (p + k) * n:d + (p + k + 1) * n]) for k in range(w)]
            for k in range(w):
                out[d + (p + k) * n:d + (p + k + 1) * n] = chunk[w - 1 - k]
    return bytes(out)


def unit_data(fs, n):
    return fs.blob(n) if n.compressed else fs.read(n)


def check(rom, spec_path, only_type=None, only_unit=None, do_apply=False, plan_key=None, verbose=False):
    root = os.path.dirname(os.path.dirname(HERE))
    spec = Spec(spec_path, root)
    fs = romfs.RomFS(rom)
    stats = {}
    errors = {}
    warnings = {}
    plan_stats = [0, 0]
    unknown = {}
    guesses = {}
    t0 = time.time()
    seen_types = set()
    for key, size, n in fs.load_units():
        typ = n.type
        if only_type and typ != only_type:
            continue
        if only_unit is not None and key != only_unit:
            continue
        seen_types.add(typ)
        data = unit_data(fs, n)
        if not n.compressed and len(data) != size:
            data = data[:size]
        u = Unit(data, key, typ)
        rule = spec.types.get(typ)
        if rule is None:
            u.mark(0, len(data), NONE)
            errors.setdefault(typ, []).append("no rule for type")
        else:
            try:
                spec.apply(rule, u, 0, len(data), typ)
            except (struct.error, IndexError) as e:
                u.err("parse error: %s" % e)
        if n.compressed:
            fs._blobs.pop(n.path, None)
        st = stats.setdefault(typ, [0, 0] + [0] * 8)
        st[0] += 1
        st[1] += len(data)
        for c in range(8):
            st[2 + c] += u.cls.count(c)
        if u.errors:
            errors.setdefault(typ, []).extend("0x%X: %s" % (key, e) for e in u.errors[:3])
        if u.warnings:
            warnings.setdefault(typ, []).extend("0x%X: %s" % (key, e) for e in u.warnings[:3])
        runs = merge_runs(u.plan)
        plan_stats[0] += len(runs)
        plan_stats[1] += len(u.planes)
        for w in u.unknown:
            unknown.setdefault(typ, set()).add(w)
        for g in u.guesses:
            guesses.setdefault(typ, set()).add(g)
        if do_apply:
            sw = apply_plan(data, u.plan, u.planes)
            back = apply_plan(sw, u.plan, u.planes)
            if back != data:
                errors.setdefault(typ, []).append("0x%X: swap twice != original" % key)
            diff = [i for i in range(0, len(data)) if sw[i] != data[i] and u.cls[i] not in (SWAP, GUESS)] if sw != data else []
            if diff:
                errors.setdefault(typ, []).append("0x%X: swap changed %d non-swap bytes (first 0x%X)" % (key, len(diff), diff[0]))
        if plan_key is not None and key == plan_key:
            print("plan for 0x%X (%s, 0x%X bytes):" % (key, typ, len(data)))
            for o, w, cnt in merge_runs(u.plan):
                print("  swap%d at 0x%X x %d" % (w * 8, o, cnt))
            for d, cnt, sw in u.planes:
                print("  transposed table at 0x%X, %d records: reverse planes %s" % (d, cnt, sw))
    return stats, errors, warnings, plan_stats, unknown, guesses, time.time() - t0


def merge_runs(plan):
    out = []
    for o, w, n in sorted(plan):
        if out and out[-1][1] == w and out[-1][0] + out[-1][1] * out[-1][2] == o:
            out[-1] = (out[-1][0], w, out[-1][2] + n)
        else:
            out.append((o, w, n))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("rom")
    ap.add_argument("--spec", default=os.path.join(HERE, "swap_spec.json"))
    ap.add_argument("--type")
    ap.add_argument("--unit", type=lambda s: int(s, 0))
    ap.add_argument("--apply", action="store_true", help="also swap every unit and check the plan")
    ap.add_argument("--plan", type=lambda s: int(s, 0), help="print the swap plan of the unit at this key")
    ap.add_argument("--json")
    ap.add_argument("-v", "--verbose", action="store_true")
    a = ap.parse_args(argv)
    stats, errors, warnings, plan_stats, unknown, guesses, dt = check(a.rom, a.spec, a.type, a.unit, a.apply, a.plan, a.verbose)
    tot = [0] * 10
    print("%-36s %6s %10s %8s  %s" % ("type", "units", "bytes", "covered", "  ".join("%6s" % c for c in CLASS_NAMES)))
    for typ, st in sorted(stats.items(), key=lambda kv: -kv[1][1]):
        cov = sum(st[2 + c] for c in COVERED)
        pct = 100.0 * cov / st[1] if st[1] else 100.0
        print("%-36s %6d %10d %7.2f%%  %s" % (typ[:36], st[0], st[1], pct,
                                              "  ".join("%5.1f%%" % (100.0 * st[2 + c] / st[1] if st[1] else 0) for c in range(8))))
        tot = [x + y for x, y in zip(tot, st)]
    cov = sum(tot[2 + c] for c in COVERED)
    print("%-36s %6d %10d %7.2f%%  %s" % ("TOTAL", tot[0], tot[1], 100.0 * cov / max(tot[1], 1),
                                          "  ".join("%5.1f%%" % (100.0 * tot[2 + c] / max(tot[1], 1)) for c in range(8))))
    print("swap bytes: %d (%.2f%%), %d units, %.1f s" % (tot[2 + SWAP], 100.0 * tot[2 + SWAP] / max(tot[1], 1), tot[0], dt))
    print("swap plan: %d runs (offset, width, count) + %d transposed tables" % tuple(plan_stats))
    nerr = sum(len(v) for v in errors.values())
    for typ, v in sorted(warnings.items()):
        print("WARNINGS %s (%d):" % (typ, len(v)))
        for e in v[:8]:
            print("   ", e)
    for typ, v in sorted(errors.items()):
        print("ERRORS %s (%d):" % (typ, len(v)))
        for e in v[:8]:
            print("   ", e)
    if a.verbose:
        for typ, v in sorted(unknown.items()):
            print("UNKNOWN %s: %s" % (typ, "; ".join(sorted(v))[:600]))
        for typ, v in sorted(guesses.items()):
            print("GUESSED %s: %s" % (typ, "; ".join(sorted(v))[:600]))
    if a.json:
        out = {"types": {t: dict(zip(["units", "bytes"] + CLASS_NAMES, st)) for t, st in stats.items()},
               "errors": errors, "warnings": warnings, "plan_runs": plan_stats[0], "plan_tables": plan_stats[1],
               "unknown": {k: sorted(v) for k, v in unknown.items()},
               "guessed": {k: sorted(v) for k, v in guesses.items()}, "seconds": round(dt, 1)}
        with open(a.json, "w") as f:
            json.dump(out, f, indent=1)
    return 1 if nerr else 0


if __name__ == "__main__":
    sys.exit(main())
