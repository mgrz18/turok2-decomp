"""Levels 5 and 6 for tools/port/playthrough.py (Phase 7 round 11 z4, docs/port/PLAYTHROUGH.md section 7).

--level 5, the Hive of the Mantids: from the hub (T2_WARP_TO=1999 on the new game's first level)
through the hub portal 5000 and its intro cinemas to level 124 (HIVE TOP), then the sub-levels
124 -> 84 -> 85 -> 86 -> 87 -> 88 -> 89 -> 90 -> 94 (embryo 1) -> 91 (the hive's hub) -> 95
(embryo 2) -> 91 -> 96 (embryo 3) -> 91 -> 93 (the master computer) -> 91 and the level exit 7500
(the Hive Totem, level 113, once the objectives are done).

--level 6, the Primagen's Lightship: the hub portal 6000 and its intro cinemas to level 114 (the
ship's hub), the three power-generator wings 115, 117 (via 116) and 119 (via 118), the assembly
plants 121, 122, 123 (via 120, with the fourth generator in 123) and back to 114, whose exit
7601 is the Primagen's arena (level 3).

The objectives (Mission_LevelObjectivesDone: link group types 9/10 and 11/12, the types table
puts them at link groups 44/98 and 97/89):
  level 5: 3 embryos (object 109: 94 actor 19, 95 actor 27, 96 actor 31; "embryo destroyed.",
           the last "all embryos destroyed. mission complete."), the master computer (93 actor 54,
           object 331: "master computer destroyed. mission complete.").
  level 6: 4 power generators (each one 4 couplings, object 288, around actor 45 of 115, 42 of
           117, 31 of 119, 37 of 123: "power generator recalibrated.", the last "all power
           generators recalibrated. mission complete."), 3 assembly plants (121 actor 37, object
           376; 122 actor 23, object 283; 123 actors 24 and 25, object 183: "assembly plant
           destroyed.", "all assembly plants destroyed. mission complete.").

Step format as playthrough.py's L2_SEGMENTS: (dt, x, y, z, rotY, walk, opts); opts "trig" (a "!"
teleport), "fire" (frames of fire taps), "aim" ((x, y, z) for T2_AIM_AT or "enemy").
"""

HUB_ARRIVAL = 13100      # T2_WARP_TO=1999 at 12,955: the hub (level 60)

# the hub portals: the approach point (tools/port/approach.py 60 --warp N), C-up into the region
L5_PORTAL = (-1701.5, -289.0, 945.7, 1.6540)    # warp 5000 -> the intro cinemas -> level 124
L6_PORTAL = (-8.0, -146.7, 2067.1, 2.8798)      # warp 6000 -> the intro cinemas -> level 114

L5_START = 17700         # level 124 is loaded (~17,500)
L6_START = 19200         # level 114 is loaded (~19,000)

W2 = 80                  # level 5: from the next region in (its warp regions have flag 0x200000: the warp
                         # fires on entering from a region without 0x40000, enter_surface in func_0024C47C)
W = 40                   # C-up frames on a warp region (teleported onto its centroid: the warp fires once the
                         # player stands on its floor, enter_surface in func_0024C47C)

# debug hops (T2_WARP_TO, relative to the level's start) where the route cannot pass yet
L5_HOPS = [(2450, 5201)]   # the hive's force field (actor 23) is still up: into level 84
L6_HOPS = []

L5_SEGMENTS = [
    ("124 HIVE TOP: the 4 force field pylons, exit 5201", 0, [
        # the pylons (object 139, actors 9-12): each death counts link group 49 down; at 0 the
        # hive's force field (actor 23) goes: "force field down."
        (0, 393.2, 0.0, -2020.3, 1.6954, 0, {"fire": 500, "aim": (0.0, 100.0, -1971.0)}),
        (600, -393.2, 0.0, 2941.9, -1.3826, 0, {"fire": 500, "aim": (0.0, 100.0, 2867.0)}),
        (1200, 2481.1, 0.0, 854.0, 0.1887, 0, {"fire": 500, "aim": (2406.0, 100.0, 461.0)}),
        (1800, -2480.3, 0.0, 858.0, -0.1850, 0, {"fire": 500, "aim": (-2406.0, 100.0, 461.0)}),
        # the pylons do not fall to the plasma rifle in this run (open, PLAYTHROUGH.md 7.4): the
        # field stays, so the route hops through the warp the field guards (T2_WARP_TO, L5_HOPS)
        (2400, 3.2, -228.3, 462.0, 1.3172, W, {}),         # warp 5201 -> 84 (inside the field)
    ]),
    ("84 Lev5 A: exit 5100", 2800, [
        (0, 4628.7, -307.0, 1077.8, -1.5437, W2, {}),          # warp 5100 -> 85
    ]),
    ("85 Lev5 B: exit 5013", 3200, [
        (0, -2152.3, -1228.6, -6033.8, -3.1373, W2, {}),      # warp 5013 -> 86
    ]),
    ("86 Lev5 C: exit 5120", 3600, [
        (0, 5811.3, -1843.0, 360.6, -1.3395, W2, {}),         # warp 5120 -> 87
    ]),
    ("87 Lev5 D: exit 5023", 4000, [
        (0, -2264.2, -2764.6, 2714.7, 1.8080, W2, {}),        # warp 5023 -> 88
    ]),
    ("88 Lev5 E: exit 5033", 4400, [
        (0, -262.4, -614.2, -4583.0, 0.2630, W2, {}),          # warp 5033 -> 89
    ]),
    ("89 Lev5 F: exit 5053", 4800, [
        (0, 6873.9, -921.4, 7364.7, -1.5618, W2, {}),         # warp 5053 -> 90
    ]),
    ("90 Lev5 G: exit 5010", 5200, [
        (0, -4513.8, 614.4, 3509.7, -2.8650, W2, {}),          # warp 5010 -> 94
    ]),
    ("94 Lev5 J: embryo 1, exit 5021", 5600, [
        (0, 1229.0, 0.0, -3500.0, 0.0, 0, {"fire": 900, "aim": (1229.0, 60.0, -4301.0)}),  # embryo actor 19
        (1000, -3550.0, 0.0, -2814.5, 1.9267, W2, {}),        # warp 5021 -> 91
    ]),
    ("91 Lev5 H: exit 5500", 7000, [
        (0, -299.6, 0.0, 4504.8, -1.2299, W2, {}),            # warp 5500 -> 95
    ]),
    ("95 Lev5 K: embryo 2, exit 5501", 7400, [
        (0, 307.0, 0.0, -300.0, 0.0, 0, {"fire": 900, "aim": (307.0, 60.0, -1075.0)}),     # embryo actor 27
        (1000, 2674.2, 0.0, 4158.1, 0.2983, W2, {}),         # warp 5501 -> 91
    ]),
    ("91 Lev5 H again: exit 5073", 8800, [
        (0, 3389.8, 614.4, -3787.7, -1.2373, W2, {}),         # warp 5073 -> 96
    ]),
    ("96 Lev5 L: embryo 3, exit 5072", 9200, [
        (0, -2000.0, 1229.0, 768.0, 1.5708, 0, {"fire": 900, "aim": (-2765.0, 1290.0, 768.0)}),  # embryo actor 31
        (1000, -142.3, 921.6, -1027.8, -1.2276, W2, {}),      # warp 5072 -> 91
    ]),
    ("91 Lev5 H again: exit 5060", 10600, [
        (0, -731.8, 614.4, -1447.8, -2.0128, W2, {}),         # warp 5060 -> 93
    ]),
    ("93 Lev5 I: the master computer, exit 5062", 11000, [
        (0, 1843.0, 922.0, 900.0, 0.0, 0, {"fire": 900, "aim": (1843.0, 1000.0, 154.0)}),  # master computer actor 54
        (1000, 1277.1, 921.6, -2778.9, 0.3221, W2, {}),      # warp 5062 -> 91
    ]),
    ("91 Lev5 H again: the level exit 7500", 12400, [
        (0, 2765.4, -614.4, -7569.4, 3.1309, W2, {}),         # warp 7500 (the Hive Totem, 113)
    ]),
]

L6_SEGMENTS = [
    ("114 Level 6A: exit 6100", 0, [
        (0, -3019.7, 38.3, -3525.4, 0.1928, W, {}),          # warp 6100 -> 115
    ]),
    ("115 Level 6B: power generator 1, exit 6101", 400, [
        (0, 4915.0, 51.0, 2300.0, 0.0, 0, {"fire": 900, "aim": (4915.0, 100.0, 1536.0)}),  # couplings 49-52
        (1000, 3679.0, 38.3, 6552.4, 0.7521, W, {}),         # warp 6101 -> 114
    ]),
    ("114 Level 6A again: exit 6200", 1800, [
        (0, -3941.2, 38.3, 3218.2, -1.5381, W, {}),          # warp 6200 -> 116
    ]),
    ("116 Level 6C: exit 6202", 2200, [
        (0, -775.4, -1190.4, 5016.4, 2.2423, W, {}),         # warp 6202 -> 117
    ]),
    ("117 Level 6D: power generator 2, exit 6298", 2600, [
        (0, 2457.0, 51.0, -1500.0, 0.0, 0, {"fire": 900, "aim": (2457.0, 100.0, -2304.0)}),  # couplings 45-48
        (1000, 3986.2, 38.3, 3173.2, 0.0380, W, {}),         # warp 6298 -> 114
    ]),
    ("114 Level 6A again: exit 6300", 4000, [
        (0, 2603.8, 38.3, 4555.6, 0.0308, W, {}),            # warp 6300 -> 118
    ]),
    ("118 Level 6E: exit 6302", 4400, [
        (0, -3019.6, 1420.8, 3064.6, -0.8187, W, {}),        # warp 6302 -> 119
    ]),
    ("119 Level 6F: power generator 3, exit 6398", 4800, [
        (0, -5683.0, -154.0, -1700.0, 0.0, 0, {"fire": 900, "aim": (-5683.0, -100.0, -2457.0)}),  # couplings 33-36
        (1000, -607.0, -115.3, -3787.6, -0.8993, W, {}),     # warp 6398 -> 114
    ]),
    ("114 Level 6A again: exit 6010", 6200, [
        (0, 4252.5, 38.3, -2911.0, -2.4701, W, {}),          # warp 6010 -> 120
    ]),
    ("120 Level 6G: exit 6400", 6600, [
        (0, 3787.6, 38.3, 7.4, -2.4701, W, {}),           # warp 6400 -> 121
    ]),
    ("121 Level 6H: assembly plant 1, exit 6402", 7000, [
        (0, -614.0, 307.0, -700.0, 0.0, 0, {"fire": 900, "aim": (-614.0, 400.0, -1382.0)}),
        (1000, -146.2, 345.6, -4093.8, -0.8993, W, {}),      # warp 6402 -> 122
    ]),
    ("122 Level 6I: assembly plant 2, exit 6404", 8400, [
        (0, -1000.0, 307.0, 0.0, 1.5708, 0, {"fire": 900, "aim": (-1690.0, 400.0, 0.0)}),
        (1000, -605.9, 345.6, -3328.5, -3.1096, W, {}),      # warp 6404 -> 123
    ]),
    ("123 Level 6J: assembly plant 3, power generator 4, exit 6498", 9800, [
        (0, 1185.0, 46.0, 2100.0, 0.0, 0, {"fire": 900, "aim": (1185.0, 140.0, 1382.0)}),
        (1000, 0.0, 51.0, -3800.0, 0.0, 0, {"fire": 900, "aim": (0.0, 100.0, -4608.0)}),  # couplings 39-42
        (2000, -1637.2, 38.3, -775.4, -1.5242, W, {}),       # warp 6498 -> 120
    ]),
    ("120 Level 6G again: exit 6020", 12200, [
        (0, -3787.6, 38.3, -7.4, 0.6715, W, {}),            # warp 6020 -> 114
    ]),
]


def build_level56(level, new_game, fire_every):
    """The --input lines, the T2_TELEPORT entries and the env lines for level 5 or 6."""
    portal, start, segs, hops = ((L5_PORTAL, L5_START, L5_SEGMENTS, L5_HOPS) if level == 5 else
                                 (L6_PORTAL, L6_START, L6_SEGMENTS, L6_HOPS))
    inputs = [new_game, "# 60 the hub: the level %d portal (warp %d)" % (level, level * 1000),
              "CU@%d..%d" % (HUB_ARRIVAL + 210, HUB_ARRIVAL + 500)]
    tps = ["%d:%.1f,%.1f,%.1f,%.4f" % ((HUB_ARRIVAL + 200,) + portal)]
    aim_at, aim_enemy, fire = [], [], []
    for name, rel, steps in segs:
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
    for a, b in fire:
        for f in range(a, b, fire_every):
            inputs.append("Z@%d..%d" % (f, f + 6))
    warps = ["1999@12955"] + ["%d@%d" % (w, start + rel) for rel, w in hops]
    env = ["T2_WARP_TO=%s" % ",".join(warps), "T2_GIVE_WEAPONS=1", "T2_WEAPON=10", "T2_CHEATS=1", "T2_FILL_AMMO=300"]
    if aim_at:
        env.append("T2_AIM_AT=\"%s\"" % ";".join(aim_at))
    if aim_enemy:
        env.append("T2_AIM_ENEMY=\"%s\"" % ";".join(aim_enemy))
    return inputs, tps, env
