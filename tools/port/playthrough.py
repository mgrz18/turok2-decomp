#!/usr/bin/env python3
"""The Port of Adia playthrough script (docs/port/PLAYTHROUGH.md, round 9 x1).

Writes the --input file and the T2_TELEPORT list for one deterministic headless run from a new
game to the end of level 1 (the Harbor Totem and back to the hub). Each step is a teleport
next to a progression point (a pickup, a switch, a cage, a beacon, a warp region), facing it,
then C-up: the game's own logic does the rest (the pickup, the trigger, the level event, the
objective counters and messages, the warp). Steps are timed relative to the start of their
segment; a segment starts at an absolute frame (each one after the previous warp has loaded).

    python3 tools/port/playthrough.py OUTDIR [--fight WEAPON]
        # OUTDIR/playthrough.txt, OUTDIR/teleport.env

--fight WEAPON: also fight the Harbor Totem's defense: fire taps (Z every 15 frames) from the
totem's arrival, and teleport.env gets T2_GIVE_WEAPONS=1 T2_WEAPON=WEAPON and
T2_FACE_NEAREST=4000@109 (the aim assist only in level 109) and T2_FILL_AMMO=600 (the ammo
refilled every 600 frames: the plasma rifle's 150 cells run out at 4 kills of 8).
"""
import argparse, os

NEW_GAME = "START@4400 A@4500 A@4700 A@4800"

# (segment name, start frame, [(dt, x, y, z, rotY, walk frames), ...])
# x/y/z/rotY: T2_TELEPORT (rotY: the player walks along (-sin r, -cos r) in x/z);
# walk: C-up held for that many frames, starting 10 frames after the teleport.
SEGMENTS = [
    ("51 Lev1 A: power cell, beacon 1, exit 1098", 13100, [
        (0, 2210.0, 0.0, 0.0, -1.5708, 50),         # the power cell (2304, 0, 0)
        (200, 1540.0, -307.2, 0.0, -1.5708, 80),    # distress beacon 1 (actor 17), behind door 27
        (350, 3650.0, 307.2, 606.0, -1.5708, 100),  # the exit strip: warp 1098 -> level 52
    ]),
    ("52 Lev1 B: the cage switch, child 1, exit 1099", 13700, [
        (0, 2511.0, 1382.0, -1382.0, -1.5708, 300),  # switch actor 18 ("cage open."), on into the cage: child 20
        (350, -2974.3, 0.0, 630.4, 0.5642, 150),     # warp 1099 -> level 53
    ]),
    ("53 Lev1 C: 2 power cells, beacons 2 and 3, the cage switch, child 2, back 1814", 14400, [
        (0, -3922.3, 0.0, 4575.7, 2.1910, 40),       # power cell (-3989, 0, 4667)
        (100, 1493.4, 0.0, 7164.8, -0.0126, 60),     # power cell (1536, 0, 6973)
        (200, 295.1, 0.0, 8005.8, 0.1593, 60),       # distress beacon 2 (actor 56)
        (300, -2229.9, 614.4, 3997.8, -0.0102, 80),  # distress beacon 3 (actor 57): all beacons
        (450, 460.8, 0.0, -3.6, 0.0, 70),            # switch actor 44 -> cage 100 open
        (590, 586.0, 0.0, 229.0, 3.1416, 150),       # into the cage: child 60
        (800, -8399.9, 0.0, -4048.6, -1.8368, 120),  # warp 1814 -> level 52
    ]),
    ("52 Lev1 B again: exit 1005", 15450, [
        (0, 3407.8, 614.4, 408.7, 1.1436, 120),      # warp 1005 -> level 55
    ]),
    ("55 Lev1 E: the talisman portal switch, exit 1006", 15800, [
        (0, 3577.0, 307.2, -3840.0, -1.5708, 80),    # switch actor 14: "portal activated." (portal 21, warp 1001)
        (200, -2653.3, 614.4, 1325.2, 1.7847, 120),  # warp 1006 -> level 54
    ]),
    ("54 Lev1 D: the cage switch, child 3, exit 1007", 16250, [
        (0, -4092.0, 0.0, -1843.0, -1.5708, 80),     # switch actor 13 -> cage 36 open
        (150, -5176.0, 0.0, -1843.0, 1.5708, 200),   # into the cage: child 22
        (450, 1871.2, 632.4, 3939.6, 2.7053, 120),   # into the water: warp 1007 -> level 56
    ]),
    ("56 Lev1 F: exit 1008", 16950, [
        (0, -1577.1, 307.2, 3203.5, 2.7864, 100),    # warp 1008 -> level 57
    ]),
    ("57 Lev1 G: the cage switch, child 4, exit 1009", 17300, [
        (0, -2454.0, -594.0, -1126.0, -1.5708, 80),  # switch actor 25 -> cage 46 open
        (150, -2607.0, -922.0, 128.0, -1.5708, 200), # into the cage: child 35: all children
        (450, 4873.6, 0.0, 1268.8, -1.7236, 150),    # warp 1009 -> level 58
    ]),
    ("58 Lev1 H: exit 1010", 18050, [
        (0, 1771.5, 0.0, -189.9, -2.8969, 120),      # warp 1010 -> level 59
    ]),
    ("59 Lev1 I: the flesh portal switch, the level exit 7100", 18450, [
        (0, -3436.0, 307.2, 2246.0, 3.1416, 80),     # switch actor 52: "portal activated." (portal 74, warp 1003)
        (200, -9838.2, -307.2, 5051.3, 1.4793, 150), # warp 7100 -> the Harbor Totem (level 109)
    ]),
]


def build(segments):
    inputs = [NEW_GAME]
    tps = []
    for name, base, steps in segments:
        inputs.append("# " + name)
        for dt, x, y, z, ry, walk in steps:
            f = base + dt
            tps.append("%d:%.1f,%.1f,%.1f,%.4f" % (f, x, y, z, ry))
            if walk:
                inputs.append("CU@%d..%d" % (f + 10, f + 10 + walk))
    return inputs, tps


TOTEM_ARRIVAL = 18850   # level 109 is loaded (the warp 7100 at 18,650)
TOTEM_END = 26000       # the 120 s defense is over by then


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out", nargs="?", default=".")
    ap.add_argument("--fight", type=int, metavar="WEAPON", help="fight the totem defense with this weapon")
    a = ap.parse_args()
    inputs, tps = build(SEGMENTS)
    env = []
    if a.fight is not None:
        inputs.append("# 109 the Harbor Totem: fire taps (T2_FACE_NEAREST aims)")
        for f in range(TOTEM_ARRIVAL, TOTEM_END, 15):
            inputs.append("Z@%d..%d" % (f, f + 6))
        # T2_FILL_AMMO (round 10 y4): 150 plasma cells last to ~20,600, 4 kills; round 9's win
        # came from a host read past gImpactEventHandlers (fixed in CParticleSystem__Spawn)
        env += ["T2_GIVE_WEAPONS=1", "T2_WEAPON=%d" % a.fight, "T2_FACE_NEAREST=4000@109", "T2_FILL_AMMO=600"]
    with open(os.path.join(a.out, "playthrough.txt"), "w") as fp:
        fp.write("\n".join(inputs) + "\n")
    with open(os.path.join(a.out, "teleport.env"), "w") as fp:
        fp.write("T2_TELEPORT=\"%s\"\n" % ";".join(tps))
        for e in env:
            fp.write(e + "\n")
        fp.write("export T2_TELEPORT %s\n" % " ".join(e.split("=")[0] for e in env))
    print("%d inputs, %d teleports; last frame %d" % (len(inputs), len(tps), max(int(t.split(":")[0]) for t in tps)))


if __name__ == "__main__":
    main()
