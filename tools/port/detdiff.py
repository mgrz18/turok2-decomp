#!/usr/bin/env python3
"""Align two determinism traces by game frame (gFrameCountGameplay) and print the first fields
that differ (round 10 y3; docs/port/FIDELITY.md section 5).

    python3 tools/port/detdiff.py N64.det PORT.log [--level 81] [--first N] [--skip FIELD,...]

N64.det comes from tools/port/n64trace.py (the N64's RDRAM), PORT.log from the port with
`T2_DET_LOG=1 T2_DET_GF=1 T2_DET_ACTORS=1` (or two port traces: Mac vs Linux). Only the frames of
the given level (the first run of it in each file) are compared. For each differing game frame it
prints the fields that differ (player: pos/rot/vel/hp/mode/btn/stick; rng; actors by kind and
index), up to --first frames.
"""
import argparse
import re
import struct
import sys

HEAD = re.compile(r"det: frame (\d+) gf (\d+) ticks (-?\d+) mode (-?\d+) level (-?\d+) rng (\w+) (\w+)(.*)")
PLAYER = re.compile(r" player (\w+) (\w+) (\w+) rot (\w+) vel (\w+) (\w+) (\w+) hp (-?\d+) mode (-?\d+) "
                    r"btn (\w+) stick (\w+) (\w+)")
ACTOR = re.compile(r"det:  (\w+) (\d+) pos (\w+) (\w+) (\w+) \(.*\) rot (\w+) hp (-?\d+) mode (-?\d+) act (-?\d+) "
                   r"anim (-?\d+) (\w+) fl (\w+)")
PRT = re.compile(r"det:  prt (\d+) (\d+) type (-?\d+) pos (\w+) (\w+) (\w+) vel (\w+) (\w+) (\w+) age (\w+)")
CAM = re.compile(r"det:  cam vp (\w+) (\w+) (\w+) roty (\w+) off (\w+) (\w+) (\w+) yoff (\w+) trem (\w+) (\w+) "
                 r"shk (\w+) (\w+) (\w+) amp (\w+) (\w+) (\w+) pos (\w+) (\w+) (\w+)")
CAMF = ["vpx", "vpy", "vpz", "roty", "offx", "offy", "offz", "yoff", "trema", "tremh", "shkx", "shky", "shkz",
        "ampx", "ampy", "ampz", "posx", "posy", "posz"]
PRTF = ["type", "x", "y", "z", "vx", "vy", "vz", "age"]
PF = ["x", "y", "z", "rot", "vx", "vy", "vz", "hp", "mode", "btn", "movex", "lookx"]
AF = ["x", "y", "z", "rot", "hp", "mode", "act", "anim", "aframe", "flags"]
FLOATS = set(CAMF) | {"x", "y", "z", "rot", "vx", "vy", "vz", "movex", "lookx", "aframe", "age"}


def load(path, level):
    """{gf: {'vi': n, 'ticks', 'rng', 'player': {...}, 'actors': {(kind, i): {...}}}} for the
    first run of `level`."""
    frames, cur, seen, done = {}, None, False, False
    for line in open(path, errors="replace"):
        m = HEAD.match(line)
        if m:
            vi, gf, ticks, mode, lvl = (int(m.group(k)) for k in range(1, 6))
            if lvl != level:
                if seen:
                    done = True
                cur = None
                continue
            if done:
                break
            seen = True
            cur = {"vi": vi, "ticks": ticks, "mode": mode, "rng": (m.group(6), m.group(7)), "player": None,
                   "actors": {}}
            p = PLAYER.search(m.group(8))
            if p:
                cur["player"] = dict(zip(PF, p.groups()))
            frames[gf] = cur
            continue
        c = CAM.match(line)
        if c and cur is not None:
            cur["actors"][("cam", 0)] = dict(zip(CAMF, c.groups()))
            continue
        q = PRT.match(line)
        if q and cur is not None:
            g = q.groups()
            cur["actors"][("prt%s" % g[0], int(g[1]))] = dict(zip(PRTF, g[2:]))
            continue
        a = ACTOR.match(line)
        if a and cur is not None:
            g = a.groups()
            cur["actors"][(g[0], int(g[1]))] = dict(zip(AF, g[2:]))
    return frames


def show(name, v):
    if name in FLOATS:
        try:
            return "%s(%g)" % (v, struct.unpack(">f", bytes.fromhex(v.zfill(8)))[0])
        except ValueError:
            return v
    return v


def diff(a, b, skip):
    out = []
    if "rng" not in skip and a["rng"] != b["rng"]:
        out.append("rng %s %s | %s %s" % (a["rng"] + b["rng"]))
    if "ticks" not in skip and a["ticks"] != b["ticks"]:
        out.append("ticks %d | %d" % (a["ticks"], b["ticks"]))
    pa, pb = a["player"], b["player"]
    if pa and pb:
        for k in PF:
            if k not in skip and pa[k] != pb[k]:
                out.append("player.%s %s | %s" % (k, show(k, pa[k]), show(k, pb[k])))
    for key in sorted(set(a["actors"]) | set(b["actors"])):
        x, y = a["actors"].get(key), b["actors"].get(key)
        if x is None or y is None:
            out.append("%s %d only in %s" % (key[0], key[1], "A" if y is None else "B"))
            continue
        for k in (CAMF if key[0] == "cam" else PRTF if key[0].startswith("prt") else AF):
            if k not in skip and x[k] != y[k]:
                out.append("%s %d.%s %s | %s" % (key[0], key[1], k, show(k, x[k]), show(k, y[k])))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("a")
    ap.add_argument("b")
    ap.add_argument("--level", type=int, default=81)
    ap.add_argument("--first", type=int, default=5, help="print this many differing frames")
    ap.add_argument("--skip", default="", help="fields to ignore (e.g. aframe,flags,dyn)")
    ap.add_argument("--max-lines", type=int, default=40)
    ap.add_argument("--shift", type=int, default=1,
                    help="B's gf for A's gf 0 (n64trace reads RDRAM before the frame's gf++, the port "
                         "logs after it: 1; two port traces: 0)")
    a = ap.parse_args()
    skip = set(filter(None, a.skip.split(",")))
    A, B = load(a.a, a.level), load(a.b, a.level)
    B = {gf - a.shift: v for gf, v in B.items()}
    common = sorted(set(A) & set(B))
    print("A: %d game frames (gf %s..%s), B: %d (gf %s..%s), common %d" % (
        len(A), min(A, default="-"), max(A, default="-"), len(B), min(B, default="-"), max(B, default="-"),
        len(common)))
    shown = 0
    for gf in common:
        d = diff(A[gf], B[gf], skip)
        for kind in ("dyn", "prt"):
            if kind in skip:
                d = [x for x in d if not x.startswith(kind)]
        if d:
            print("gf %d (A vi %d, B vi %d): %d fields differ" % (gf, A[gf]["vi"], B[gf]["vi"], len(d)))
            for x in d[:a.max_lines]:
                print("   " + x)
            shown += 1
            if shown >= a.first:
                break
    if not shown:
        print("no difference over the common game frames")
    return 0


if __name__ == "__main__":
    sys.exit(main())
