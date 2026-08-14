#!/usr/bin/env python3
"""Compare the built ROM against the baserom and attribute differences to segments.

A byte-exact build is the goal, but the *diff* is useful long before that: every
region that reassembles identically confirms its segment mapping, and every
region that does not points straight at a wrong VRAM or a mis-symbolized
reference. It is a sharper discriminator than any heuristic, because it tests
the whole pipeline against ground truth.

Usage:
    python3 tools/romdiff.py
    python3 tools/romdiff.py --max-runs 40
"""

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BASE = ROOT / "baserom.us.z64"
BUILT = ROOT / "build" / "turok2.us.z64"
YAML = ROOT / "versions" / "turok2.us.yaml"


def load_map():
    """Flatten the splat yaml into sorted (start, end, kind, name) ROM ranges."""
    import yaml

    with YAML.open() as fh:
        doc = yaml.safe_load(fh)

    tops = []
    for seg in doc["segments"]:
        tops.append({"start": seg[0], "type": None} if isinstance(seg, list) else seg)
    tops.sort(key=lambda s: s["start"])

    flat = []
    for i, seg in enumerate(tops):
        seg_end = tops[i + 1]["start"] if i + 1 < len(tops) else seg["start"]
        subs = seg.get("subsegments") if isinstance(seg, dict) else None
        if not subs:
            flat.append((seg["start"], seg_end, seg.get("type"), seg.get("name", "?")))
            continue
        starts = [s[0] if isinstance(s, list) else s["start"] for s in subs]
        for j, sub in enumerate(subs):
            if not isinstance(sub, list):
                continue
            start = sub[0]
            kind = sub[1] if len(sub) > 1 else seg.get("type")
            name = sub[2] if len(sub) > 2 else f"{start:X}"
            end = starts[j + 1] if j + 1 < len(starts) else seg_end
            flat.append((start, end, kind, name))
    return sorted(flat)


def attribute(segments, off):
    for start, end, kind, name in segments:
        if start <= off < end:
            return name, kind
    return "?", "?"


def diff_runs(a: bytes, b: bytes):
    """Yield (start, end) of differing byte runs, merging gaps under 16 bytes."""
    n = min(len(a), len(b))
    runs = []
    i = 0
    while i < n:
        if a[i] != b[i]:
            start = i
            while i < n and a[i] != b[i]:
                i += 1
            if runs and start - runs[-1][1] < 16:
                runs[-1] = (runs[-1][0], i)
            else:
                runs.append((start, i))
        else:
            i += 1
    return runs


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--max-runs", type=int, default=25, help="cuantos runs listar")
    args = ap.parse_args()

    if not BUILT.exists():
        sys.exit(f"no existe {BUILT} — corre `make rom` primero")

    base, built = BASE.read_bytes(), BUILT.read_bytes()
    print(f"baserom : {len(base):,} bytes")
    print(f"built   : {len(built):,} bytes")
    if len(base) != len(built):
        print(f"  !! difieren en tamano por {abs(len(base)-len(built)):,} bytes")
    print()

    runs = diff_runs(base, built)
    if not runs:
        print("IDENTICOS — build byte-exact")
        return

    segments = load_map()
    total_bad = sum(e - s for s, e in runs)
    print(f"runs distintos : {len(runs):,}")
    print(f"bytes distintos: {total_bad:,} de {min(len(base),len(built)):,} "
          f"({100*total_bad/min(len(base),len(built)):.2f}%)")
    print()

    # Per-segment rollup: which segments are clean is the actionable signal.
    per_seg = {}
    for s, e in runs:
        name, kind = attribute(segments, s)
        acc = per_seg.setdefault(name, {"kind": kind, "bytes": 0, "runs": 0})
        acc["bytes"] += e - s
        acc["runs"] += 1

    print(f"{'segmento':<26}{'tipo':<8}{'runs':>7}{'bytes malos':>14}{'del seg':>10}")
    print("-" * 66)
    seg_size = {n: (e - s) for s, e, k, n in segments}
    for name, acc in sorted(per_seg.items(), key=lambda kv: -kv[1]["bytes"]):
        size = seg_size.get(name, 0)
        frac = f"{100*acc['bytes']/size:.1f}%" if size else "-"
        print(f"{name:<26}{acc['kind'] or '-':<8}{acc['runs']:>7}"
              f"{acc['bytes']:>14,}{frac:>10}")

    clean = [n for s, e, k, n in segments if n not in per_seg and (e - s) > 0]
    if clean:
        print()
        print(f"segmentos byte-exact ({len(clean)}): {', '.join(clean[:14])}"
              f"{' ...' if len(clean) > 14 else ''}")

    print()
    print(f"primeros {min(args.max_runs, len(runs))} runs:")
    for s, e in runs[:args.max_runs]:
        name, _ = attribute(segments, s)
        print(f"  ROM 0x{s:07X}-0x{e:07X}  {e-s:>7,} bytes  {name}")
        if e - s <= 16:
            print(f"      base : {base[s:e].hex()}")
            print(f"      built: {built[s:e].hex()}")


if __name__ == "__main__":
    main()
