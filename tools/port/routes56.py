"""Levels 5 and 6 for tools/port/playthrough.py (Phase 7 round 11 z4, docs/port/PLAYTHROUGH.md section 9).

--level 5, the Hive of the Mantids: from the hub (T2_WARP_TO=1999 on the new game's first level)
through the hub portal 5000 and its intro cinemas to level 124 (HIVE TOP), then the sub-levels
124 -> 84 -> 85 -> 86 -> 87 -> 88 -> 89 -> 90 -> 94 (embryo 1) -> 91 (the hive's hub) -> 95
(embryo 2) -> 91 -> 96 (embryo 3) -> 91 -> 93 (the master computer) -> 91 and the level exit 7500
(the Hive Totem, level 113, once the objectives are done), the totem's defence, the Mantid Queen
(level 2) and the hub (round 12 a1: the whole level, docs/port/PLAYTHROUGH.md section 9.4).

--level 6, the Primagen's Lightship: the hub portal 6000 and its intro cinemas to level 114 (the
ship's hub), the three power-generator wings 115, 117 (via 116) and 119 (via 118), the assembly
plants 121, 122, 123 (via 120, with the fourth generator in 123) and back to 114, whose exit
7601 is the Mother's arena (level 3); after her, the hub and a hop to the Primagen (7602, level 4),
his death, the ending and the credits (round 12 a2: PLAYTHROUGH.md 9.4).

The objectives (Mission_LevelObjectivesDone: link group types 9/10 and 11/12, the types table
puts them at link groups 44/98 and 97/89):
  level 5: 3 embryos (object 109: 94 actor 19, 95 actor 27, 96 actor 31, each in a cocoon: 78, 94,
           95; "embryo destroyed.", the last "all embryos destroyed. mission complete."), the
           master computer (93: four sockets, actors 7-10, each fed a satchel charge from 91;
           actor 53: "master computer destroyed. mission complete.").
  level 6: 4 power generators (each one 4 couplings, object 288, around actor 45 of 115, 42 of
           117, 31 of 119, 37 of 123: "power generator recalibrated.", the last "all power
           generators recalibrated. mission complete."; walked into, each one uses a crystal ion
           capacitor, item 4231), 3 assembly plants (121 actor 37, object 376; 122 actor 23,
           object 283: plasma fire; 123: its two laser power crystal slots, actions 24 and 25,
           object 183, a red laser cell (4229) each: "assembly plant destroyed.", "all assembly
           plants destroyed. mission complete.").

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
L5_HOPS = []             # round 12 a1: none (z4's 5201 hop: the pylons fall now, the field drops)
L6_HOPS = []

L5_SEGMENTS = [
    ("124 HIVE TOP: the 4 force field pylons, exit 5201", 0, [
        # the pylons (object 139, actors 9-12): each death counts link group 49 down; at 0 the
        # hive's force field (actor 23) goes: "force field down."
        # round 12 a1: the pylons are turrets (intelligence type 4, 100 health, Turret_Hurt): the
        # plasma rifle kills each in ~3 hits at its gun node (~170 high); z4's fixed aim at 100
        # passed under it. T2_AIM_ENEMY with the pylon as its first actor ("aim": ("actor", A)).
        (0, 393.2, 0.0, -2020.3, 1.6954, 0, {"fire": 300, "aim": ("actor", 9)}),
        (400, -393.2, 0.0, 2941.9, -1.3826, 0, {"fire": 300, "aim": ("actor", 10)}),
        (800, 2481.1, 0.0, 854.0, 0.1887, 0, {"fire": 300, "aim": ("actor", 11)}),
        (1200, -2480.3, 0.0, 858.0, -0.1850, 0, {"fire": 300, "aim": ("actor", 12)}),
        # "force field down." (link group 49 at 0): the hive's centre is open
        (1700, 154.9, -228.3, 445.2, 1.6811, W2, {}),      # warp 5201 -> 84 (approach.py --steps 1)
    ]),
    # the three level 6 keys (object 586, item 0x10FE: "level 6 key") lie in 84, 87 and 89
    ("84 Lev5 A: a level 6 key, exit 5100", 2800, [
        (0, 1016.5, -20.5, -2241.2, -0.7767, 40, {}),         # level 6 key (1075.2, 10.2, -2300.9)
        (200, 4628.7, -307.0, 1077.8, -1.5437, W2, {}),          # warp 5100 -> 85
    ]),
    ("85 Lev5 B: exit 5013", 3400, [
        (0, -2152.3, -1228.6, -6033.8, -3.1373, W2, {}),      # warp 5013 -> 86
    ]),
    ("86 Lev5 C: exit 5120", 3800, [
        (0, 5811.3, -1843.0, 360.6, -1.3395, W2, {}),         # warp 5120 -> 87
    ]),
    ("87 Lev5 D: a level 6 key, exit 5023", 4200, [
        (0, 713.3, -2150.4, 3471.0, 0.8432, 40, {}),          # level 6 key (615.1, -2119.7, 3383.5)
        (200, -2264.2, -2764.6, 2714.7, 1.8080, W2, {}),        # warp 5023 -> 88
    ]),
    ("88 Lev5 E: exit 5033", 4800, [
        (0, -262.4, -614.2, -4583.0, 0.2630, W2, {}),          # warp 5033 -> 89
    ]),
    ("89 Lev5 F: a level 6 key, exit 5053", 5200, [
        (0, 6772.1, -614.4, -369.6, 2.7291, 40, {}),          # level 6 key (6743.2, -583.7, -303.6)
        (200, 6873.9, -921.4, 7364.7, -1.5618, W2, {}),         # warp 5053 -> 90
    ]),
    ("90 Lev5 G: exit 5010", 5800, [
        (0, -4513.8, 614.4, 3509.7, -2.8650, W2, {}),          # warp 5010 -> 94
    ]),
    # the embryos (object 109, an enemy, 255 health) sleep (EnemyMode_WaitForTrigger, no hits) in
    # a cocoon (object 380, a destructible, 255 health, at the same place); the cocoon's death
    # (link head 0x40000) wakes the embryo (kind 1). Fire at the cocoon first (T2_AIM_ENEMY ,R,A:
    # the level actor A, then the nearest live actor: the embryo). The shredder (weapon 9) breaks
    # a cocoon in ~250 frames (the plasma rifle needs ~1,500). Each embryo counts link group 44
    # (type 9) down: "embryo destroyed.", the last "all embryos destroyed. mission complete."
    ("94 Lev5 J: embryo 1, exit 5021", 6200, [
        (0, 1399.3, 0.0, -4106.0, 0.7189, 0, {"fire": 400, "aim": ("actor", 78)}),   # the cocoon 78
        (450, 1399.3, 0.0, -4106.0, 0.7189, 0, {"fire": 600, "aim": ("actor", 19)}), # the embryo 19
        (1100, -3550.0, 0.0, -2814.5, 1.9267, W2, {}),        # warp 5021 -> 91
    ]),
    # the 4 satchel charges (object 599, item 0x100E: CPlayer__CollectKeyPiece) lie in 91; the
    # master computer's 4 sockets in 93 take one each (Action_CheckRequiredItem)
    ("91 Lev5 H: the 4 satchel charges, exit 5500", 7700, [
        (0, 921.6, -307.2, -7944.8, 3.1416, 40, {}),          # satchel (921.6, -307.2, -7884.8)
        (200, 3072.0, -307.2, -5845.6, 3.1416, 40, {}),       # satchel (3072, -307.2, -5785.6)
        (400, 1490.4, 614.4, -3041.9, -2.0036, 15, {}),       # satchel (1536, 614.4, -3020.8), on a ledge (region 539)
        (600, 307.2, -307.2, -1903.2, 3.1416, 40, {}),        # satchel (307.2, -307.2, -1843.2)
        (800, -299.6, 0.0, 4504.8, -1.2299, W2, {}),          # warp 5500 -> 95
    ]),
    ("95 Lev5 K: embryo 2, exit 5501", 8900, [
        (0, 511.1, 0.0, -1246.8, 2.2706, 0, {"fire": 400, "aim": ("actor", 94)}),   # the cocoon 94
        (450, 511.1, 0.0, -1246.8, 2.2706, 0, {"fire": 600, "aim": ("actor", 27)}), # the embryo 27
        (1100, 2674.2, 0.0, 4158.1, 0.2983, W2, {}),         # warp 5501 -> 91
    ]),
    ("91 Lev5 H again: exit 5073", 10400, [
        (0, 3389.8, 614.4, -3787.7, -1.2373, W2, {}),         # warp 5073 -> 96
    ]),
    ("96 Lev5 L: embryo 3, exit 5072", 10800, [
        (0, -2570.0, 1228.8, 597.5, 2.2899, 0, {"fire": 400, "aim": ("actor", 95)}),   # the cocoon 95
        (450, -2570.0, 1228.8, 597.5, 2.2899, 0, {"fire": 600, "aim": ("actor", 31)}), # the embryo 31
        (1100, -142.3, 921.6, -1027.8, -1.2276, W2, {}),      # warp 5072 -> 91
    ]),
    ("91 Lev5 H again: exit 5060", 12300, [
        (0, -731.8, 614.4, -1447.8, -2.0128, W2, {}),         # warp 5060 -> 93
    ]),
    # the master computer: its 4 sockets (actors 7-10, actions needing item 0x100E, auto radius
    # 174) each take a satchel charge and count link group 99 down; at 0 the computer (actor 54):
    # "warning, five seconds until detonation.", then actor 53 (group 98, type 10): "master
    # computer destroyed. mission complete." z4's aim point (1843, 922, 900) was over the pit
    # round the room: the fall killed the player (level 164) even with T2_CHEATS.
    ("93 Lev5 I: the master computer, exit 5062", 12700, [
        (0, 1510.0, 921.6, 662.3, -0.1282, 40, {}),           # socket actor 7 (1536, 921.6, 460.8)
        (200, 931.6, 921.6, 629.3, 0.0592, 40, {}),           # socket actor 8 (921.6, 921.6, 460.8)
        (400, 1564.7, 921.6, -94.0, 0.4488, 40, {}),          # socket actor 9 (1536, 921.6, -153.6)
        (600, 942.6, 921.6, -317.0, 3.0140, 40, {}),          # socket actor 10 (921.6, 921.6, -153.6)
        (1000, 1277.1, 921.6, -2778.9, 0.3221, W2, {}),       # warp 5062 -> 91 (after the blast)
    ]),
    ("91 Lev5 H again: the level exit 7500", 14100, [
        (0, 2765.4, -614.4, -7569.4, 3.1309, W2, {}),         # warp 7500 (the Hive Totem, 113)
    ]),
]

# the Hive Totem (113) arrives at ~32,000 (rel 14,300); saved (8 kills) by ~33,500
L5_TOTEM = (14250, 2000, 113)
# the Mantid Queen (level 2, ~33,100 after the totem): fire held to the end of the run; her death
# (mode 16) at ~42,100, the hub (60) by ~43,500 (run 48,000 frames: the fight varies)
L5_BOSS = (15900, 16000)

def _couplings(dt0, cx, y, cz, half=102.4, out=60.0, gap=200, walk=50):
    """Steps that walk into the 4 couplings (object 288, action intelligence 574) round a power
    generator at (cx, cz): each one from `out` units outside it along x, facing it, C-up `walk`
    frames. A coupling goes when the player comes within its m_AutoGoRadius (41; 35.8 in 123)
    (CScene__CheckAutoActivation, type flag 4) and holds a crystal ion capacitor (item 4231,
    Action_CheckRequiredItem uses one per coupling)."""
    steps = []
    for k, (sx, sz) in enumerate(((1, 1), (1, -1), (-1, -1), (-1, 1))):
        x, z = cx + sx * half, cz + sz * half
        steps.append((dt0 + k * gap, x + sx * out, y, z, sx * 1.5708, walk, {}))
    return steps


# round 12 a2: the items, handed by T2_GIVE_ITEMS (relative frame, spec) in --level 6 (a run from a
# new game). Round 13 b1: they are NOT from earlier levels. Cart 2 maps object 597 -> item 4231 (the
# crystal ion capacitor) and 595 -> 4229 (the red laser cell); a2 read object 601 (item 4400) and 599
# (4110, the satchel charge). Level 6 itself holds 16 capacitors, one per coupling (L6_ITEMS), and
# four red cells in 123 (on the blue slots 26-29); the continuous run (fullrun.py) picks them up.
L6_GIVE = [(400, "4231:4"), (3000, "4231:4"), (5100, "4231:4"), (10200, "4231:4,4229:2")]

# round 13 b1: the level-6 pickups by segment index of L6_SEGMENTS (tools/assets/levels.py: level
# actors, object 597 = 4231, 595 = 4229, 592 = 4365 Primagen key 6; approaches by
# tools/port/itemreach.py). Entry: (item x, y, z, approach angle, distance, walk frames[, kind]).
# kind "air": the item hangs over a lower floor (on a machine's top): the stand point is 40 units
# out at the item's height - 20, and the walk falls through it. kind ("pit", dy, (x, y, z, rotY), F):
# the item hangs over (117: where its bridges, object 412 actors 58/60, extend; the route does not
# extend them) or lies on (116: region info 23, damage 10,000) a deadly floor: picked from the air
# (the item's height + dy), then a teleport onto a safe floor F frames later (DEBUG-ASSISTED).
P2 = 1.5708
L6_ITEMS = {
    1: [(-3840.0, 153.1, -1612.8, 0.0, 90, 50), (-3532.8, 153.1, -1612.8, 0.0, 90, 50),
        (2611.2, 41.3, -2304.0, 0.0, 40, 30, "air"), (-1228.8, 155.8, 537.6, 0.0, 40, 30, "air")],  # 115: 4 capacitors
    3: [(0.0, 155.4, -5068.8, 0.0, 90, 50), (-1862.3, -1945.6, 921.6, -P2, 40, 12, ("pit", 30.0, (0.0, 155.4, -4978.8, 0.0), 14))],  # 116: 2
    4: [(0.0, 51.5, 153.6, 0.0, 40, 30, ("pit", -20.0, (250.0, 0.0, 153.6, P2), 30)),
        (0.0, 51.5, 1689.6, 0.0, 40, 30, ("pit", -20.0, (250.0, 0.0, 1689.6, P2), 30))],     # 117: 2, over the pit
    6: [(-3906.8, -256.0, -1536.0, -P2, 90, 50), (-770.4, 0.3, 1136.3, P2, 90, 50)],               # 118: 2
    7: [(-2304.0, 308.0, -2380.8, P2, 90, 50), (-1075.2, 308.0, -2380.8, P2, 90, 50)],             # 119: 2
    9: [(1996.8, -153.2, 0.0, 0.0, 90, 50)],                                  # 120: 1
    10: [(307.2, 383.3, 0.0, 0.0, 40, 30, "air")],                            # 121: 1
    11: [(-1612.8, 305.6, 0.0, P2, 90, 50)],                                  # 122: 1
    12: [(0.0, 1.7, -1331.2, 0.0, 90, 50),                                    # 123: 1, then two red cells (4229)
         (-275.7, 1.7, -1228.8, 0.0, 90, 90), (275.7, 1.7, -1228.8, 0.0, 90, 90)],
    # 114's last visit: Primagen key 6 (4365) at its centre. On the first visit a walk from 90 units
    # out was pushed back 70 units (something round it, not identified): taken after "force field down."
    14: [(0.0, -50.8, 0.0, 0.0, 90, 50)],
}
ITEM_STEP = 150


def _item_steps(it, dt):
    import math
    x, y, z, a, d, walk = it[:6]
    kind = it[6] if len(it) > 6 else ""
    if kind == "air":
        y -= 20.0
    elif kind:
        y += kind[1]
    st = [(dt, x + math.sin(a) * d, y, z + math.cos(a) * d, a, walk, {})]
    if kind and kind != "air":
        st.append((dt + kind[3],) + tuple(kind[2]) + (0, {}))
    return st


def with_items(segments, items, step=ITEM_STEP):
    """segments with each segment's items picked up at its start (the segment's own steps and every
    later segment move by the time added); returns (segments, total frames added)."""
    assert all(0 <= i < len(segments) for i in items), "an item list past the last segment"
    out, added = [], 0
    for i, (name, rel, steps) in enumerate(segments):
        new, k = [], 0
        for it in items.get(i, ()):
            new += _item_steps(it, k)
            k += step
        new += [(s[0] + k,) + tuple(s[1:]) for s in steps]
        out.append((name + ("" if not k else " (+%d items)" % len(items[i])), rel + added, new))
        added += k
    return out, added


def l5_with_primagen_key():
    """L5_SEGMENTS with the detour to 92 (the hive's talisman level: Primagen key 5, item 4364, at
    (614.4, 2151.2, 2918.4)) between the satchel charges of 91 and its exit 5500: 91's warp 5813
    -> 92, the key, 92's exit 5814 -> 91, then 5500. Returns (segments, frames added)."""
    segs, add = [], 0
    for name, rel, steps in L5_SEGMENTS:
        if name.startswith("91 Lev5 H: the 4 satchel charges"):
            segs.append(("91 Lev5 H: the 4 satchel charges, exit 5813 (92)", rel,
                         list(steps[:-1]) + [(800, 4013.5, 614.4, -263.9, 3.1023, W2, {})]))   # warp 5813 -> 92
            segs.append(("92 Lev5 H TALISMAN: Primagen key 5, exit 5814", rel + 1200, [
                (0, 643.5, 2151.2, 3003.6, 0.3289, 50, {}),                   # Primagen key 5 (4364)
                (300, 258.0, 307.2, -1368.0, 1.5249, W2, {}),                 # warp 5814 -> 91 (4005, 614, -24)
            ]))
            segs.append(("91 Lev5 H again: exit 5500", rel + 2000, [(0,) + tuple(steps[-1][1:])]))
            add = 1200
            continue
        segs.append((name, rel + add, steps))
    return segs, add

L6_SEGMENTS = [
    ("114 Level 6A: exit 6100", 0, [
        (0, -3019.7, 38.3, -3525.4, 0.1928, W, {}),          # warp 6100 -> 115
    ]),
    ("115 Level 6B: power generator 1 (couplings 49-52), force field generator 38, exit 6101", 400,
     _couplings(0, 4915.2, 51.2, 1536.0) + [
        (800, 1201.0, 640.4, -2522.3, 2.0461, 80, {}),        # force field generator 38: "force field generator disabled."
        (1100, 3679.0, 38.3, 6552.4, 0.7521, W, {}),         # warp 6101 -> 114
    ]),
    ("114 Level 6A again: exit 6200", 1900, [
        (0, -3941.2, 38.3, 3218.2, -1.5381, W, {}),          # warp 6200 -> 116
    ]),
    ("116 Level 6C: force field generator 53, exit 6202", 2300, [
        (0, 3290.3, 1254.8, 1969.0, 0.4721, 80, {}),         # force field generator 53
        (300, -775.4, -1190.4, 5016.4, 2.2423, W, {}),       # warp 6202 -> 117
    ]),
    ("117 Level 6D: power generator 2 (couplings 45-48), exit 6298", 3000,
     _couplings(0, 2457.6, 51.2, -2304.0) + [
        (900, 3986.2, 38.3, 3173.2, 0.0380, W, {}),          # warp 6298 -> 114
    ]),
    ("114 Level 6A again: exit 6300", 4300, [
        (0, 2603.8, 38.3, 4555.6, 0.0308, W, {}),            # warp 6300 -> 118
    ]),
    ("118 Level 6E: exit 6302", 4700, [
        (0, -3019.6, 1420.8, 3064.6, -0.8187, W, {}),        # warp 6302 -> 119
    ]),
    ("119 Level 6F: power generator 3 (couplings 33-36), force field generator 23, exit 6398", 5100,
     # its couplings stand 154 units over the room's floor (y -307): the jump does not reach them
     # (open, PLAYTHROUGH.md 9.4), so each approach starts in the air at y -110, 45 units out
     _couplings(0, -5683.2, -110.0, -2457.6, out=45.0) + [
        (800, -703.2, 486.8, 7037.8, 0.4754, 80, {}),        # force field generator 23
        (1100, -607.0, -115.3, -3787.6, -0.8993, W, {}),     # warp 6398 -> 114
    ]),
    ("114 Level 6A again: exit 6010", 6600, [
        (0, 4252.5, 38.3, -2911.0, -2.4701, W, {}),          # warp 6010 -> 120
    ]),
    ("120 Level 6G: exit 6400", 7000, [
        (0, 3787.6, 38.3, 7.4, -2.4701, W, {}),              # warp 6400 -> 121
    ]),
    ("121 Level 6H: assembly plant 1 (object 376, actor 37), exit 6402", 7400, [
        (0, -532.0, 307.6, -1703.4, 2.8916, 0, {"fire": 900, "aim": (-614.4, 400.0, -1382.4)}),
        (1000, -146.2, 345.6, -4093.8, -0.8993, W, {}),      # warp 6402 -> 122
    ]),
    ("122 Level 6I: assembly plant 2 (object 283, actor 23), exit 6404", 8800, [
        (0, -1000.0, 307.0, 0.0, 1.5708, 0, {"fire": 900, "aim": (-1690.0, 400.0, 0.0)}),
        (1000, -605.9, 345.6, -3328.5, -3.1096, W, {}),      # warp 6404 -> 123
    ]),
    ("123 Level 6J: assembly plant 3 (the laser power crystal slots 24/25), power generator 4 "
     "(couplings 39-42), force field generator 33, exit 6498", 10200, [
        (0, 1245.1, 45.7, 1254.4, 1.5708, 50, {}),           # slot 24 (object 183, item 4229)
        (200, 1245.1, 45.7, 1510.4, 1.5708, 50, {}),         # slot 25: "assembly plant destroyed."
    ] + _couplings(400, 0.0, 51.2, -4608.0) + [
        (1200, 1871.0, 179.6, -7767.6, -1.0855, 80, {}),     # force field generator 33
        (1500, -1637.2, 38.3, -775.4, -1.5242, W, {}),       # warp 6498 -> 120
    ]),
    ("120 Level 6G again: exit 6020", 12000, [
        (0, -3787.6, 38.3, -7.4, 0.6715, W, {}),             # warp 6020 -> 114
    ]),
    ("114 Level 6A again: the force field is down: exit 7601 to the Primagen", 12400, [
        # the pad's region info has flag 0x200000: the warp fires on walking in from a region
        # without 0x40000 (enter_surface), not on a teleport onto it
        (0, 31.1, -305.6, 6000.0, 3.1416, W2, {}),           # warp 7601 (regions 1348/1349) -> the Primagen
    ]),
]

# 7601 with both objectives done is the Mother's arena (level 3, ~12,500 after L6_START): z5's
# Mother recipe (docs/port/STATUS.md: fire held, the aim on her weak spots, strafing left with a
# step back every 600 frames). Her death (~17,000 frames into her arena) warps 6999: back to 114 at its
# start; its other 7601 pad (regions 151/152, by the start) then leads to the hub (CPlayer__Warp:
# objectives done and the level's key 7 -> 6999). The hub has no warp region to the Primagen (7602):
# the route hops there (T2_WARP_TO, L6_PRIMAGEN_HOP) and runs z2's recipe (T2_BOSS_FRONT=450): his
# death, the ending cinema, the credits, the hub. The hop frame is tuned: his lair waits pass only
# when the player sees his eye past the lair door, and where he comes to rest at the lair (z 2918 to
# 2925) varies with the run before; from a hop at 53,700 he rested where no floor spot sees him
# (docs/port/PLAYTHROUGH.md 9.5).
L6_MOTHER = 12500          # the Mother's arena is loaded (relative to L6_START)
L6_MOTHER_END = 31000      # relative: the strafing and the steps back stop (her death ~29,600, her cinema)
L6_EXIT = (33500, 1231.8, 0.0, -4006.8, -1.5708)  # from 114's start (+x) onto its pad 7601 (regions 151/152) -> the hub
L6_PRIMAGEN_HOP = 34800    # relative: T2_WARP_TO=7602 from the hub (54,000)
L6_END = 90000             # relative: the end of the held fire


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
                elif isinstance(o.get("aim"), tuple) and o["aim"][0] == "actor":
                    # the level actor A first (T2_AIM_ENEMY's ,R,A), then the nearest live actor
                    aim_enemy.append("%d-%d,%d,%d" % (a, b, o["aim"][2] if len(o["aim"]) > 2 else 4000, o["aim"][1]))
                elif o.get("aim"):
                    aim_at.append("%d-%d:%.1f,%.1f,%.1f" % ((a, b) + tuple(o["aim"])))
    extra_env = []
    if level == 5:
        # the Hive Totem (113, from the exit 7500 once both objectives are done): fire taps with the
        # aim assist on the nearest enemy (as levels 1-4), 8 kills save it
        rel, length, lv = L5_TOTEM
        inputs.append("# %d the Hive Totem: fire taps (T2_FACE_NEAREST aims)" % lv)
        fire.append((start + rel, start + rel + length))
        # then the Mantid Queen (level 2): fire held, T2_AIM_NODE=boss@2, T2_BOSS_FRONT (z2's recipe;
        # the shredder kills her too); her death cinema and the hub
        rel, length = L5_BOSS
        inputs += ["# 2 the Mantid Queen: fire held, T2_AIM_NODE=boss@2, T2_BOSS_FRONT=450,200",
                   "Z@%d..%d" % (start + rel, start + rel + length)]
        extra_env = ["T2_FACE_NEAREST=4000@%d" % lv, "T2_AIM_NODE=boss@2", "T2_BOSS_FRONT=450,200",
                     "T2_BOSS_LOG=1"]
    for a, b in fire:
        for f in range(a, b, fire_every):
            inputs.append("Z@%d..%d" % (f, f + 6))
    warps = ["1999@12955"] + ["%d@%d" % (w, start + rel) for rel, w in hops]
    # level 5: the shredder (the cocoons); level 6: the plasma rifle
    env = ["T2_WARP_TO=%s" % ",".join(warps), "T2_GIVE_WEAPONS=1", "T2_WEAPON=%d" % (9 if level == 5 else 10),
           "T2_CHEATS=1", "T2_FILL_AMMO=%d" % (100 if level == 5 else 300)]
    env += extra_env
    if level == 6:
        # round 12 a2: the earlier levels' items, the Mother (level 3), the hop to the Primagen
        # (level 4), each boss with the boss env
        env.append("T2_GIVE_ITEMS=\"%s\"" % ";".join("%s@%d" % (spec, start + rel) for rel, spec in L6_GIVE))
        m = start + L6_MOTHER
        inputs += ["# 3 the Mother, then 4 the Primagen: fire held, T2_AIM_NODE=boss@3/4",
                   "Z@%d..%d" % (m + 800, start + L6_END), "CL@%d..%d" % (m + 5800, m + 15000)]
        inputs += ["CD@%d..%d" % (f, f + 250) for f in range(m + 5800, start + L6_MOTHER_END, 600)]
        rel, x, y, z, ry = L6_EXIT
        inputs += ["# 114 after the Mother: the pad 7601 by the start -> the hub", "CU@%d..%d" % (start + rel + 10, start + rel + 130)]
        tps.append("%d:%.1f,%.1f,%.1f,%.4f" % (start + rel, x, y, z, ry))
        env[0] += ",7602@%d" % (start + L6_PRIMAGEN_HOP)
        env[env.index("T2_FILL_AMMO=300")] = "T2_FILL_AMMO=600"
        env += ["T2_AIM_NODE=boss@3/4", "T2_BOSS_FRONT=450", "T2_BOSS_LOG=1"]
    if aim_at:
        env.append("T2_AIM_AT=\"%s\"" % ";".join(aim_at))
    if aim_enemy:
        env.append("T2_AIM_ENEMY=\"%s\"" % ";".join(aim_enemy))
    return inputs, tps, env
