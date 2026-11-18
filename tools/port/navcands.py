#!/usr/bin/env python3
"""Round 16 e3: every teleport of a default --all run, classified for the waypoint walker.

    navcands.py ROM RUNLOG [--table] [--jumps]   # RUNLOG: a default --all run's log with T2_DET_LOG=1

For each "teleport:" line, the sub-level it fired in (the det line before it), where the player stood and
faced (the det line) and where it put him; then the walk the navigation mesh finds between the two
(tools/port/nav.py) and the route step it belongs to (levels 1-4: the segment lists fullrun.py plays;
a teleport matching no step is a pick: a key, health, a weapon, a hub key slot).
--table prints fullrun.NAV_WALKS's entries (the route steps with a path) instead. --jumps (round 19 h5):
the mesh with nav.py's jump edges (ledges, gaps); a path then counts its jumps. Round 19 h5 also names
the doors (nav.doors: level actors of type 0x641 and what opens them) within DOOR_NEAR of each path.
--level L: only the teleports of game level L (0 = the hub and the rest)."""
import collections, math, os, re, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fullrun as fr  # noqa: E402
import nav  # noqa: E402
import playthrough as pt  # noqa: E402


def steps_of(level):
    segs = {1: pt.SEGMENTS, 2: fr.l2_segments(True)[0], 3: fr.l3_segments(True), 4: pt.L4_SEGMENTS}.get(level, [])
    return [(i, j, int(name.split()[0]), st) for i, (name, _, steps) in enumerate(segs) for j, st in enumerate(steps)]


def game_level(sub):
    """The game level (1-6) of a sub-level (fullrun's ranges; 0 for the hub and the rest)."""
    for lv, subs in ((1, list(range(51, 60)) + [109]), (2, list(range(125, 136)) + [73, 110]),
                     (3, list(range(61, 72)) + [111]), (4, list(range(98, 109)) + [112]),
                     (5, list(range(84, 97)) + [124, 113]), (6, list(range(114, 124)) + [4])):
        if sub in subs:
            return lv
    return 0


def main():
    rom, log = sys.argv[1], sys.argv[2]
    table = "--table" in sys.argv
    jumps = "--jumps" in sys.argv
    only = int(sys.argv[sys.argv.index("--level") + 1]) if "--level" in sys.argv else None
    tps = []
    for l in open(log, errors="replace"):
        m = re.match(r"teleport: frame (\d+) player \(([-\d. ]+)\) -> \(([-\d. ]+)\)", l)
        if m:
            tps.append((int(m.group(1)), [float(v) for v in m.group(2).split()], [float(v) for v in m.group(3).split()]))
    want = {f - 1 for f, _, _ in tps}
    det = {}
    for l in open(log, errors="replace"):
        if l.startswith("det: frame") and " rot " in l:
            f = int(l.split()[2])
            if f in want:
                lvl = int(re.search(r" level (\d+)", l).group(1))
                ry = struct.unpack(">f", bytes.fromhex(re.search(r"rot (\w+)", l).group(1)))[0]
                det[f + 1] = (lvl, ry)
    used = set()
    count = collections.Counter()
    walkable = collections.Counter()
    for f, a, b in tps:
        sub, ry = det.get(f, (0, 0.0))
        glv = game_level(sub)
        if only is not None and glv != only:
            continue
        count[glv] += 1
        d = math.hypot(a[0] - b[0], a[2] - b[2])
        hit = None
        for i, j, s, st in steps_of(glv):
            if (glv, i, j) not in used and s == sub and abs(st[1] - b[0]) < 0.06 and abs(st[3] - b[2]) < 0.06:
                hit = (glv, i, j)
                break
        if hit:
            used.add(hit)
        try:
            res = nav.load(rom, sub, jumps).path(tuple(a), tuple(b)) if sub else None
        except Exception:
            res = None
        kind = "path %.0f (%d wp%s)" % (res[2], len(res[0]), ", %d jumps" % len(res[3]) if res[3] else "") if res else "no path"
        if res and hasattr(nav, "doors_on"):
            ds = nav.doors_on(rom, sub, res[0], a[1])
            if ds:
                kind += "; doors " + ", ".join("%d (%s)" % (d["actor"], nav.opener_text(d)) for d in ds)
        if res and res[2] < 8000:
            walkable[glv] += 1
        if table:
            if hit and res and res[2] < 8000:
                print("    %d: {(%d, %d): (%.1f, %.1f, %.4f, %.1f)},   # %d in %d: %.0f straight, %.0f by the path" % (
                    hit[0], hit[1], hit[2], a[0], a[2], ry, a[1], f, sub, d, res[2]))
            continue
        print("%6d L%d %3d %-9s (%7.1f %6.1f %7.1f) -> (%7.1f %6.1f %7.1f) %5.0f  %s" % (
            f, glv, sub, "step %d,%d" % hit[1:] if hit else "pick", a[0], a[1], a[2], b[0], b[1], b[2], d, kind))
    if not table:
        print("per level (teleports, with a path under 8,000):", {k: (count[k], walkable[k]) for k in sorted(count)})


if __name__ == "__main__":
    main()
