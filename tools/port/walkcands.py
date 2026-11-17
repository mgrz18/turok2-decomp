#!/usr/bin/env python3
"""Round 15 d5: the open-floor teleports of levels 1 and 2 as walk candidates.

    walkcands.py RUNLOG     # a default --all run's log with T2_DET_LOG=1 (teleport: lines + det rot)

For each teleport of levels 1-2 (before the City Totem) that stays on one floor (|dy| < 40) and
moves 100-4,000 units, prints its (segment, step) key for fullrun.L1_WALKS / L2_WALKS with where the
player stood and faced when it fired."""
import math, os, re, struct, sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tools", "port"))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fullrun as fr  # noqa: E402
import playthrough as pt  # noqa: E402

log = sys.argv[1]
tps = []
for l in open(log, errors="replace"):
    m = re.match(r"teleport: frame (\d+) player \(([-\d. ]+)\) -> \(([-\d. ]+)\)", l)
    if m and int(m.group(1)) < 77000:
        tps.append((int(m.group(1)), [float(v) for v in m.group(2).split()], [float(v) for v in m.group(3).split()]))
want = {f - 1 for f, _, _ in tps}
rot = {}
for l in open(log, errors="replace"):
    if l.startswith("det: frame") and " rot " in l:
        f = int(l.split()[2])
        if f in want:
            rot[f + 1] = struct.unpack(">f", bytes.fromhex(re.search(r"rot (\w+)", l).group(1)))[0]

l1 = [(i, j, st) for i, (_, _, steps) in enumerate(pt.SEGMENTS) for j, st in enumerate(steps)]
l2segs, _ = fr.l2_segments(True)
l2 = [(i, j, st) for i, (_, _, steps) in enumerate(l2segs) for j, st in enumerate(steps)]
used = set()
for f, a, b in tps:
    d = math.hypot(a[0] - b[0], a[2] - b[2])
    dy = b[1] - a[1]
    if not (abs(dy) < 40 and 100 < d < 4000):
        continue
    hit = None
    for lv, steps in ((1, l1), (2, l2)):
        for i, j, st in steps:
            if (lv, i, j) not in used and abs(st[1] - b[0]) < 0.06 and abs(st[3] - b[2]) < 0.06:
                hit = (lv, i, j)
                break
        if hit:
            break
    if not hit:
        print("    # %d: (%.1f, %.1f) not a route step (a key, health or hub pick), %.0f units" % (f, b[0], b[2], d))
        continue
    used.add(hit)
    print("    %d: (%d, %d): (%.1f, %.1f, %.4f),   # %d: %.0f units" % (hit[0], hit[1], hit[2], a[0], a[2], rot.get(f, 0.0), f, d))
