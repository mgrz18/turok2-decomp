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
T2_FACE_NEAREST=4000@109 (the aim assist only in level 109) and T2_FILL_AMMO=600 (the ammo
refilled every 600 frames: the plasma rifle's 150 cells run out at 4 kills of 8).

--level 2 (round 10 y1): the River of Souls instead, from the hub (T2_WARP_TO=1999 on the new
game's first level) through the hub portal 2000 and its intro cinemas to level 125, then the
sub-levels 125 -> 126 -> 127 -> 128 -> 129 (soul gate 1) -> 130 -> 133 (sister of despair 1) ->
130 -> 131 -> 134 (sister 2) -> 131 -> 132 (soul gate 2) -> 135 (the graveyard keys, sister 3)
-> 132 and the level exit 7200 to the City Totem (110), its defense, and the hub (round 11 z1).
Fights: T2_AIM_AT (a soul gate) or T2_AIM_ENEMY windows (the nearest live actor, or "enemy:A":
level actor A first) with fire taps, the plasma rifle, T2_CHEATS=1 and T2_FILL_AMMO; in 110
T2_FACE_NEAREST=4000@110 with T2_FACE_PITCH=1; a "!" teleport also fires the region's enter
triggers (the enemy wake-ups a walk-in would fire). docs/port/PLAYTHROUGH.md sections 6.2, 6.5.
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
        # region info 7: zombies 22-24 -> door 28 -> sister 20. Round 13 b2: the zombies aimed one by one
        # first (with the sister aimed from the start, door 28 opened only when stray shots had killed
        # them, ~5,000 frames, and she woke too late for the window in some runs), then the sister
        (1400, -847.0, -4761.6, 1574.0, 3.1416, 0, {"trig": 1, "fire": 1500, "aim": "enemy:22"}),
        (2950, -847.0, -4761.6, 1574.0, 3.1416, 0, {"fire": 1500, "aim": "enemy:23"}),
        (4500, -847.0, -4761.6, 1574.0, 3.1416, 0, {"fire": 1500, "aim": "enemy:24"}),
        (6050, -847.0, -4761.6, 1574.0, 3.1416, 0, {"fire": 1900, "aim": "enemy:20"}),
        (8000, 827.5, -4761.6, -1627.5, -2.8692, 150, {}),    # warp 2615 -> 130
    ]),
    ("130 Lev2 F again: exit 2103", 12050, [
        (0, 6063.6, -3532.6, -1300.2, -0.0653, 150, {}),      # warp 2103 -> 131
    ]),
    ("131 Lev2 G: exit 2612 (the second sister)", 12450, [
        (0, -4131.2, -51.2, 7062.5, -2.6877, 150, {}),        # warp 2612 -> 134
    ]),
    ("134 Lev2 J: the graveyard, zombies 2-4, gate 8, sister 2, exit 2613", 12850, [
        # round 11 z1: each zombie aimed in turn (the nearest-actor aim stayed on the generators' spawns
        # and gate 8 with the real aggression): 2, 3 at the start, 4 at the gate, then the sister
        (100, 78.7, 1024.0, -870.5, -1.5708, 0, {"fire": 1200, "aim": "enemy:2"}),   # zombie 2 comes to the start
        (1300, 78.7, 1024.0, -870.5, -1.5708, 0, {"fire": 1200, "aim": "enemy:3"}),  # zombie 3
        (2600, 1650.0, 1024.0, -870.0, -1.5708, 0, {"fire": 3000, "aim": "enemy:4"}),  # at the gate: zombie 4 -> gate 8 opens (key 7)
        (5700, 1650.0, 1024.0, -870.0, -1.5708, 0, {"fire": 2900, "aim": "enemy:1"}),  # sister 2 (actor 1)
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
        # round 11 z1: the actions 46/47 (object 156) want a graveyard key each (pickup 4025, "requires
        # graveyard key."): the keys lie in the two crypts, whose doors 75/76 open when the crypt zombies
        # 45/44 die; those wake in region info 19/20 (the rooms behind the crypts)
        (3200, -1043.0, 0.0, 926.0, -0.6480, 0, {"trig": 1, "fire": 2500, "aim": "enemy:45"}),  # info 19: zombie 45 -> door 75
        (5800, -780.0, 0.0, 560.0, -0.7420, 60, {}),          # into the crypt: graveyard key (-714, 0, 488)
        (6100, 1336.0, 0.0, 743.0, 0.6000, 0, {"trig": 1, "fire": 2500, "aim": "enemy:44"}),   # info 20: zombie 44 -> door 76
        (8700, 980.0, 0.0, 580.0, 0.7710, 60, {}),            # into the crypt: graveyard key (909, 0, 507)
        # the actions stand in the water hall's corners (regions 894/947, auto-go radius 256)
        (9000, -1050.0, 0.0, 2250.0, 0.6540, 40, {}),         # action 46 (a key used)
        (9300, 1250.0, 0.0, 2250.0, -0.6800, 40, {}),         # action 47: key 12 -> platform 1 -> door 60 -> sister 31 wakes
        (9700, 102.0, 0.0, 3600.0, 3.1416, 0, {"fire": 2000, "aim": "enemy:31"}),  # sister 3 (actor 31), in front of door 60
        (11800, 198.8, 0.0, -6489.0, -1.4076, 150, {}),       # warp 2608 -> 132
    ]),
    ("132 Lev2 H again: the level exit 7200", 36150, [
        (0, -176.3, 797.0, 4371.7, -3.0575, 150, {}),         # warp 7200 (objectives done: the City Totem, 110; else 2000)
    ]),
]


# the City Totem (110): the warp 7200 at L2_START + 36,160; its defense (as level 1's, 109)
L2_TOTEM_ARRIVAL = L2_START + 36150 + 400
L2_TOTEM_END = L2_TOTEM_ARRIVAL + 7200
# Round 13 b2: the defense from the south side of the totem (level actor 1 at (-102.4, 0, -153.6)),
# facing it; from the arrival ledge (-665.6, 307.2, -153.6), 642 units away, the aimed fire killed
# 4-6 of the 8 in 120 s (the attackers stay by the totem)
CITY_TOTEM_SPOT = (-102.4, 0.0, -330.0, 3.1416)


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
                elif isinstance(o.get("aim"), str) and o["aim"].startswith("enemy:"):
                    # "enemy:A" (round 11 z1): the level actor A first (a sister), else the nearest
                    aim_enemy.append("%d-%d,4000,%s" % (a, b, o["aim"][6:]))
                elif o.get("aim"):
                    aim_at.append("%d-%d:%.1f,%.1f,%.1f" % ((a, b) + tuple(o["aim"])))
    # round 11 z1: the City Totem (level 110) after the exit 7200: fire taps through its defense, aimed
    # by T2_FACE_NEAREST in level 110 only (as level 1's --fight does in 109)
    inputs.append("# 110 the City Totem: fire taps (T2_FACE_NEAREST aims)")
    fire.append((L2_TOTEM_ARRIVAL, L2_TOTEM_END))
    tps.append("%d:%.1f,%.1f,%.1f,%.4f" % ((L2_TOTEM_ARRIVAL + 400,) + CITY_TOTEM_SPOT))
    for a, b in fire:
        for f in range(a, b, FIRE_EVERY):
            inputs.append("Z@%d..%d" % (f, f + 6))
    env = ["T2_WARP_TO=1999", "T2_GIVE_WEAPONS=1", "T2_WEAPON=10", "T2_CHEATS=1", "T2_FILL_AMMO=300",
           "T2_AIM_AT=\"%s\"" % ";".join(aim_at), "T2_AIM_ENEMY=\"%s\"" % ";".join(aim_enemy),
           "T2_FACE_NEAREST=4000@110", "T2_FACE_PITCH=1", "T2_FACE_STICKY=1"]
    return inputs, tps, env


# ---- level 3: the Death Marshes (round 11 z3) ----
# The hub portal 3000 (walked into from the hub, then the level-3 intro cinemas): level 61 at ~18,250.
# --direct: T2_WARP_TO=3502 (the warp record in 61 that 62's exit uses) on the new game's level 51
# instead, 61 at ~13,100 (no hub, no cinemas: for quick runs).
# Objectives (Mission_LevelObjectivesDone(3): link group types 6 and 7, groups 38 and 95):
# 5 prisoners (object 328, an action of type 0x64C: walking into one fires "prisoner free.") and
# 3 ammo storage facilities (69/70/71: walking into object 231 with a satchel charge, item 4100,
# fires "warning, five seconds until detonation." and the armory's records). The 3 satchel
# charges (object 598) lie in 61, 64 and 66. Exit: 7300 in 68 (the Marsh Totem, 111).
L3_HUB_PORTAL = (HUB_ARRIVAL + 200, (-1738.6, 296.1, -691.7, 0.9279))   # warp 3000
L3_START = 18300
L3_DIRECT_START = 13100
L3_SEGMENTS = [
    ("61 Lev3 A: satchel 1, prisoner 1, exit 3777 (ammo storage 1)", 0, [
        (0, 4553.3, -431.0, -3553.5, -1.0913, 80, {}),       # satchel charge (4710, -410, -3635)
        (200, 2811.3, -222.0, -6848.7, -0.0103, 100, {}),    # prisoner actor 39 (2814, -289, -7113)
        (400, -381.5, 921.3, -5127.8, 2.9245, 150, {}),      # warp 3777 -> 69
    ]),
    ("69 Lev3 I: ammo storage 1, back 3778", 800, [
        (0, 1103.2, 308.4, -1854.4, 0.0466, 300, {}),        # the charge point, actor 23 (1075, 307, -2458)
        (600, -1492.1, -305.2, -1428.2, 0.1738, 150, {}),    # warp 3778 -> 61
    ]),
    ("61 Lev3 A again: exit 3501", 1600, [
        (0, 5243.5, -307.2, -5327.4, 0.6981, 150, {}),       # warp 3501 -> 62
    ]),
    ("62 Lev3 B: exit 3503", 2000, [
        (0, -2103.7, 10.7, 4645.5, 1.8513, 150, {}),         # warp 3503 -> 63
    ]),
    ("63 Lev3 C: exit 3505", 2400, [
        (0, 1125.0, 0.3, 4252.6, -0.2367, 250, {}),           # warp 3505 -> 64
    ]),
    ("64 Lev3 D: satchel 2, exit 3780 (ammo storage 2)", 2900, [
        (0, -1394.2, -53.7, 4557.0, -2.2502, 60, {}),        # satchel charge (-1331, 3, 4608)
        (150, -1820.5, 614.3, 2531.9, -1.2796, 150, {}),     # warp 3780 -> 70
    ]),
    ("70 Lev3 J: ammo storage 2, back 3779", 3400, [
        (0, 1629.3, -921.4, -524.6, -0.0187, 200, {}),       # the charge point, actor 37 (1638, -922, -992)
        (600, -887.1, -614.2, 1300.7, 0.1285, 150, {}),      # warp 3779 -> 64
    ]),
    ("64 Lev3 D again: exit 3289", 4200, [
        (0, 3479.2, 1231.6, 4476.1, -0.5426, 250, {}),       # warp 3289 -> 65
    ]),
    ("65 Lev3 E: prisoner 2, exit 3511", 4700, [
        (0, -610.9, 116.6, -4963.5, 0.9690, 100, {}),        # prisoner actor 16 (-830, 18, -5114)
        (200, 765.5, 924.4, -8547.0, -0.0027, 150, {}),       # warp 3511 -> 66
    ]),
    ("66 Lev3 F: satchel 3, prisoners 3-5, exit 3513", 5300, [
        (0, -204.0, -253.0, -1482.0, 2.7500, 60, {}),        # satchel charge (-308, -256, -1229), on its ledge (region 365)
        (200, -225.9, -498.8, -1444.3, 2.5131, 100, {}),     # prisoner actor 29 (-381, -596, -1231)
        (400, 849.3, -806.0, 2242.1, 2.5096, 100, {}),       # prisoner actor 30 (697, -903, 2450)
        (600, 2837.1, -806.0, 2262.7, -0.5851, 100, {}),     # prisoner actor 31 (2982, -903, 2044)
        (800, 3178.8, -614.4, -177.0, -0.3073, 100, {}),      # warp 3513 -> 67
    ]),
    ("67 Lev3 G: exit 3782 (ammo storage 3)", 6300, [
        (0, -2505.6, -0.2, 3278.9, -2.3889, 150, {}),         # warp 3782 -> 71
    ]),
    ("71 Lev3 K: ammo storage 3, back 3781", 6700, [
        (0, 1113.3, -1533.8, -1090.6, 1.1009, 200, {}),      # the charge point, actor 32 (833, -1536, -1233)
        (600, -800.8, -919.4, 191.7, 0.1449, 100, {}),      # warp 3781 -> 67
    ]),
    ("67 Lev3 G again: exit 3288", 7500, [
        (0, -3401.8, 0.0, 700.1, 2.8037, 100, {}),           # warp 3288 -> 68
    ]),
    ("68 Lev3 H: the level exit 7300", 7900, [
        (0, -2218.1, 0.0, -4896.3, -0.3706, 120, {}),        # warp 7300 (objectives done: the Marsh Totem, 111; else 3000)
    ]),
]
L3_TOTEM = (8300, 8300, 111)   # (fire from, fire for, totem level): relative to the start

# ---- level 4: the Lair of the Blind (round 11 z3) ----
# The hub portal 4000: level 98 at ~16,900 (--direct: T2_WARP_TO=4132, an inner warp record of 98, on level 51: 98 at ~13,100 without the intro cinemas; 4000 itself plays them).
# Objective (Mission_LevelObjectivesDone(4): link group type 8, group 21): 3 thermal vents, one in
# each of 106 (I), 107 (J), 108 (K): object 174, an action that needs a satchel charge (item 4111,
# object 600: in 101, 102 and 105). Each vent room is behind a door (object 442) that a switch
# (object 176, an action needing a key, item 4020, object 579: "this door requires a key.") opens.
# Exit: 7400 in 98 or 103 (the Cave Totem, 112; then 7401: the Blind One).
L4_HUB_PORTAL = (HUB_ARRIVAL + 200, (1711.7, -412.4, 968.4, -0.9589))   # warp 4000
L4_START = 17000
L4_DIRECT_START = 13100
L4_SEGMENTS = [
    ("98 Lev4 A: exit 4115", 0, [
        (0, 3243.8, -1831.2, 3289.2, 2.5120, 150, {}),        # warp 4115 (a drop) -> 99
    ]),
    ("99 Lev4 B: exit 4101", 400, [
        (0, 5455.8, -3683.3, -3164.2, -0.0962, 150, {}),      # warp 4101 (the pit) -> 100
    ]),
    ("100 Lev4 C: key 1, the level key, exit 4109", 1000, [
        (0, -1270.0, -2457.4, 1951.0, -1.4381, 120, {}),      # key (item 4020) (-955, -2406, 1909)
        (200, 945.9, -2457.6, 3583.4, -1.9265, 60, {}),       # level key (item 4350) (1074, -2437, 3631)
        (400, 2626.2, -1843.2, -5157.2, 0.1175, 150, {}),     # warp 4109 -> 101
    ]),
    ("101 Lev4 D: satchel 1, key 2, exit 4128 (vent 1)", 1800, [
        (0, -154.5, -2764.8, -1191.4, 2.6563, 120, {}),       # satchel charge (item 4111) (-324, -2714, -870)
        (200, 4588.6, -3379.2, -471.1, -1.7923, 60, {}),      # key (4749, -3379, -435)
        (400, 3064.6, -2457.6, 3257.4, -1.6434, 150, {}),     # warp 4128 -> 106
    ]),
    ("106 Lev4 I: the switch, vent 1, back 4129", 2600, [
        (0, 1752.9, -2457.4, -12.6, -2.0720, 100, {}),        # switch actor 3 (1869, -2458, 51): door 23 opens
        (300, 1851.6, -1770.2, 2460.8, 0.6476, 190, {}),      # vent actor 2 (1705, -1690, 2267)
        (800, -788.1, -2457.4, -2568.5, 0.4164, 150, {}),     # warp 4129 -> 101
    ]),
    ("101 Lev4 D again: exit 4111", 3800, [
        (0, -1016.6, -2457.6, -4383.8, 1.4982, 150, {}),      # warp 4111 -> 102
    ]),
    ("102 Lev4 E: key 3, satchel 2, exit 4130 (vent 2)", 4200, [
        (0, 1880.0, -4298.6, -74.2, -2.3162, 40, {}),         # key (1903, -4298, -53)
        (150, -2918.0, -2150.4, 2130.8, -1.3640, 80, {}),     # satchel charge (-2771, -2150, 2100)
        (350, 3716.1, -3379.0, 3334.2, -1.5166, 150, {}),     # warp 4130 -> 107
    ]),
    ("107 Lev4 J: the switch, vent 2, back 4131", 4950, [
        (0, 1752.9, -2457.4, -12.6, -2.0720, 100, {}),        # switch actor 2
        (300, 1904.3, -1770.2, 2348.4, 1.1829, 190, {}),      # vent actor 1
        (800, 1209.7, -2457.4, 247.0, 1.0030, 200, {}),       # warp 4131 -> 102
    ]),
    ("102 Lev4 E again: exit 4117", 6150, [
        (0, 987.5, -3379.5, -3405.4, 0.7498, 150, {}),        # warp 4117 -> 103
    ]),
    ("103 Lev4 F: exit 4107", 6550, [
        (0, -3220.0, -3991.9, -4329.8, -2.1256, 150, {}),     # warp 4107 -> 105
    ]),
    ("105 Lev4 H: satchel 3, exit 4113 (vent 3)", 6950, [
        (0, -7114.1, -1536.0, 4722.7, 1.4586, 300, {}),       # satchel charge (-7680, -1536, 4659)
        (500, -8107.1, -1536.0, 7311.7, 2.9817, 150, {}),     # warp 4113 -> 108
    ]),
    ("108 Lev4 K: the switch, vent 3, back 4114", 7850, [
        (0, 1742.6, -2457.4, -14.2, -2.5622, 150, {}),        # switch actor 13 or 14
        (300, 1938.8, -1770.2, 2250.1, 1.6428, 190, {}),      # vent actor 12
        (800, 133.3, -2457.4, -5058.3, 2.7420, 150, {}),      # warp 4114 -> 105
    ]),
    ("105 Lev4 H again: exit 4106", 9050, [
        (0, -5365.8, -2150.2, 4644.8, 2.9937, 150, {}),       # warp 4106 (a drop) -> 103
    ]),
    ("103 Lev4 F again: the level exit 7400", 9450, [
        (0, -10243.1, -3685.0, -288.5, 2.2092, 120, {}),      # warp 7400 (objective done: the Cave Totem, 112; else 4000)
    ]),
]
L4_TOTEM = (9850, 4000, 112)   # the Cave Totem's defence is won by ~29,600 (8/8)
# The Blind One (level 1, 7401 after the totem: keys[3] set, keys[5] not): at ~30,500. Round 10 y2's
# recipe: on the ledge (-400, 204.8, 0), fire held, T2_AIM_NODE=boss (only in level 1 here).
# Round 11 C26: the teleport 1,000 frames later (on the merged tree the Cave Totem is won at
# ~31,000 and the warp to level 1 lands at 31,264: the old 31,000 teleport came before it)
L4_BOSS = (15000, 12000, (-400.0, 204.8, 0.0, -1.5708), 1)


def build_hub_level(segments, start, portal=None, warp="1999", totem=None, boss=None):
    """A hub level's run: the new game, the hub (T2_WARP_TO=warp) and its portal (portal =
    (frame, teleport)) unless warp goes straight to a sub-level; segments as L2_SEGMENTS."""
    inputs = [NEW_GAME]
    tps = []
    if portal:
        f, (x, y, z, ry) = portal
        inputs += ["# 60 the hub: the level portal", "CU@%d..%d" % (f + 10, f + 300)]
        tps.append("%d:%.1f,%.1f,%.1f,%.4f" % (f, x, y, z, ry))
    aim_at, aim_enemy, fire = [], [], []
    for name, rel, steps in segments:
        inputs.append("# " + name)
        for dt, x, y, z, ry, walk, o in steps:
            f = start + rel + dt
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
    env = ["T2_WARP_TO=%s" % warp, "T2_GIVE_WEAPONS=1", "T2_WEAPON=10", "T2_CHEATS=1", "T2_FILL_AMMO=300"]
    if totem:
        rel, length, level = totem
        inputs.append("# %d the totem: fire taps (T2_FACE_NEAREST aims)" % level)
        fire.append((start + rel, start + rel + length))
        env.append("T2_FACE_NEAREST=4000@%d" % level)
    for a, b in fire:
        for f in range(a, b, FIRE_EVERY):
            inputs.append("Z@%d..%d" % (f, f + 6))
    if boss:
        rel, length, (x, y, z, ry), level = boss
        f = start + rel
        inputs += ["# %d the boss: fire held, T2_AIM_NODE=boss@%d" % (level, level), "Z@%d..%d" % (f + 10, f + length)]
        tps.append("%d:%.1f,%.1f,%.1f,%.4f" % (f, x, y, z, ry))
        env += ["T2_AIM_NODE=boss@%d" % level, "T2_BOSS_LOG=1"]
        env[env.index("T2_FILL_AMMO=300")] = "T2_FILL_AMMO=600"
    env += ["T2_AIM_AT=\"%s\"" % ";".join(aim_at), "T2_AIM_ENEMY=\"%s\"" % ";".join(aim_enemy)]
    return inputs, tps, env


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out", nargs="?", default=".")
    ap.add_argument("--fight", type=int, metavar="WEAPON", help="fight the totem defense with this weapon")
    ap.add_argument("--level", type=int, default=1, choices=(1, 2, 3, 4, 5, 6),
                    help="1: the Port of Adia, 2: the River of Souls, 3: the Death Marshes, 4: the Lair of the Blind, "
                         "5: the Hive of the Mantids, 6: the Lightship (tools/port/routes56.py)")
    ap.add_argument("--direct", action="store_true", help="level 3+: warp straight to the first sub-level (no hub)")
    ap.add_argument("--all", action="store_true", help="one continuous run from a new game: levels 1-6 (1-4 through the hub "
                    "portals), the keys, a save after each level, the Primagen, the ending (tools/port/fullrun.py, PLAYTHROUGH.md section 10)")
    a = ap.parse_args()
    if a.all:
        import sys
        sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
        import fullrun  # round 12 a3
        fullrun.main_all(a.out)   # levels 1-6, the Primagen, the ending (round 12 C27)
        return
    if a.level == 4:
        if a.direct:
            inputs, tps, env = build_hub_level(L4_SEGMENTS, L4_DIRECT_START, warp="4132", totem=L4_TOTEM, boss=L4_BOSS)
        else:
            inputs, tps, env = build_hub_level(L4_SEGMENTS, L4_START, portal=L4_HUB_PORTAL, totem=L4_TOTEM, boss=L4_BOSS)
    elif a.level == 3:
        if a.direct:
            inputs, tps, env = build_hub_level(L3_SEGMENTS, L3_DIRECT_START, warp="3502", totem=L3_TOTEM)
        else:
            inputs, tps, env = build_hub_level(L3_SEGMENTS, L3_START, portal=L3_HUB_PORTAL, totem=L3_TOTEM)
    elif a.level == 2:
        inputs, tps, env = build_level2()
    elif a.level in (5, 6):
        import sys
        sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
        import routes56  # round 11 z4: levels 5 and 6
        inputs, tps, env = routes56.build_level56(a.level, NEW_GAME, FIRE_EVERY)
    else:
        inputs, tps = build(SEGMENTS)
        env = []
    if a.fight is not None and a.level == 1:
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
