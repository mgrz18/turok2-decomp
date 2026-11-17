#!/usr/bin/env python3
"""Compare the fights of whole-game runs (round 15 d3: the stick-aimed run against the assisted one).

    python3 tools/port/aimstats.py LOG [LOG2 ...]

From each run's log (T2_HURT_LOG, T2_PROGRESS_LOG, the totem lines): the damage taken and the
deaths per fight level, each totem's kills and time left when it was won (or lost), the frame each
totem and boss bit was set, and the lives lost overall.
"""
import re, sys

FIGHTS = [109, 133, 134, 135, 110, 64, 111, 106, 112, 1, 113, 2, 3, 4]
NAMES = {109: "Harbor Totem", 133: "sister 1", 134: "sister 2", 135: "sister 3", 110: "City Totem",
         64: "marsh 64", 111: "Marsh Totem", 106: "lair 106", 112: "Cave Totem", 1: "Blind One",
         113: "Hive Totem", 2: "Mantid Queen", 3: "Mother", 4: "Primagen"}


def scan(path):
    dmg, deaths, hits, minh = {}, {}, {}, {}
    totems, marks, lives = {}, [], 0
    last = None
    cur_totem = None
    level = None
    for line in open(path, errors="replace"):
        if line.startswith("hurtsum: level "):
            m = re.match(r"hurtsum: level (\d+) frames \S+ hits (\d+) damage (\d+) min (-?\d+) deaths (\d+)", line)
            if m:
                lv = int(m.group(1))
                hits[lv] = hits.get(lv, 0) + int(m.group(2))
                dmg[lv] = dmg.get(lv, 0) + int(m.group(3))
                minh[lv] = min(minh.get(lv, 999), int(m.group(4)))
                deaths[lv] = deaths.get(lv, 0) + int(m.group(5))
        elif line.startswith("death:") and "a life lost" in line:
            lives += 1
        elif line.startswith("totem: state"):
            m = re.match(r"totem: state (\d+) time (\d+)/\d+ s kills (\d+)/(\d+)", line)
            if m and level in (109, 110, 111, 112, 113):
                cur = totems.get(level)
                if cur is None or cur[0] == 0:
                    totems[level] = (int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)))
        elif line.startswith("progress:"):
            m = re.match(r"progress: frame (\d+) level (\d+) totems (\d+) bosses (\d+)", line)
            if m:
                level = int(m.group(2))
                key = (m.group(3), m.group(4))
                if key != last:
                    marks.append((int(m.group(1)), int(m.group(2)), m.group(3), m.group(4)))
                    last = key
        elif line.startswith("state:"):
            m = re.match(r"state: frame \d+ mode \d+ level (\d+)", line)
            if m:
                level = int(m.group(1))
    return dict(dmg=dmg, deaths=deaths, hits=hits, minh=minh, totems=totems, marks=marks, lives=lives)


def main():
    runs = [(p, scan(p)) for p in sys.argv[1:]]
    print("%-14s" % "fight" + "".join("  %-26s" % p[-26:] for p, _ in runs))
    for lv in FIGHTS:
        cells = []
        for _, r in runs:
            c = "dmg %d hits %d min %s d %d" % (r["dmg"].get(lv, 0), r["hits"].get(lv, 0), r["minh"].get(lv, "-"),
                                                r["deaths"].get(lv, 0))
            if lv in r["totems"]:
                st, t, k, n = r["totems"][lv]
                c += " | kills %d/%d %ds %s" % (k, n, t, "won" if st == 1 else "st%d" % st)
            cells.append(c)
        print("%-14s" % NAMES[lv] + "".join("  %-26s" % c for c in cells))
    for p, r in runs:
        print("%s: lives lost %d; totem/boss bits: %s" % (p, r["lives"], " ".join(
            "%d:%s/%s" % (f, t, b) for f, lv, t, b in r["marks"])))


if __name__ == "__main__":
    main()
