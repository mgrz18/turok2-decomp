#!/usr/bin/env python3
"""Where to stand to walk into a pickup (docs/port/PLAYTHROUGH.md section 14, round 13 b1).

    python3 tools/port/itemreach.py LEVELS_DIR LEVEL x,y,z [--d 90]

LEVELS_DIR is tools/assets/levels.py's output. Prints the floor under the item and the approach
angles `ang` (fullrun.Route.pick: the player stands at item + d (sin ang, cos ang) on that floor
and walks along -sin, -cos into the item) whose samples on the way in (a quarter, a half, three
quarters and all of d) have a floor within 25 units of the item's floor (no step, no pit; walls
are not checked).
"""
import argparse, glob, json, math


def load(levels, level):
    d = json.load(open(glob.glob("%s/levels/%03d_*.json" % (levels, level))[0]))
    c = d["collision"]
    return c["vertices"], c["regions"]


def floors(V, R, x, z):
    """The heights of every region triangle over (x, z)."""
    out = []
    for r in R:
        a, b, c = (V[i] for i in r["v"])
        d = (b[2] - c[2]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[2] - c[2])
        if abs(d) < 1e-6:
            continue
        l1 = ((b[2] - c[2]) * (x - c[0]) + (c[0] - b[0]) * (z - c[2])) / d
        l2 = ((c[2] - a[2]) * (x - c[0]) + (a[0] - c[0]) * (z - c[2])) / d
        l3 = 1 - l1 - l2
        if min(l1, l2, l3) < -1e-4:
            continue
        out.append(l1 * a[1] + l2 * b[1] + l3 * c[1])
    return out


def floor_at(V, R, x, y, z, up=40.0):
    """The highest floor at (x, z) no higher than y + up."""
    f = [h for h in floors(V, R, x, z) if h <= y + up]
    return max(f) if f else None


def reach(V, R, x, y, z, d=90.0):
    f0 = floor_at(V, R, x, y, z)
    if f0 is None:
        return None, []
    good = []
    for k in range(16):
        a = k * math.pi / 8 - math.pi
        ok = True
        for s in (0.25, 0.5, 0.75, 1.0):
            h = floor_at(V, R, x + math.sin(a) * d * s, f0, z + math.cos(a) * d * s)
            if h is None or abs(h - f0) > 25:
                ok = False
                break
        if ok:
            good.append(round(a, 4))
    return f0, good


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("levels")
    ap.add_argument("level", type=int)
    ap.add_argument("point")
    ap.add_argument("--d", type=float, default=90.0)
    a = ap.parse_args()
    V, R = load(a.levels, a.level)
    x, y, z = (float(v) for v in a.point.split(","))
    f0, good = reach(V, R, x, y, z, a.d)
    print("floor %s; approach angles %s" % ("none" if f0 is None else "%.1f" % f0, good))
