#!/usr/bin/env python3
"""benchcsv.py - frame-time report from `--bench` + T2_BENCH_CSV=<file> (round 17, f4).

    benchcsv.py BENCH.csv [--det OUT.log] [--top 15] [--paced-ms 16.667]

Per VI the CSV has the host times (game CPU, audio, draw, total). The report gives:
  - percentiles and a histogram of the total per VI, and of the time per drawn frame
    (the sum over the VIs since the previous drawn frame), both against the real-time budgets
    (16.7 ms a VI, 33.3 ms a 30 fps frame);
  - with --det (a T2_DET_LOG=1 run's stdout: `det: frame F ... mode M level L ...`): the same per
    level (the level the VI belongs to), the levels sorted by their p99;
  - the worst VIs with their level and what dominated (game / audio / draw).
"""
import argparse
import bisect
import csv


def pct(v, p):
    if not v:
        return 0.0
    v = sorted(v)
    return v[min(len(v) - 1, int(p * (len(v) - 1) + 0.5))]


def levels_from_det(path):
    vis, lv = [], []
    last = None
    with open(path, errors="replace") as f:
        for line in f:
            if not line.startswith("det: frame") or " level " not in line:
                continue
            t = line.split()
            fr = int(t[2])
            lvl = int(t[t.index("level") + 1])
            if lvl != last:
                vis.append(fr)
                lv.append(lvl)
                last = lvl
    return vis, lv


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csv")
    ap.add_argument("--det")
    ap.add_argument("--top", type=int, default=15)
    args = ap.parse_args()
    rows = []
    with open(args.csv) as f:
        for r in csv.DictReader(f):
            rows.append((int(r["vi"]), float(r["game_ms"]), float(r["audio_ms"]), float(r["draw_ms"]),
                         float(r["total_ms"]), int(r["drawn"])))
    lvis, lvl = levels_from_det(args.det) if args.det else ([], [])

    def level_of(vi):
        i = bisect.bisect_right(lvis, vi) - 1
        return lvl[i] if i >= 0 else -1

    tot = [r[4] for r in rows]
    frames, acc, perlevel, perlevel_fr = [], 0.0, {}, {}
    for r in rows:
        acc += r[4]
        L = level_of(r[0])
        perlevel.setdefault(L, []).append(r[4])
        if r[5]:
            frames.append(acc)
            perlevel_fr.setdefault(L, []).append(acc)
            acc = 0.0
    edges = [0.5, 1, 2, 4, 8, 16.667, 33.333]

    def hist(v):
        h = [0] * (len(edges) + 1)
        for x in v:
            h[bisect.bisect_right(edges, x)] += 1
        names = ["<0.5", "0.5-1", "1-2", "2-4", "4-8", "8-16.7", "16.7-33.3", ">=33.3"]
        return "  ".join(f"{n} {c}" for n, c in zip(names, h))

    print(f"VIs {len(rows)}: mean {sum(tot) / len(tot):.3f} ms  p50 {pct(tot, .5):.3f}  p99 {pct(tot, .99):.3f}  "
          f"p99.9 {pct(tot, .999):.3f}  max {max(tot):.2f}  over 16.7 ms: {sum(x >= 16.667 for x in tot)}")
    print("  per VI:    " + hist(tot))
    if frames:
        print(f"drawn frames {len(frames)}: host ms per frame mean {sum(frames) / len(frames):.3f}  p99 "
              f"{pct(frames, .99):.3f}  max {max(frames):.2f}  over 33.3 ms: {sum(x >= 33.333 for x in frames)}")
        print("  per frame: " + hist(frames))
    split = [sum(r[i] for r in rows) / len(rows) for i in (1, 2, 3)]
    print(f"mean split per VI: game {split[0]:.3f}  audio {split[1]:.3f}  draw {split[2]:.3f} ms")
    if lvis:
        print("per level (by p99 of the host ms per VI):")
        for L, v in sorted(perlevel.items(), key=lambda kv: -pct(kv[1], .99)):
            fr = perlevel_fr.get(L, [])
            print(f"  level {L:4d}: VIs {len(v):6d}  mean {sum(v) / len(v):.3f}  p99 {pct(v, .99):.3f}  max {max(v):7.2f}"
                  f"   frames {len(fr):6d} mean {sum(fr) / max(1, len(fr)):.3f} p99 {pct(fr, .99):.3f}")
    print(f"the {args.top} worst VIs:")
    for r in sorted(rows, key=lambda r: -r[4])[:args.top]:
        part = max((("game", r[1]), ("audio", r[2]), ("draw", r[3])), key=lambda t: t[1])
        print(f"  VI {r[0]:7d}  level {level_of(r[0]):4d}  total {r[4]:7.2f}  game {r[1]:7.2f}  audio {r[2]:6.2f}  "
              f"draw {r[3]:7.2f}  ({part[0]})")


if __name__ == "__main__":
    main()
