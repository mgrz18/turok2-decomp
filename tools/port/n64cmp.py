#!/usr/bin/env python3
"""The port against the N64, member by member (round 17 f3; docs/port/FIDELITY.md section 32-f3).

Both sides start the same warp from the attract loop with the same random seed and dump the same
state structs at the same game frames: the N64 with `n64trace.py --start-warp ID@N --poke
N:u32=gRandomValue:HEX --raw FILE:LEVEL:GFS` (its RDRAM, the N64 layout, big-endian), the port with
`T2_START_WARP=ID@F T2_RNG_SET=LEVEL:GF:HEX T2_RAW_DUMP=FILE:LEVEL:GFS` (the host layout). Each
record (gScene, the player, its camera, every level actor, every pooled instance) is decoded with
its struct's layout on that side, computed by libclang from include/ (mips-linux-gnu for the N64,
the port's own compile flags for the host), down to every scalar member; the two are compared by
member path. Pointers compare by what they point at (null, a dumped record and the member there,
or "elsewhere"). For each object the first differing member is reported, with a summary of the
members that differ most often (a host bug that stores at an N64 offset shows up as the same member
on every object).

    # run both sides and compare (the lair, game frames 2-40 of level 7602)
    python3 tools/port/n64cmp.py run --warp 7602 --level 7602 --gf 2-40 --work /tmp/cmp \\
        --port OUT/build/app/turok2 --archive OUT/turok2.o2r
    # or compare two dumps already made
    python3 tools/port/n64cmp.py cmp N64.raw PORT.raw [--gf 5] [--only LVL] [--all-fields]

Never commit the dumps.
"""
import argparse
import collections
import json
import os
import shlex
import struct
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
LIBCLANG = os.environ.get("LIBCLANG", "/usr/lib/llvm-19/lib/libclang-19.so.1")
HEADERS = ["common.h", "structs/particle.h", "structs/world.h", "structs/player.h", "structs/game.h",
           "structs/game_object.h", "structs/intelligence.h", "structs/collision.h", "structs/camera.h"]
TYPES = {"SCN ": "CScene", "PLR ": "CPlayer", "CAM ": "CCamera", "LVL ": "CGameObjectInstance",
         "DYN ": "CGameObjectInstance"}
N64_ARGS = ["--target=mips-linux-gnu", "-std=gnu89", "-fsigned-char", "-DF3DEX_GBI_2", "-D_LANGUAGE_C",
            '-DGAME_VERSION="us"', "-w"]


# ---------------------------------------------------------------- layouts (libclang)

def host_args(build):
    cc = json.load(open(os.path.join(build, "compile_commands.json")))
    for e in cc:
        if e["file"].endswith("port/game/t2det.c"):
            args = e.get("arguments") or shlex.split(e["command"])
            out, skip = [], False
            for a in args[1:]:
                if skip:
                    skip = False
                    continue
                if a in ("-o", "-c", "-MF", "-MT", "-MQ"):
                    skip = True
                    continue
                if a.endswith(".c") or a.startswith("-W") or a in ("-MD", "-MMD"):
                    continue
                out.append(a)
            return out
    sys.exit("n64cmp: no compile command for port/game/t2det.c in %s" % build)


class Leaf:
    __slots__ = ("path", "off", "size", "kind", "bit", "width")

    def __init__(self, path, off, size, kind, bit=0, width=0):
        self.path, self.off, self.size, self.kind, self.bit, self.width = path, off, size, kind, bit, width


def layouts(args, types, maxarr, all_union=False):
    """{type: [Leaf]} for one target: every scalar member path (arrays of scalars element by
    element up to maxarr, records and arrays of records recursively, union members all)."""
    import clang.cindex as ci
    if not ci.Config.loaded:
        ci.Config.set_library_file(LIBCLANG)
    src = "".join('#include "%s"\n' % h for h in HEADERS)
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as f:
        f.write(src)
        path = f.name
    try:
        tu = ci.Index.create().parse(path, args=args + ["-I" + os.path.join(ROOT, "include")])
    finally:
        os.unlink(path)
    K = ci.TypeKind
    recs = {}
    for c in tu.cursor.walk_preorder():
        if c.kind == ci.CursorKind.TYPEDEF_DECL and c.spelling in types and c.spelling not in recs:
            d = c.underlying_typedef_type.get_canonical().get_declaration()
            if d.is_definition():
                recs[c.spelling] = d

    def scalar(t):
        t = t.get_canonical()
        if t.kind == K.POINTER or t.kind == K.FUNCTIONPROTO:
            return "p"
        if t.kind in (K.FLOAT,):
            return "f"
        if t.kind in (K.DOUBLE,):
            return "d"
        if t.kind == K.ENUM:
            return "i"
        if t.kind in (K.CHAR_U, K.UCHAR, K.USHORT, K.UINT, K.ULONG, K.ULONGLONG, K.BOOL):
            return "u"
        if t.kind in (K.CHAR_S, K.SCHAR, K.SHORT, K.INT, K.LONG, K.LONGLONG):
            return "i"
        return None

    def anon_base(rec, anon):
        """The offset of an anonymous struct/union member of rec (libclang lists it as a
        STRUCT_DECL/UNION_DECL child, with no FIELD_DECL): through its first named field."""
        for g in anon.get_children():
            if g.kind == ci.CursorKind.FIELD_DECL and g.spelling:
                return (rec.type.get_offset(g.spelling) - anon.type.get_offset(g.spelling)) // 8
            if g.kind in (ci.CursorKind.STRUCT_DECL, ci.CursorKind.UNION_DECL) and g.is_anonymous():
                b = anon_base(anon, g)
                if b is not None:
                    inner = anon_base(rec, g)
                    return inner - b if inner is not None else None
        return None

    def walk(rec, prefix, base, out, depth):
        seen = False
        for f in rec.get_children():
            if rec.kind == ci.CursorKind.UNION_DECL and not all_union:
                # a union: only its first member (the other views read the same bytes in another
                # width, which differs between the byte orders by design)
                if f.kind not in (ci.CursorKind.FIELD_DECL, ci.CursorKind.STRUCT_DECL, ci.CursorKind.UNION_DECL):
                    continue
                if f.kind != ci.CursorKind.FIELD_DECL and not f.is_anonymous():
                    continue
                if seen:
                    continue
                seen = True
            if f.kind in (ci.CursorKind.STRUCT_DECL, ci.CursorKind.UNION_DECL) and f.is_anonymous():
                b = anon_base(rec, f)
                if b is not None and b >= 0:
                    walk(f, prefix, base + b, out, depth + 1)
                continue
            if f.kind != ci.CursorKind.FIELD_DECL:
                continue
            boff = f.get_field_offsetof()
            t = f.type.get_canonical()
            name = f.spelling
            if not name or f.is_anonymous():
                d = t.get_declaration()
                walk(d, prefix, base + boff // 8, out, depth + 1)
                continue
            p = prefix + name
            off = base + boff // 8
            if f.is_bitfield():
                st = t.get_size()
                o = (boff // (8 * st)) * st
                out.append(Leaf(p, base + o, st, "b" + ("u" if scalar(t) == "u" else "i"),
                                boff - o * 8, f.get_bitfield_width()))
                continue
            sk = scalar(t)
            if sk:
                out.append(Leaf(p, off, t.get_size(), sk))
            elif t.kind == K.RECORD and depth < 8:
                walk(t.get_declaration(), p + ".", off, out, depth + 1)
            elif t.kind == K.CONSTANTARRAY:
                et = t.element_type.get_canonical()
                es = et.get_size()
                n = t.element_count
                ek = scalar(et)
                if ek:
                    for k in range(min(n, maxarr)):
                        out.append(Leaf("%s[%d]" % (p, k), off + k * es, es, ek))
                elif et.kind == K.RECORD and depth < 8:
                    for k in range(n):
                        walk(et.get_declaration(), "%s[%d]." % (p, k), off + k * es, out, depth + 1)
                elif et.kind == K.CONSTANTARRAY:
                    ee = et.element_type.get_canonical()
                    ek2 = scalar(ee)
                    if ek2:
                        m = et.element_count
                        for k in range(min(n, maxarr)):
                            for j in range(min(m, maxarr)):
                                out.append(Leaf("%s[%d][%d]" % (p, k, j), off + k * es + j * ee.get_size(),
                                                ee.get_size(), ek2))
    out = {}
    for t in types:
        if t not in recs:
            sys.exit("n64cmp: no definition of %s" % t)
        lv = []
        walk(recs[t], "", 0, lv, 0)
        out[t] = (lv, recs[t].type.get_size())
    return out


# ---------------------------------------------------------------- dumps

def read_dump(path):
    """{gf: {(tag, idx): (addr, bytes, level)}}"""
    data = open(path, "rb").read()
    assert data[:4] == b"T2RW", "%s: not a T2_RAW_DUMP / --raw file" % path
    pos, out = 4, collections.defaultdict(dict)
    hs = struct.calcsize("<4siIiQI")
    while pos + hs <= len(data):
        tag, idx, gf, lv, addr, size = struct.unpack_from("<4siIiQI", data, pos)
        pos += hs
        out[gf][(tag.decode(), idx)] = (addr, data[pos:pos + size], lv)
        pos += size
    return out


def value(buf, leaf, be, psize):
    e = ">" if be else "<"
    if leaf.off + leaf.size > len(buf):
        return None
    if leaf.kind == "p":
        return ("p", struct.unpack_from(e + ("I" if psize == 4 else "Q"), buf, leaf.off)[0])
    if leaf.kind == "f":
        return struct.unpack_from(e + "f", buf, leaf.off)[0]
    if leaf.kind == "d":
        return struct.unpack_from(e + "d", buf, leaf.off)[0]
    fmt = {1: "b", 2: "h", 4: "i", 8: "q"}[leaf.size]
    if leaf.kind in ("u", "bu"):
        fmt = fmt.upper()
    v = struct.unpack_from(e + fmt, buf, leaf.off)[0]
    if leaf.kind[0] == "b":
        raw = struct.unpack_from(e + {1: "B", 2: "H", 4: "I", 8: "Q"}[leaf.size], buf, leaf.off)[0]
        bits = leaf.size * 8
        # clang numbers bit-fields from the most significant bit on big-endian targets
        sh = (bits - leaf.bit - leaf.width) if be else leaf.bit
        v = (raw >> sh) & ((1 << leaf.width) - 1)
        if leaf.kind == "bi" and v >> (leaf.width - 1):
            v -= 1 << leaf.width
    return v


class Side:
    def __init__(self, recs, lay, be, psize):
        self.recs, self.lay, self.be, self.psize = recs, lay, be, psize
        # address ranges of the dumped records, to name what a pointer points at
        self.ranges = sorted((a, a + len(b), k) for k, (a, b, _) in recs.items())

    def where(self, ptr):
        if ptr == 0:
            return "null"
        for (a, z, k) in self.ranges:
            if a <= ptr < z:
                lv, _ = self.lay[TYPES[k[0]]]
                d = ptr - a
                best = None
                for leaf in lv:
                    if leaf.off <= d < leaf.off + max(leaf.size, 1) and (best is None or leaf.size <= best.size):
                        best = leaf
                name = "%s%d" % (k[0].strip(), k[1])
                if d == 0:
                    return name
                return "%s.%s%s" % (name, best.path if best else "?", "" if best is None or best.off == d
                                    else "+%d" % (d - best.off))
        return "elsewhere"

    def fields(self, key):
        addr, buf, _ = self.recs[key]
        lv, _ = self.lay[TYPES[key[0]]]
        out = collections.OrderedDict()
        for leaf in lv:
            v = value(buf, leaf, self.be, self.psize)
            if isinstance(v, tuple):
                v = self.where(v[1])
            out[leaf.path] = v
        return out


def same(a, b, tol):
    if isinstance(a, float) and isinstance(b, float):
        if a == b or (a != a and b != b):
            return True
        return tol > 0 and abs(a - b) <= tol * max(1.0, abs(a), abs(b))
    if a == "elsewhere" or b == "elsewhere":
        return a != "null" and b != "null"  # a pointer outside the dumps: only null-ness compares
    return a == b


def fmt(v):
    if isinstance(v, float):
        return "%.6g" % v
    return str(v)


def compare(n64, port, a):
    """Print the differences per game frame; return the number of differing members."""
    total = 0
    summary = collections.Counter()
    first_seen = {}
    gfs = sorted(set(n64) & set(port))
    if a.gf:
        want = set()
        for it in a.gf.split(","):
            lo, _, hi = it.partition("-")
            want |= set(range(int(lo), int(hi or lo) + 1))
        gfs = [g for g in gfs if g in want]
    if not gfs:
        print("n64cmp: no common game frame (N64 %s..., port %s...)" % (sorted(n64)[:5], sorted(port)[:5]))
        return 0
    skip = [s for s in a.skip.split(",") if s]
    for gf in gfs:
        sn = Side(n64[gf], a.lay_n64, True, 4)
        sp = Side(port[gf], a.lay_host, False, 8)
        keys = sorted(set(sn.recs) | set(sp.recs), key=lambda k: (list(TYPES).index(k[0]), k[1]))
        lines = []
        for k in keys:
            if a.only and k[0].strip() not in a.only.split(","):
                continue
            if k not in sn.recs or k not in sp.recs:
                lines.append("  %s%d: only on the %s" % (k[0].strip(), k[1], "N64" if k in sn.recs else "port"))
                continue
            fn, fp = sn.fields(k), sp.fields(k)
            diffs = []
            for path, vn in fn.items():
                if path not in fp or any(path.startswith(s) or ("." + s) in path for s in skip):
                    continue
                vp = fp[path]
                if vn is None or vp is None or same(vn, vp, a.tol):
                    continue
                diffs.append((path, vn, vp))
            if not diffs:
                continue
            total += len(diffs)
            name = "%s%d" % (k[0].strip(), k[1])
            for (path, _, _) in diffs:
                summary[(k[0].strip(), path)] += 1
                first_seen.setdefault((k[0].strip(), path), (gf, name))
            show = diffs if a.all_fields else diffs[:a.per_object]
            desc = fp.get("header.m_nObjType", "")
            lines.append("  %s%s: %d member(s) differ%s" % (
                name, (" (type %s)" % desc) if desc != "" else "", len(diffs),
                "" if len(show) == len(diffs) else ", first %d" % len(show)))
            for (path, vn, vp) in show:
                lines.append("      %-48s N64 %-14s port %s" % (path, fmt(vn), fmt(vp)))
        if lines:
            print("gf %d:" % gf)
            print("\n".join(lines[:a.max_lines]))
            if len(lines) > a.max_lines:
                print("  ... %d more lines" % (len(lines) - a.max_lines))
        else:
            print("gf %d: the same" % gf)
    if summary:
        print("\nmembers that differ most often (kind, member: objects x frames, first at):")
        for (kp, n) in summary.most_common(a.summary):
            print("  %-4s %-52s %5d  gf %d %s" % (kp[0], kp[1], n, *first_seen[kp]))
    return total


# ---------------------------------------------------------------- running both sides

def run_both(a):
    os.makedirs(a.work, exist_ok=True)
    n64raw = os.path.join(a.work, "n64.raw")
    portraw = os.path.join(a.work, "port.raw")
    gfs = a.gf or "1-30"
    last = max(int(x.split("-")[-1]) for x in gfs.split(","))
    rng = a.rng
    if not a.skip_n64:
        cmd = ["xvfb-run", "-a", "-s", "-screen 0 1024x768x24", sys.executable,
               os.path.join(HERE, "n64trace.py"), "--rom", a.rom, "--out", os.path.join(a.work, "emu"),
               "--to", str(a.n64_at + a.n64_frames + 4 * last), "--from", str(a.n64_at),
               "--start-warp=%d@%d" % (a.warp, a.n64_at),
               "--raw", "%s:%d:%s" % (n64raw, a.level, gfs)]
        if rng:
            cmd += ["--rng-set", "%d:%d:%s" % (a.level, a.rng_gf, rng)]
        if a.pad:
            cmd += ["--pad", a.pad]
        if a.n64_poke:
            cmd += ["--poke", a.n64_poke]
        print("n64cmp: N64:", " ".join(shlex.quote(c) for c in cmd), file=sys.stderr)
        with open(os.path.join(a.work, "n64.log"), "w") as lg:
            subprocess.run(cmd, stdout=lg, stderr=subprocess.STDOUT, check=False, timeout=a.timeout)
    if not a.skip_port:
        env = dict(os.environ)
        for k in list(env):
            if k.startswith("T2_"):
                del env[k]
        env.update({"SDL_VIDEODRIVER": "offscreen", "T2_START_WARP": "%d@%d" % (a.warp, a.port_at),
                    "T2_RAW_DUMP": "%s:%d:%s" % (portraw, a.level, gfs)})
        if rng:
            env["T2_RNG_SET"] = "%d:%d:%s" % (a.level, a.rng_gf, rng)
        for kv in a.env:
            k, _, v = kv.partition("=")
            env[k] = v
        cmd = ["timeout", "-s", "KILL", str(a.timeout), os.path.abspath(a.port), "--archive",
               os.path.abspath(a.archive), "--rom", os.path.abspath(a.rom), "--game", "--pak-blank",
               "--frames", str(a.port_at + a.port_frames + 4 * last)]
        if a.input:
            cmd += ["--input", a.input]
        print("n64cmp: port:", " ".join(shlex.quote(c) for c in cmd), file=sys.stderr)
        with open(os.path.join(a.work, "port.log"), "w") as lg:
            subprocess.run(cmd, stdout=lg, stderr=subprocess.STDOUT, check=False,
                           cwd=os.path.dirname(os.path.abspath(a.archive)), env=env)
    return n64raw, portraw


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("cmp", "run"):
        p = sub.add_parser(name)
        if name == "cmp":
            p.add_argument("n64")
            p.add_argument("port")
        else:
            p.add_argument("--warp", type=int, required=True, help="the warp id (7602 the lair, -4 the Mother)")
            p.add_argument("--level", type=int, required=True, help="the level the warp leads to")
            p.add_argument("--work", required=True, help="a scratch dir for the dumps and logs")
            p.add_argument("--port", default=os.path.join(ROOT, "OUT/build/app/turok2"))
            p.add_argument("--archive", default=os.path.join(ROOT, "OUT/turok2.o2r"))
            p.add_argument("--rom", default=os.path.join(ROOT, "baserom.us.z64"))
            p.add_argument("--n64-at", type=int, default=600, help="emulator frame of --start-warp")
            p.add_argument("--port-at", type=int, default=1200, help="port frame of T2_START_WARP")
            p.add_argument("--n64-frames", type=int, default=600, help="emulator frames allowed for the load")
            p.add_argument("--port-frames", type=int, default=1200, help="port frames allowed for the load")
            p.add_argument("--rng", default="12345678", help="gRandomValue set on both sides ('' keeps each)")
            p.add_argument("--rng-gf", type=int, default=1, help="the level's game frame at which both sides set it")
            p.add_argument("--pad", default="", help="n64trace.py --pad (the N64's controller 1)")
            p.add_argument("--input", default="", help="the port's --input (the same buttons)")
            p.add_argument("--n64-poke", default="")
            p.add_argument("--env", action="append", default=[], help="K=V for the port (repeatable)")
            p.add_argument("--timeout", type=int, default=900)
            p.add_argument("--skip-n64", action="store_true", help="reuse WORK/n64.raw")
            p.add_argument("--skip-port", action="store_true", help="reuse WORK/port.raw")
        p.add_argument("--build", default=os.path.join(ROOT, "OUT/build"), help="for the host compile flags")
        p.add_argument("--gf", default="", help="game frames: GF[-GF2][,...]")
        p.add_argument("--only", default="", help="record kinds: SCN,PLR,CAM,LVL,DYN")
        p.add_argument("--skip", default="_pad", help="member names or path prefixes to ignore, comma-separated (default: the padding)")
        p.add_argument("--tol", type=float, default=0.0, help="relative float tolerance (0: exact)")
        p.add_argument("--per-object", type=int, default=6, help="members shown per object")
        p.add_argument("--all-fields", action="store_true")
        p.add_argument("--max-lines", type=int, default=400)
        p.add_argument("--summary", type=int, default=40)
        p.add_argument("--maxarr", type=int, default=64, help="scalar array elements compared")
        p.add_argument("--all-union", action="store_true", help="every member of a union, not only the first")
    a = ap.parse_args()
    if a.cmd == "run":
        n64raw, portraw = run_both(a)
    else:
        n64raw, portraw = a.n64, a.port
    types = sorted(set(TYPES.values()))
    a.lay_n64 = layouts(N64_ARGS, types, a.maxarr, a.all_union)
    a.lay_host = layouts(host_args(a.build), types, a.maxarr, a.all_union)
    for t in types:
        print("n64cmp: %-20s N64 %6d members, 0x%x bytes; host %6d members, 0x%x bytes" % (
            t, len(a.lay_n64[t][0]), a.lay_n64[t][1], len(a.lay_host[t][0]), a.lay_host[t][1]), file=sys.stderr)
    n64, port = read_dump(n64raw), read_dump(portraw)
    compare(n64, port, a)
    return 0


if __name__ == "__main__":
    sys.exit(main())
