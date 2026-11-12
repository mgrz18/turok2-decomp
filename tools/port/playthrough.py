#!/usr/bin/env python3
"""The Port of Adia playthrough script (docs/port/PLAYTHROUGH.md, round 9 x1).

Writes the --input file and the T2_TELEPORT list for one deterministic headless run from a new
game to the end of level 1 (the Harbor Totem and back to the hub). Each step is a teleport
next to a progression point (a pickup, a switch, a cage, a beacon, a warp region), facing it,
then C-up: the game's own logic does the rest (the pickup, the trigger, the level event, the
objective counters and messages, the warp). Steps are timed relative to the start of their
segment; a segment starts at an absolute frame (each one after the previous warp has loaded).

    python3 tools/port/playthrough.py OUTDIR [--fight WEAPON] [--level 1|2]
        # OUTDIR/playthrough.txt, OUTDIR/teleport.env

--fight WEAPON: also fight the Harbor Totem's defense: fire taps (Z every 15 frames) from the
totem's arrival, and teleport.env gets T2_GIVE_WEAPONS=1 T2_WEAPON=WEAPON and
T2_FACE_NEAREST=4000@109 (the aim assist only in level 109).

--level 2 (round 10 y1): the River of Souls instead, from the hub (T2_WARP_TO=1999 on the new
game's first level) through the hub portal 2000 and its intro cinemas to level 125, then the
sub-levels 125 -> 126 -> 127 -> 128 -> 129 (soul gate 1) -> 130 -> 133 (sister of despair 1) ->
130 -> 131 -> 134 (sister 2) -> 131 -> 132 (soul gate 2) -> 135 (the graveyard) -> 132 and the
level exit 7200. Fights: T2_AIM_AT (a soul gate) or T2_AIM_ENEMY (the nearest live actor) windows
with fire taps, the plasma rifle, T2_CHEATS=1 and T2_FILL_AMMO; a "!" teleport also fires the
region's enter triggers (the enemy wake-ups a walk-in would fire). docs/port/PLAYTHROUGH.md
section 6.
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


# ---- level 2: the River of Souls (round 10 y1) ----
HUB_ARRIVAL = 13100      # T2_WARP_TO=1999 at 12,955 (60 frames into level 51): the hub (level 60)
L2_START = 18800         # level 125, after the portal 2000 (hub +200) and the River of Souls intro cinemas
FIRE_EVERY = 15

# (segment name, start frame relative to L2_START, [(dt, x, y, z, rotY, walk, opts), ...])
# opts: "trig" (a "!" teleport: fire the region's enter triggers), "fire": frames of fire taps from
# the teleport, "aim": (x, y, z) (T2_AIM_AT) or "enemy" (T2_AIM_ENEMY) during the fire window.
L2_SEGMENTS = [
    ("125 Lev2 A: exit 2500", 0, [
        (0, 4047.4, 921.6, 8200.3, 0.9998, 150, {}),          # warp 2500 -> 126
    ]),
    ("126 Lev2 B: exit 2601", 400, [
        (0, -4711.5, 409.6, -263.3, -1.4035, 150, {}),        # warp 2601 -> 127
    ]),
    ("127 Lev2 C: exit 2100", 800, [
        (0, -3885.5, -614.4, -367.2, 1.7717, 150, {}),        # warp 2100 -> 128
    ]),
    ("128 Lev2 D: exit 2101", 1200, [
        (0, 1523.8, 786.7, 5243.2, -1.2596, 150, {}),         # warp 2101 -> 129
    ]),
    ("129 Lev2 E: soul gate 1, exit 2102", 1600, [
        # on the gate's platform (y -259), facing it: soul gate actor 64 (destructible 649) at (-4147, -256, 5094)
        (0, -4155.0, -259.0, 5541.0, 0.0, 0, {"fire": 1150, "aim": (-4147.0, -150.0, 5094.0)}),
        (1250, -6778.7, 460.6, -452.6, -1.9684, 150, {}),     # warp 2102 -> 130
    ]),
    ("130 Lev2 F: exit 2614 (the first sister)", 3250, [
        (0, -2654.0, -4761.6, 6693.4, 2.1184, 150, {}),       # warp 2614 -> 133
    ]),
    ("133 Lev2 I: the zombies, door 28, sister 1, exit 2615", 3650, [
        (0, -1504.0, -4761.6, -972.0, -1.5708, 0, {"trig": 1, "fire": 1300, "aim": "enemy"}),   # region info 4: zombies 9, 16, 17 -> door 27
        (1400, -847.0, -4761.6, 1574.0, 3.1416, 0, {"trig": 1, "fire": 6500, "aim": "enemy"}),  # region info 7: zombies 22-24 -> door 28 -> sister 20
        (8000, 827.5, -4761.6, -1627.5, -2.8692, 150, {}),    # warp 2615 -> 130
    ]),
    ("130 Lev2 F again: exit 2103", 12050, [
        (0, 6063.6, -3532.6, -1300.2, -0.0653, 150, {}),      # warp 2103 -> 131
    ]),
    ("131 Lev2 G: exit 2612 (the second sister)", 12450, [
        (0, -4131.2, -51.2, 7062.5, -2.6877, 150, {}),        # warp 2612 -> 134
    ]),
    ("134 Lev2 J: the graveyard, zombies 2-4, gate 8, sister 2, exit 2613", 12850, [
        (100, 78.7, 1024.0, -870.5, -1.5708, 0, {"fire": 2400, "aim": "enemy"}),     # zombies 2, 3 come to the start
        (2600, 1650.0, 1024.0, -870.0, -1.5708, 0, {"fire": 6000, "aim": "enemy"}),  # at the gate: zombie 4, gate 8 opens, sister 1
        (8700, 2278.9, 1024.0, -809.8, -1.3620, 150, {}),     # warp 2613 -> 131
    ]),
    ("131 Lev2 G again: exit 2104", 21950, [
        (0, 3106.0, 716.8, -3375.1, 0.1319, 150, {}),         # warp 2104 -> 132
    ]),
    ("132 Lev2 H: soul gate 2, exit 2501", 22350, [
        # on the gate's platform (y 1225), facing it: soul gate actor 50 (destructible 649) at (11878, 1229, -1485)
        (0, 12171.4, 1225.4, -1240.4, 1.5708, 0, {"fire": 1150, "aim": (11878.0, 1330.0, -1485.0)}),
        (1250, 924.0, 716.8, -1267.0, 0.6143, 150, {}),       # warp 2501 -> 135
    ]),
    ("135 Lev2 K: the big graveyard (sister 3), exit 2608", 23950, [
        (100, 141.7, 0.0, -5650.5, 3.1416, 0, {"trig": 1}),   # region info 8: door 59 opens
        (200, -37.6, 0.0, -4953.9, 3.1416, 0, {"trig": 1, "fire": 1500, "aim": "enemy"}),  # info 9: "graveyard located.", zombies
        (1800, 1041.0, 0.2, -872.3, 3.0697, 100, {}),         # switch 53 (the crypts' switches, key 9)
        (2000, 2753.6, 0.2, -2241.3, -0.3950, 100, {}),       # switch 54
        (2200, -2855.1, 0.2, -1447.4, 2.7377, 100, {}),       # switch 55: "graveyard gate open." (gate 62)
        (2400, 2602.5, 0.2, 517.9, -0.8506, 120, {}),         # switch 48 (key 11)
        (2600, -2846.2, 0.2, 93.2, 2.6991, 100, {}),          # switch 49
        (2800, -2545.0, 0.2, 1534.4, 1.5749, 140, {}),        # switch 50
        (3000, 2594.8, 0.2, 1545.5, -1.5329, 100, {}),        # switch 56: "graveyard gate open." (gate 63)
        (3200, -888.4, 0.0, 2506.0, 0.5892, 160, {}),         # action 46 (key 12: the platform 1, door 60): open
        (3450, 1108.6, 0.0, 2500.9, -0.5649, 160, {}),        # action 47
        (3800, 391.6, 0.0, 5862.8, 1.4818, 0, {"trig": 1, "fire": 1500, "aim": "enemy"}),  # sister 3 (actor 31): open
        (5400, 198.8, 0.0, -6489.0, -1.4076, 150, {}),        # warp 2608 -> 132
    ]),
    ("132 Lev2 H again: the level exit 7200", 29750, [
        (0, -176.3, 797.0, 4371.7, -3.0575, 150, {}),         # warp 7200 (objectives done: the City Totem, 110; else 2000)
    ]),
]


def build_level2(fight_frames=True):
    inputs = [NEW_GAME, "# 60 the hub: the level 2 portal (warp 2000)", "CU@%d..%d" % (HUB_ARRIVAL + 210, HUB_ARRIVAL + 500)]
    tps = ["%d:1328.3,200.5,-986.9,-0.6088" % (HUB_ARRIVAL + 200)]
    aim_at, aim_enemy, fire = [], [], []
    for name, rel, steps in L2_SEGMENTS:
        inputs.append("# " + name)
        for dt, x, y, z, ry, walk, o in steps:
            f = L2_START + rel + dt
            tps.append("%d:%.1f,%.1f,%.1f,%.4f%s" % (f, x, y, z, ry, "!" if o.get("trig") else ""))
            if walk:
                inputs.append("CU@%d..%d" % (f + 10, f + 10 + walk))
            if o.get("fire"):
                a, b = f + 10, f + 10 + o["fire"]
                fire.append((a, b))
                if o.get("aim") == "enemy":
                    aim_enemy.append("%d-%d" % (a, b))
                elif o.get("aim"):
                    aim_at.append("%d-%d:%.1f,%.1f,%.1f" % ((a, b) + tuple(o["aim"])))
    for a, b in fire:
        for f in range(a, b, FIRE_EVERY):
            inputs.append("Z@%d..%d" % (f, f + 6))
    env = ["T2_WARP_TO=1999", "T2_GIVE_WEAPONS=1", "T2_WEAPON=10", "T2_CHEATS=1", "T2_FILL_AMMO=300",
           "T2_AIM_AT=\"%s\"" % ";".join(aim_at), "T2_AIM_ENEMY=\"%s\"" % ";".join(aim_enemy)]
    return inputs, tps, env


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out", nargs="?", default=".")
    ap.add_argument("--fight", type=int, metavar="WEAPON", help="fight the totem defense with this weapon")
    ap.add_argument("--level", type=int, default=1, choices=(1, 2), help="1: the Port of Adia, 2: the River of Souls")
    a = ap.parse_args()
    if a.level == 2:
        inputs, tps, env = build_level2()
    else:
        inputs, tps = build(SEGMENTS)
        env = []
    if a.fight is not None and a.level == 1:
        inputs.append("# 109 the Harbor Totem: fire taps (T2_FACE_NEAREST aims)")
        for f in range(TOTEM_ARRIVAL, TOTEM_END, 15):
            inputs.append("Z@%d..%d" % (f, f + 6))
        env += ["T2_GIVE_WEAPONS=1", "T2_WEAPON=%d" % a.fight, "T2_FACE_NEAREST=4000@109"]
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
