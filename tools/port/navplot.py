#!/usr/bin/env python3
"""Round 16 e3: draw one walk: the sub-level's floor (walkable grey, steep red, water blue), the static
objects' boxes (orange), the planned path (green) and the player's track from an aimdrive --stats file
(black; T2_MOVE_STATS_EVERY=10 or less).

    navplot.py ROM LEVEL STATS F1 F2 OUT.png [X,Y,Z X,Y,Z]   (needs matplotlib)"""
import os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import nav  # noqa: E402
import matplotlib  # noqa: E402
matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402
from matplotlib.patches import Polygon, Rectangle  # noqa: E402


def main():
    rom, level, stats, f1, f2, out = sys.argv[1], int(sys.argv[2]), sys.argv[3], int(sys.argv[4]), int(sys.argv[5]), sys.argv[6]
    n = nav.load(rom, level)
    track = []
    for l in open(stats):
        p = l.split()
        if p and p[0] == "walk" and f1 <= int(p[1]) <= f2:
            track.append((float(p[2]), float(p[4]), p[-1]))
    if not track:
        sys.exit("no walk lines in %d-%d" % (f1, f2))
    xs = [t[0] for t in track]
    zs = [t[1] for t in track]
    path = None
    if len(sys.argv) > 8:
        a = tuple(float(v) for v in sys.argv[7].split(","))
        b = tuple(float(v) for v in sys.argv[8].split(","))
        path = n.path(a, b)
        xs += [a[0], b[0]]
        zs += [a[2], b[2]]
    pad = 600
    x0, x1, z0, z1 = min(xs) - pad, max(xs) + pad, min(zs) - pad, max(zs) + pad
    fig, ax = plt.subplots(figsize=(10, 10))
    for i in range(len(n.r)):
        c = n.corners(i)
        if all(p[0] < x0 or p[0] > x1 for p in c) or all(p[2] < z0 or p[2] > z1 for p in c):
            continue
        col = "#dddddd" if n.walk[i] else "#ee8888"
        if n.water[i]:
            col = "#88aaee"
        ax.add_patch(Polygon([(p[0], p[2]) for p in c], closed=True, fc=col, ec="#999999", lw=0.3))
        for k, j in enumerate(n.r[i]["neighbours"]):
            if j is None:
                a, b = n.edge(i, k)
                ax.plot([a[0], b[0]], [a[2], b[2]], color="black", lw=1.0)
    for b in n.boxes:
        if b[1] < x0 or b[0] > x1 or b[3] < z0 or b[2] > z1:
            continue
        ax.add_patch(Rectangle((b[0], b[2]), b[1] - b[0], b[3] - b[2], fc="none", ec="orange", lw=0.8))
    if path:
        ax.plot([p[0] for p in path[0]], [p[1] for p in path[0]], color="green", lw=1.5, marker="o", ms=3)
    ax.plot([t[0] for t in track], [t[1] for t in track], color="black", lw=1.0)
    ax.plot(track[0][0], track[0][1], "bs")
    ax.plot(track[-1][0], track[-1][1], "kx")
    ax.set_xlim(x0, x1)
    ax.set_ylim(z0, z1)
    ax.set_aspect("equal")
    ax.set_title("level %d, frames %d-%d" % (level, f1, f2))
    fig.savefig(out, dpi=80)


if __name__ == "__main__":
    main()
