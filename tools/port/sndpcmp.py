#!/usr/bin/env python3
"""Compare the sound player events of the port (T2_SNDP_LOG=2) with the N64's (n64trace.py --calls
alSndpSetSound,alSndpPlay,alSndpStop,alSndpSetVol,alSndpSetPan,alSndpSetPitch,alSndpSetFXMix), per game
frame: which sounds start, stop, and with what volume, pan, pitch and FX mix (round 20 i5, AUDIO.md 12).

    python3 tools/port/sndpcmp.py PORT.log N64.txt [--from GF] [--to GF] [--show N]

The port's lines come from every game frame of the run (the attract loop too): --from/--to select the
level's game frames (both sides restart gf at a level start; the N64 trace's first --from frame should
be in the level). Prints, per event kind, the counts on both sides and the game frames whose event sets
differ (the first N of them in full).
"""
import argparse
import collections
import re
import struct

KIND = {"alSndpPlay": "play", "alSndpStop": "stop", "alSndpSetVol": "vol", "alSndpSetPan": "pan",
        "alSndpSetPitch": "pitch", "alSndpSetFXMix": "fx"}


def port_events(path, lo, hi):
    ev = collections.defaultdict(list)
    seen_reset = False
    last = -1
    for line in open(path, errors="replace"):
        m = re.match(r"sndp: gf (\d+) (\w+) target (-?\d+)(?: (\S+))?", line)
        if not m:
            continue
        gf = int(m.group(1))
        if gf < last:
            seen_reset = True  # a new level: keep only what follows the last restart
            ev.clear()
        last = gf
        if lo <= gf <= hi:
            val = m.group(4)
            if m.group(2) == "pitch":
                val = "%.4f" % float(val)
            if val == "(not":
                val = None
            ev[gf].append((m.group(2), int(m.group(3)), val))
    return ev


def n64_events(path, lo, hi):
    ev = collections.defaultdict(list)
    target = -1
    for line in open(path, errors="replace"):
        m = re.match(r"call: frame \d+ gf (\d+) (\w+) ra \w+ a0-a3 (\w+) (\w+) (\w+) (\w+)", line)
        if not m:
            continue
        gf, sym, a1 = int(m.group(1)), m.group(2), int(m.group(4), 16)
        if sym == "alSndpSetSound":
            target = struct.unpack("<h", struct.pack("<H", a1 & 0xFFFF))[0]
            continue
        if not (lo <= gf <= hi) or sym not in KIND:
            continue
        k = KIND[sym]
        val = None
        if k == "vol":
            val = str(struct.unpack("<h", struct.pack("<H", a1 & 0xFFFF))[0])
        elif k in ("pan", "fx"):
            val = str(a1 & 0xFF)
        elif k == "pitch":
            val = "%.4f" % struct.unpack("<f", struct.pack("<I", a1))[0]
        ev[gf].append((k, target, val))
    return ev


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("port")
    ap.add_argument("n64")
    ap.add_argument("--from", dest="lo", type=int, default=1)
    ap.add_argument("--to", dest="hi", type=int, default=10 ** 9)
    ap.add_argument("--show", type=int, default=10)
    a = ap.parse_args()
    p, n = port_events(a.port, a.lo, a.hi), n64_events(a.n64, a.lo, a.hi)
    hi = min(max(p) if p else 0, max(n) if n else 0)
    cp, cn = collections.Counter(), collections.Counter()
    diff = []
    for gf in range(a.lo, hi + 1):
        sp, sn = collections.Counter(p.get(gf, [])), collections.Counter(n.get(gf, []))
        for e in sp.elements():
            cp[e[0]] += 1
        for e in sn.elements():
            cn[e[0]] += 1
        if sp != sn:
            diff.append((gf, sp - sn, sn - sp))
    print("game frames %d..%d" % (a.lo, hi))
    for k in ("play", "stop", "vol", "pan", "pitch", "fx"):
        print("  %-5s port %6d  n64 %6d" % (k, cp[k], cn[k]))
    print("  frames whose events differ: %d" % len(diff))
    for gf, only_p, only_n in diff[: a.show]:
        print("  gf %d: port only %s; n64 only %s" % (gf, sorted(only_p.elements()), sorted(only_n.elements())))


if __name__ == "__main__":
    main()
