#!/usr/bin/env python3
"""Approach points for a scripted playthrough (docs/port/PLAYTHROUGH.md, round 9 x1).

For a level's warp regions (CRegionInfo.m_WarpID) or any target point, print a T2_TELEPORT
entry `x,y,z,rotY` on a floor region a few regions away, facing the target, so that walking
forward (C-up) enters the warp region the game's own way (CPlayer__Update -> CPlayer__Warp).

    python3 tools/port/approach.py LEVELS_DIR LEVEL [--warp ID] [--point x,y,z] [--steps N]

LEVELS_DIR is the output of tools/assets/levels.py (levels/NNN_*.json). The player walks
along -sin(rotY), -cos(rotY) in x/z (rotY 0 = -z, -pi/2 = +x).
"""
import argparse, glob, json, math


def centroid(regs, V):
    pts = [V[i] for r in regs for i in r["v"]]
    return [sum(p[k] for p in pts) / len(pts) for k in range(3)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("levels")
    ap.add_argument("level", type=int)
    ap.add_argument("--warp", type=int)
    ap.add_argument("--point")
    ap.add_argument("--steps", type=int, default=3, help="regions away from the target")
    ap.add_argument("--water", action="store_true", help="also walk through water regions")
    a = ap.parse_args()
    d = json.load(open(glob.glob("%s/levels/%03d_*.json" % (a.levels, a.level))[0]))
    c = d["collision"]
    V, R, I = c["vertices"], c["regions"], c["region_infos"]
    if a.warp is not None:
        infos = {i for i, ri in enumerate(I) if ri.get("warp_id") == a.warp}
        start = [k for k, r in enumerate(R) if r["info"] in infos]
    else:
        p = [float(x) for x in a.point.split(",")]
        best = min(range(len(R)), key=lambda k: sum((centroid([R[k]], V)[j] - p[j]) ** 2 for j in (0, 2)) + 0.25 * (centroid([R[k]], V)[1] - p[1]) ** 2)
        start = [best]
        infos = {R[best]["info"]}
    target = centroid([R[k] for k in start], V) if a.point is None else p
    # breadth-first walk away from the target over floor neighbours (no warp regions, no water)
    seen = set(start)
    ring = list(start)
    for _ in range(a.steps):
        nxt = []
        for k in ring:
            for n in R[k]["neighbours"]:
                if n is None or n in seen:
                    continue
                if a.warp is not None and R[n]["info"] in infos:
                    continue
                if I[R[n]["info"]]["flags"] & 1 and not a.water:
                    continue
                seen.add(n)
                nxt.append(n)
        if not nxt:
            break
        ring = nxt
    for k in ring[:4]:
        p = centroid([R[k]], V)
        dx, dz = target[0] - p[0], target[2] - p[2]
        ry = math.atan2(-dx, -dz)
        dist = math.hypot(dx, dz)
        print("%.1f,%.1f,%.1f,%.4f   # region %d, %.0f units from (%.1f %.1f %.1f)" % (p[0], p[1], p[2], ry, k, dist, *target))


if __name__ == "__main__":
    main()
