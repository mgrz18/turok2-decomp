#!/usr/bin/env python3
"""One continuous playthrough from a new game (docs/port/PLAYTHROUGH.md section 10, round 12 a3).

    python3 tools/port/playthrough.py OUTDIR --all [--upto N] [--no-extras]
        # OUTDIR/playthrough.txt, OUTDIR/teleport.env, OUTDIR/fullrun.txt (the timeline)

A single process: the new game, level 1 (the Port of Adia, its level keys, the talisman and
flesh-eater portals), the hub (the keys placed, a save on the pak), level 2, the hub, level 3,
the hub, level 4 (the Blind One), the hub. Each level's route is the one of `--level N`
(playthrough.py's segment lists) moved in time: a level starts a fixed time after the hub is
reached, so a route is shifted by (its hub arrival in this run) - (the arrival its own run had).
The hub arrivals are measured on the merged tree (STATUS.md); a change upstream that moves one
moves every later level: re-measure them with `T2_STATE_LOG=50` (the first "level 60" line after
each totem/boss) and edit HUB_AT below.

Levels 5 and 6 (round 12 C27) append through LEVEL_HOOKS: level 5 (the Hive of the Mantids, the
Hive Totem, the Mantid Queen), a save, level 6 (the Lightship, the Mother), the hop to the Primagen,
his death, the ending, the credits and the hub (routes56.py's routes moved in time, each sub-level
start re-anchored with T2_ANCHOR). The per-level settings (the shredder on level 5, the ammo fill,
the Queen's and the Primagen's T2_BOSS_FRONT) are frame-scheduled values ("V@F;...").
"""
import math, os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import playthrough as pt  # noqa: E402
import routes56 as r56  # noqa: E402


class Route:
    """Inputs, teleports and the frame-window debug switches of one run, in absolute frames."""

    def __init__(self):
        self.inputs = [pt.NEW_GAME]
        self.tps = []
        self.aim_at = []
        self.aim_enemy = []
        self.fire = []
        self.face_levels = []
        self.aim_node = None
        self.saves = []
        self.timeline = []
        self.fill_ammo = 300
        self.anchors = []
        # frame-scheduled values (round 12 C27): [(from frame, value), ...]
        self.weapon = [(0, "10")]
        self.fill = [(0, "300")]
        self.boss_front = []
        self.warps = []
        self.give = []
        # round 13 b5: where each level starts (its hub visit before it included), for the
        # per-level T2_CHEATS schedule (--cheats), and extra env lines (--env)
        self.level_start = {1: 0}
        self.no_cheats = set()
        self.extra_env = []
        self.cheat_windows = []
        self.cheats_on = set()   # sub-levels (the totems: 109 110 111 112) kept invincible (--cheats-on)

    def note(self, frame, text):
        self.inputs.append("# %d: %s" % (frame, text))
        self.timeline.append((frame, text))

    def tp(self, f, x, y, z, ry, walk=0, trig=False):
        self.tps.append("%d:%.1f,%.1f,%.1f,%.4f%s" % (f, x, y, z, ry, "!" if trig else ""))
        if walk:
            self.inputs.append("CU@%d..%d" % (f + 10, f + 10 + walk))

    def pick(self, f, x, y, z, ang=0.0, d=90.0, walk=50):
        """Stand d units from a pickup (or an action) at (x, y, z), facing it, and walk into it."""
        self.tp(f, x + math.sin(ang) * d, y, z + math.cos(ang) * d, ang, walk)

    def steps(self, segments, start, seven=True):
        """playthrough.py's segment lists: (name, rel, [(dt, x, y, z, rotY, walk[, opts]), ...])."""
        for seg in segments:
            name, rel, steps = seg if seven else (seg[0], seg[1] - start, seg[2])
            self.note(start + rel, name)
            for st in steps:
                dt, x, y, z, ry, walk = st[:6]
                o = st[6] if len(st) > 6 else {}
                f = start + rel + dt
                self.tp(f, x, y, z, ry, walk, o.get("trig"))
                if o.get("fire"):
                    a, b = f + 10, f + 10 + o["fire"]
                    self.fire.append((a, b))
                    aim = o.get("aim")
                    if aim == "enemy":
                        self.aim_enemy.append("%d-%d" % (a, b))
                    elif isinstance(aim, str) and aim.startswith("enemy:"):
                        self.aim_enemy.append("%d-%d,4000,%s" % (a, b, aim[6:]))
                    elif isinstance(aim, tuple) and aim[0] == "actor":
                        # routes56 (round 12 a1): the level actor A first, then the nearest live actor
                        self.aim_enemy.append("%d-%d,%d,%d" % (a, b, aim[2] if len(aim) > 2 else 4000, aim[1]))
                    elif aim:
                        self.aim_at.append("%d-%d:%.1f,%.1f,%.1f" % ((a, b) + tuple(aim)))

    def write(self, out, env_extra=()):
        for a, b in self.fire:
            for f in range(a, b, pt.FIRE_EVERY):
                self.inputs.append("Z@%d..%d" % (f, f + 6))
        def sched(vals):
            return ";".join(v if f == 0 else "%s@%d" % (v, f) for f, v in sorted(vals))
        cheats = [(f, "0" if lv in self.no_cheats else "1") for lv, f in sorted(self.level_start.items())]
        for a, b in self.cheat_windows:   # invincible inside [a, b) whatever the level's setting
            back = [v for f, v in cheats if f <= b][-1] if any(f <= b for f, v in cheats) else "1"
            cheats = [(f, v) for f, v in cheats if not a <= f < b] + [(a, "1"), (b, back)]
            cheats.sort()
        env = ["T2_GIVE_WEAPONS=1", "T2_WEAPON=\"%s\"" % sched(self.weapon), "T2_CHEATS=\"%s\"" % sched(cheats),
               "T2_FILL_AMMO=\"%s\"" % sched(self.fill),
               "T2_FACE_NEAREST=4000@%s" % ",".join(str(l) for l in self.face_levels),
               "T2_FACE_PITCH=1", "T2_FACE_STICKY=1", "T2_BOSS_LOG=1",
               "T2_AIM_AT=\"%s\"" % ";".join(self.aim_at), "T2_AIM_ENEMY=\"%s\"" % ";".join(self.aim_enemy)]
        if self.aim_node:
            env.append("T2_AIM_NODE=%s" % self.aim_node)
        if self.anchors:
            env.append("T2_ANCHOR=\"%s\"" % ";".join("%s@%d" % (l, f) for f, l in sorted(self.anchors, key=lambda a: a[0])))
        if self.saves:
            env.append("T2_SAVE_PORTAL=%s" % ",".join(str(f) for f in self.saves))
        if self.boss_front:
            env.append("T2_BOSS_FRONT=\"%s\"" % sched(self.boss_front))
        if self.warps:
            env.append("T2_WARP_TO=%s" % ",".join("%d@%d" % (w, f) for f, w in sorted(self.warps)))
        if self.give:
            env.append("T2_GIVE_ITEMS=\"%s\"" % ";".join("%s@%d" % (v, f) for f, v in sorted(self.give)))
        env += list(env_extra) + list(self.extra_env)
        with open(os.path.join(out, "playthrough.txt"), "w") as fp:
            fp.write("\n".join(self.inputs) + "\n")
        with open(os.path.join(out, "teleport.env"), "w") as fp:
            fp.write("T2_TELEPORT=\"%s\"\n" % ";".join(self.tps))
            for e in env:
                fp.write(e + "\n")
            fp.write("export T2_TELEPORT %s\n" % " ".join(e.split("=")[0] for e in env))
        with open(os.path.join(out, "fullrun.txt"), "w") as fp:
            for f, t in sorted(self.timeline):
                fp.write("%7d  %s\n" % (f, t))
        last = max([int(t.split(":")[0]) for t in self.tps] + [getattr(self, "end", 0)])
        print("%d inputs, %d teleports; last frame %d" % (len(self.inputs), len(self.tps), last))
        return last


# ---- level 1 with its keys (the level-1 route of playthrough.py, the key pickups inserted) ----
# The keys lying in level 1 (tools/assets/levels.py: level actors; the item id is the object
# type's id in cart entry 2): level 2 keys (item 4310, object 582) in 52 (2) and 58; level 3 keys
# (4320, object 583) in 55 and 59 (2); Primagen key 1 (4360, object 587) in 55.
# (segment index in pt.SEGMENTS, [(x, y, z, approach angle), ...]): picked up before the segment's
# own steps, 150 frames each; every later step moves by the time added.
L1_KEYS = {
    1: [(-153.6, 0.0, -1789.3, 0.0), (-153.5, 921.6, 2152.7, 0.0)],     # 52: level 2 keys
    4: [(1568.6, 0.0, -765.0, 0.0), (2662.4, 921.6, 1689.6, 0.0)],      # 55: level 3 key, Primagen key 1
    8: [(1262.1, 307.2, -1395.9, 0.0)],                                  # 58: level 2 key
    9: [(-613.6, 307.2, 639.4, 0.0), (-5682.8, 614.4, 6556.4, 0.0)],    # 59: level 3 keys
}
KEY_STEP = 150

# ---- health on the way (round 13 b5: the run without T2_CHEATS invincibility) ----
# The health pickups the routes pass near (tools/assets/levels.py; the item id is the object's id
# in cart entry 2: 1702 health +10 to 100, 1703 full health +100 to 100, 1704 ultra health +100 to
# 200; they lie on the floor), taken like the keys (150 frames each, at a segment's start).
# Before round 13 no health pickup was ever taken: the host table had m_Max 0 (gendata swapped the
# rows' union by its first member; include/structs/pickup.h CPickupRow18Health).
L1_HEALTH = {
    9: [(7372.8, 312.3, -256.0, 0.0)],                                   # 59: ultra health (the Harbor Totem next)
}
L2_HEALTH = {
    5: [(2508.2, 10.0, 3788.2, 0.0), (2048.0, -4443.2, 6041.6, 0.0)],   # 130: full + ultra health (sister 1 next, 133)
    7: [(-4249.6, 418.9, 1484.8, 0.0)],                                  # 130: ultra health (sister 2 next, 134)
    10: [(-1536.0, 419.6, -3328.0, 0.0)],                                # 131: full health
    12: [(482.3, 471.5, 6727.5, 0.0)],                                   # 135: ultra health (at its start: the crypts and sister 3 next)
}
# Not taken: 131's full health (-1996.3, 102.4, 6255.8), 132's (12288.0, 870.5, -665.6) and 132's
# ultra health (-6297.6, 102.4, 204.8) lie in damage regions (10 every 2 frames: a stand to pick one
# cost 330 to 740 health in a rescue run)
# a pick inside a segment (no shift): 135's ultra health after sister 3, before the City Totem
L2_HEALTH_LATE = []
# Not taken: the six health +10 beside sister 3's spot ((-479..681, 0, 3722..3828)): walking to them
# in the middle of her fight cost ~1,000 health (they lie in the pit's damage region)
L2_RETURN_LATE = []
# 135's first crypt key missed: without it the platform before door 60 stays down and sister 3's
# spot (102, 0, 3600) is a pit with a damage region (10 every 2 frames). A third walk at the key,
# after the second crypt (the retries are no shift).
L2_RETRY_B5 = [(12, 8850, -714.0, 0.0, 488.0, 0.0)]


def merged(*dicts):
    out = {}
    for d in dicts:
        for k, v in d.items():
            out.setdefault(k, []).extend(v)
    return out


def level1(r, start_shift=0, keys=True, health=True):
    """Level 1 from the new game: (the frame level 109 is reached, the extra frames added)."""
    added = 0
    for i, (name, base, steps) in enumerate(pt.SEGMENTS):
        f0 = base + start_shift + added
        r.note(f0, name)
        picks = merged(L1_KEYS, L1_HEALTH if health else {}) if keys else {}
        if i in picks:
            for x, y, z, ang in picks[i]:
                r.pick(f0, x, y, z, ang)
                f0 += KEY_STEP
                added += KEY_STEP
        for dt, x, y, z, ry, walk in steps:
            r.tp(f0 + dt, x, y, z, ry, walk)
    arrival = pt.TOTEM_ARRIVAL + start_shift + added
    r.note(arrival, "109 the Harbor Totem: fire taps (T2_FACE_NEAREST aims)")
    r.fire.append((arrival, arrival + 7150))
    r.face_levels.append(109)
    return arrival, added


# ---- the keys of levels 2-4 (inserted at the start of a segment of the level's own route) ----
# (segment index in the level's segment list, [(x, y, z, approach angle), ...]); y is the floor
# under the key (the key floats 30.7 above it)
L2_KEYS = {
    2: [(3760.1, 614.4, -302.3, 0.0)],                                   # 127: level 4 key (4330)
    8: [(1740.8, 409.6, 4968.8, 0.0)],                                   # 131: level 4 key
    11: [(2044.9, 1331.2, -255.9, 0.0), (-1697.9, 716.8, -1486.1, 0.0)],  # 132: level 4 key, Primagen key 2 (4361)
}
L3_KEYS = {
    0: [(1075.3, -307.2, 2663.8, 0.0)],                                  # 61: level 5 key (4340)
    3: [(-4761.5, 308.8, -1074.0, 1.5708)],                              # 62: level 5 key (from +x: no floor at +z, itemreach.py)
    4: [(-102.4, 307.6, 870.4, 0.0)],                                    # 63: Primagen key 3 (4362)
    13: [(-2150.5, 0.0, -1381.4, 0.0)],                                  # 68: level 5 key
}
# round 13 b1: 62's and 68's level-5 keys were picked before the warp into the sub-level had loaded
# (and 68's sat at index 15 of a 14-segment list: never inserted): their segments are anchored on the
# sub-level (T2_ANCHOR) and the picks wait KEY_WAIT frames after it
L3_ANCHOR = {3, 13}
# ---- level 3 without invincibility (round 14 c1, --cheats-off 3) ----
# Health on the way (object 542 health +10, 545 full health, 548 ultra health: the level actors of
# tools/assets/levels.py's grid sections; y is the floor): 61's two +10 beside the first satchel
# charge, 69's full health (only reachable from -z) before its ammo storage.
L3_HEALTH = {
    0: [(4608.5, -431.0, -3682.8, 0.0), (4762.4, -431.0, -3531.5, 0.0)],   # 61: 2 health +10
    1: [(149.8, 0.0, -165.0, 3.1416)],                                     # 69: full health
    5: [(-1280.0, 2.8, 4505.6, 3.1416), (-1280.0, 2.8, 4710.4, 0.0)],      # 64: 2 health +10 on the satchel's island
    9: [(2634.4, -560.0, 2051.6, 1.5708)],                                 # 66: ultra health (the Marsh Totem next)
}
# 69 (the first ammo storage): "armory destroyed." (~440 frames after the charge walk starts) wakes
# an ambush of obj-230 gunners on the balconies over the charge point (instances 7, 8, 12, 13; their
# bursts take 13-42 health per shot, 3 shots a burst: 44 health was gone in 16 frames). Round 12
# stood by the charge point until 600; now the player waits at the exit warp 3778 (a floor below,
# out of their sight) and walks into it when the armory has blown.
L3_69_NOCHEATS = [
    (0, 1103.2, 308.4, -1854.4, 0.0466, 300, {}),        # the charge point, actor 23 (1075, 307, -2458)
    (330, -1492.1, -305.2, -1428.2, 0.1738, 0, {}),      # by the warp 3778, standing
    (480, -1492.1, -305.2, -1428.2, 0.1738, 150, {}),    # warp 3778 -> 61 (the armory blew at ~440)
]


# 64: the second satchel charge (-1331, 3, 4608) lies on an island (region info 14, floor 2.8) in the
# marsh; round 12 stood in the water beside it (region info 12: 75 health every 15 game frames,
# the run's first death). Now from the island's +x side, through its third health +10 (-1229, 4608).
L3_64_NOCHEATS = [
    (0, -1150.0, 2.8, 4608.0, 1.5708, 25, {}),          # satchel charge (-1331, 3, 4608), from +x (a longer walk leaves the island)
    (150, -1820.5, 614.3, 2531.9, -1.2796, 150, {}),    # warp 3780 -> 70
]
# 70 and 71 (the other two ammo storages): the same ambush when the armory blows (70: four gunners
# woke 4 frames after "armory destroyed.", 100 health in 36 frames), the same way out
L3_70_NOCHEATS = [
    (0, 1629.3, -921.4, -524.6, -0.0187, 200, {}),      # the charge point, actor 37 (1638, -922, -992)
    (330, -887.1, -614.2, 1300.7, 0.1285, 0, {}),       # by the warp 3779, standing
    (460, -887.1, -614.2, 1300.7, 0.1285, 150, {}),     # warp 3779 -> 64 (the armory blew at ~420)
]
L3_71_NOCHEATS = [
    (0, 1113.3, -1533.8, -1090.6, 1.1009, 200, {}),     # the charge point, actor 32 (833, -1536, -1233)
    (330, -800.8, -919.4, 191.7, 0.1449, 0, {}),        # by the warp 3781, standing
    (480, -800.8, -919.4, 191.7, 0.1449, 100, {}),      # warp 3781 -> 67
]
L3_NOCHEATS = {1: ("69 ", L3_69_NOCHEATS), 5: ("64 ", L3_64_NOCHEATS), 6: ("70 ", L3_70_NOCHEATS),
               11: ("71 ", L3_71_NOCHEATS)}


def l3_segments(nocheats):
    """playthrough.L3_SEGMENTS, with L3_NOCHEATS's steps when level 3 has no invincibility."""
    if not nocheats:
        return pt.L3_SEGMENTS
    segs = list(pt.L3_SEGMENTS)
    for i, (prefix, steps) in L3_NOCHEATS.items():
        name, rel, _ = segs[i]
        assert name.startswith(prefix)
        segs[i] = (name, rel, steps)
    return segs


# ---- level 4 without invincibility (round 14 c1, --cheats-off 4) ----
# Health on the way (as L3_HEALTH): 100's full health by the level key, 105's full health on its
# second visit and 103's ultra health by the level exit 7400 (the Cave Totem, then the Blind One).
L4_HEALTH = {
    2: [(1075.2, -2451.3, 3123.2, 0.0)],                                   # 100: full health
    12: [(-5404.5, -2150.5, 216.8, 1.5708)],                               # 105 again: full health
    13: [(-9594.8, -3684.6, -157.0, 1.5708)],                              # 103 again: ultra health
}

L4_TOTEM_WEAPON_BEFORE = 600   # the scorpion launcher this long before the Cave Totem's fire window

L4_KEYS = {
    0: [(6758.4, -614.4, 2304.0, 0.0)],                                  # 98: Primagen key 4 (4363)
    9: [(-8652.4, -3634.9, -149.1, 0.0)],                                # 103: level 6 key (4350)
    10: [(-5429.1, -2764.8, -3174.4, 0.0)],                              # 105: level 6 key
}
# round 13 b1: 103's key sat at index 12 (105's second visit: never picked); now at 103's first
# visit, anchored on 103
L4_ANCHOR = {9}
KEY_WAIT = 150


# Retries inside a segment (no shift): (segment index, dt, x, y, z, approach angle). In the
# continuous run 135's first crypt key was missed once (the walk-in was pushed aside)
# The City Totem (110): playthrough.CITY_TOTEM_SPOT, this long after 110 is reached (round 13 b2)
CITY_TOTEM_TP_AFTER = 150
L2_RETRY = [(12, 5950, -714.0, 0.0, 488.0, 0.0)]


# Longer fights (segment index, step index, extra frames): the step's fire window grows and the
# rest of the run moves. The fights of level 2 depend on the state the level is entered with
# (here: after level 1 and the hub, not a new game): in one run the zombies of 133 died ~2,000
# frames later than in --level 2 and the route left before the sister woke.
L2_STRETCH = [(6, 4, 3000), (9, 3, 2000), (12, 15, 2000)]   # (6, 4): sister 1 s window (round 13 b2: after the zombies)
# Round 13 C28: level 2 WITHOUT invincibility (--cheats-off 2) keeps round 12's sister-1 steps (the
# sister aimed from the start, b5's measured route): b2's zombies-one-by-one aim stands the player
# among them for 4,500 frames (200 damage in 133, a death, and the run desyncs from there)
L2_133_NOCHEATS = [
    (0, -1504.0, -4761.6, -972.0, -1.5708, 0, {"trig": 1, "fire": 1300, "aim": "enemy"}),
    (1400, -847.0, -4761.6, 1574.0, 3.1416, 0, {"trig": 1, "fire": 6500, "aim": "enemy:20"}),
    (8000, 827.5, -4761.6, -1627.5, -2.8692, 150, {}),
]
L2_STRETCH_NOCHEATS = [(6, 1, 3000), (9, 3, 2000), (12, 15, 2000)]


def l2_segments(nocheats):
    """playthrough.L2_SEGMENTS, with round 12's sister-1 steps when level 2 has no invincibility."""
    if not nocheats:
        return pt.L2_SEGMENTS, L2_STRETCH
    segs = list(pt.L2_SEGMENTS)
    name, rel, steps = segs[6]
    assert name.startswith("133 ")
    segs[6] = (name, rel, L2_133_NOCHEATS)
    return segs, L2_STRETCH_NOCHEATS


def stretch(segments, extra):
    segs = []
    for i, (name, rel, steps) in enumerate(segments):
        steps = [list(st) for st in steps]
        for si, sj, more in extra:
            if si != i:
                continue
            o = dict(steps[sj][6])
            o["fire"] = o.get("fire", 0) + more
            steps[sj][6] = o
            for k in range(sj + 1, len(steps)):
                steps[k][0] += more
        segs.append((name, rel, [tuple(st) for st in steps], sum(m for si, sj, m in extra if si == i)))
    return segs


def level_segments(r, segments, start, keys, retry=(), extra=(), anchor=(), back=(), detours=None):
    """A 7-tuple segment list from `start` with keys inserted; returns the frames added.
    anchor: the segment indices whose key picks re-anchor the schedule clock when the segment's level
    is reached (T2_ANCHOR) and wait KEY_WAIT frames, so that a pick-up waits for the level when its
    warp loads later than the route assumed (round 13 b1: 62, 68, 103).
    back: (segment index, dt, x, y, z, rotY) teleports inside a segment (round 13 b5).
    detours: {segment index: level} a talisman chamber trip (talisman()) at the segment's start,
    after its keys (round 13 b3)."""
    assert all(0 <= i < len(segments) for i in keys), "a key list past the last segment"
    added = 0
    starts = {}
    out = []
    trips = []
    for i, (name, rel, steps, more) in enumerate(stretch(segments, extra)):
        pre = []
        if i in keys:
            if i in anchor:
                r.anchors.append((start + rel + added, int(name.split()[0])))
                added += KEY_WAIT
            for k in keys[i]:
                pre.append((rel + added, k))
                added += KEY_STEP
        if detours and i in detours:
            trips.append((rel + added, detours[i]))
            added += TALISMAN_TRIP
        out.append((name, rel + added, steps))
        starts[i] = rel + added
        added += more
        for f, (x, y, z, ang) in pre:
            r.pick(start + f, x, y, z, ang)
    for f, lv in trips:
        talisman(r, start + f, lv)
    r.steps(out, start)
    for i, dt, x, y, z, ang in retry:
        r.pick(start + starts[i] + dt, x, y, z, ang)
    for i, dt, x, y, z, ry in back:
        r.tp(start + starts[i] + dt, x, y, z, ry)
    return added


# ---- the talisman chambers (round 13 b3; PLAYTHROUGH.md section 15) ----
# Each talisman level (73-77) is a chamber behind a portal N001 of its level: an action by the
# pedestal (object 274/310/191/226/298, intelligence type 10, auto-go radius 512, collision
# radius 61) needs the level's talisman piece (items 4400-4404, CActionIntelligence
# .m_GoPickupNeeded; one lies in a sub-level of the same level); walking into it with the piece
# plays a cinema (~1,650 frames), opens the door (actor 8, kind 6) to the talisman's dais and the
# talisman (items 4380-4384) is picked up there. The chamber's exit N002 (by its entrance) leads
# back to the portal.
# level: (segment index, the piece (x, y, z, approach angle, distance) [y the floor; the approach
#         from a region next to the piece's], the portal pad's approach (x, y, z, rotY)
#         [a neighbouring region's centre, facing the pad], the chamber level)
TALISMAN = {
    2: (13, (1230.2, 409.6, -798.7, -2.2218, 90.0), (-84.8, 436.0, -192.9, 2.9878), 73),    # 132: piece 4402, portal 2001 (Leap of Faith 4382)
    3: (9, (-2033.5, -307.2, -1457.5, -2.2721, 42.0), (-4139.1, -285.4, -2245.3, 3.0067), 74),  # 66: piece 4400 (on a ledge), portal 3001 (Breath of Life 4380)
}
# Not in the route: level 4 (piece 4404 in 101, portal 4001 in 99: the route passes 99 before
# 101), level 5 (piece 4403 and portal 5001 both in 88), level 6 (piece 4401 in 122, portal 6001
# in 121, passed before 122); `tools/port/talisman.py` plays each chamber on its own.
# The chamber, from the frame it is reached (the T2_ANCHOR frame, TALISMAN_ANCHOR after the
# portal's walk starts; it is reached ~160 frames into the walk): (button, from, to); the player
# stands at (0, 0, -768) facing +z
CHAMBER = [("CU", 300, 392),     # into the pedestal's action: the piece placed, the cinema
           ("CU", 2150, 2550),   # through the opened door (z 467) to the dais (stops at x -62, z 530)
           ("CR", 2600, 2609),   # a step right
           ("CU", 2620, 2950),   # onto the dais: the talisman
           ("CD", 3050, 3550)]   # back to the exit N002 (z -830): the portal's level again
TALISMAN_ANCHOR = 500
TALISMAN_TRIP = KEY_STEP + TALISMAN_ANCHOR + 3550 + 600


def talisman(r, t, lv):
    """The piece of level lv's talisman, its portal, the chamber and back (TALISMAN_TRIP frames)."""
    seg, (px, py, pz, pa, pd), (x, y, z, ry), chamber = TALISMAN[lv]
    r.note(t, "%d: the talisman piece, the portal %d001 -> %d (the chamber)" % (lv, lv, chamber))
    r.pick(t, px, py, pz, pa, pd)
    t += KEY_STEP
    r.tp(t, x, y, z, ry, 40)
    c = t + TALISMAN_ANCHOR
    r.anchors.append((c, chamber))
    r.note(c, "%d the talisman chamber: the pedestal, the door, the talisman, the exit %d002" % (chamber, lv))
    for b, f0, f1 in CHAMBER:
        r.inputs.append("%s@%d..%d" % (b, c + f0, c + f1))


# ---- the hub between levels ----
# The keys go in by walking over the key slots around the hub's central pads (actions of type
# 0x648 needing the level key, item 4310 ... 4350): when a pad's slots are all filled its link
# group fires "access to level N granted." and the pad (a door, object 426) opens the way to
# the level's portal. Teleports below land in the open plaza (no door crossed); the walks go
# through the doors. The Primagen key slots (around the centre) take the Primagen keys.
PAD = {   # level: [(teleport x, y, z, rotY, walk), ...] legs; the last one reaches the portal
    2: [(150.0, 0.0, 0.0, -0.9270, 240), (546.4, -16.3, -318.1, -0.8180, 530)],
    3: [(-253.0, 0.0, -325.4, 2.0852, 250), (-253.0, 0.0, -325.4, 2.0852, 560)],
    4: [(381.3, 0.0, -5.7, -3.0752, 250), (1711.7, -412.4, 968.4, -0.9589, 300)],   # the last leg: a teleport past the open door (no straight walk found)
    # round 13 b1: one walk from the plaza over the three slots, through the door that their keys
    # open and down the way to the portal (5: rotY 2.03 misses the slots, 2.16 stops 200 short)
    5: [(-200.0, 0.0, 128.0, 2.0800, 700)],
    6: [(-12.0, 0.0, 300.0, 3.1416, 800)],
}
CINEMA_AFTER = {2: 260, 3: 510, 4: 120,     # last leg start -> the level's first intro cinema
                5: 420, 6: 470}             # 5, 6: -> the portal's warp (the leg start + 10 + these)
OLD_CINEMA = {2: 13420, 3: 13410, 4: 13420}  # the same frame in the level's own --level N run
PRIMAGEN_SLOTS = {1: (71.0, 0.0, 183.0), 2: (133.0, 0.0, 38.0), 3: (37.0, 0.0, -85.0),
                  4: (-119.0, 0.0, -71.0), 5: (-86.0, 0.0, 203.0), 6: (-185.0, 0.0, 73.0)}
# round 13 b1: the walk for Primagen key K starts at slot PRIMAGEN_FROM[K]'s outside stand point. A
# walk straight in at slot 2 or 3 is deflected round them (the plaza's centre: no region inside the
# ring); the walk at slot 1 goes on to slot 2, and once slot 2 holds its key, the walk at slot 2
# goes on to slot 3 (a new game with the six keys given: every slot filled; until round 12 keys
# 2 and 3 never went in, so the hub portal could not open)
PRIMAGEN_FROM = {1: 1, 2: 1, 3: 2, 4: 4, 5: 5, 6: 6}
# keys 2 and 3 in the continuous run (slot 1 already full, the walk above stops at (147, 128)): from
# the previous slot's pad along the ring
PRIMAGEN_ALONG = {2: (71.0, 0.0, 183.0, -0.4040), 3: (133.0, 0.0, 38.0, 0.6620)}
SAVE_PORTAL = (-977.9, -221.0, -2035.3, 1.4423)   # the hub's save portal (warp 999 -> level 97)
SAVE_MENU = [(740, "A"), (890, "A"), (1090, "A"), (1240, "DD"), (1340, "A"), (1490, "A")]
# a later save overwrites note 1: its "overwrite?" dialog opens with the cursor on NO
SAVE_MENU_AGAIN = [(740, "A"), (890, "A"), (990, "DU"), (1090, "A"), (1290, "A"), (1440, "DD"), (1540, "A"), (1690, "A")]
SAVE_TIME = 2000


# The key slots filled in each hub visit (after level N): every door as soon as its keys are in
# hand (level 3s keys lie in level 1, level 4s in level 2). Round 13 b2: an opened door stays
# open on later visits (the fired events are replayed when a level loads); round 12 opened each
# door in the visit that used it because a used key slot blocked the walk on a later visit.
GRANT = {1: (2, 3), 2: (4,), 3: ()}


def hub(r, arrival, grant=(), primagen=(), save=True, go=None):
    """The hub from `arrival`: pads `grant` (their keys in), Primagen key slots, a save, then
    the portal of level `go`; returns (the frame the level's first cinema starts, or the end)."""
    t = arrival + 150
    if save:
        r.note(t, "60 the hub: the save portal (warp 999 -> 97, Adon), SAVE, GO BACK")
        x, y, z, ry = SAVE_PORTAL
        r.tp(t, x, y, z, ry, 150)
        for dt, b in (SAVE_MENU if not r.saves_at else SAVE_MENU_AGAIN):
            r.inputs.append("%s@%d..%d" % (b, t + dt, t + dt + 6))
        r.saves_at.append(t)
        t += SAVE_TIME
    for lv in grant:
        x, y, z, ry, walk = PAD[lv][0]
        r.note(t, "60 the hub: the level %d key slots (\"access to level %d granted.\")" % (lv, lv))
        r.tp(t, x, y, z, ry, walk)
        t += 300
    for k in primagen:
        r.note(t, "60 the hub: Primagen key %d slot" % k)
        if k in PRIMAGEN_ALONG:   # from the previous slot along the ring
            x, y, z, ry = PRIMAGEN_ALONG[k]
            r.tp(t, x, y, z, ry, 40)
        else:
            x, y, z = PRIMAGEN_SLOTS[PRIMAGEN_FROM[k]]
            r.pick(t, x, y, z, math.atan2(x, z))   # from the outside of the ring
        t += 150
    if go is None:
        return t
    r.note(t, "60 the hub: through the level %d door to the portal %d000" % (go, go))
    legs = PAD[go]
    for i, (x, y, z, ry, walk) in enumerate(legs):
        r.tp(t, x, y, z, ry, walk)
        if i < len(legs) - 1:
            t += walk + 20
    return t + CINEMA_AFTER[go]


def main_all(out, upto=6, extras=True, hubs_in=None, no_cheats=(), extra_env=(), health=True, cheats_on=(),
             talismans=()):
    """hubs: the measured hub arrivals {after level: frame} (HUB_AT). no_cheats: the levels played
    without T2_CHEATS invincibility (each from its hub visit before it); extra_env: more env lines.
    talismans: the levels whose chamber trip is made (TALISMAN: 2, 3; each moves the later hub
    arrivals by TALISMAN_TRIP)."""
    hubs = dict(HUB_AT)
    for lv in talismans:
        for k in hubs:
            if k >= lv:
                hubs[k] += TALISMAN_TRIP
    hubs.update(hubs_in or {})
    det = {lv: {TALISMAN[lv][0]: lv} for lv in talismans}
    r = Route()
    r.no_cheats = set(no_cheats)
    r.cheats_on = set(cheats_on)
    r.extra_env = list(extra_env)
    for lv in (2, 3, 4, 5):
        r.level_start[lv] = hubs[lv - 1]
    r.saves_at = []
    # the health pickups only in the levels played without invincibility (with it the route stays
    # round 12's: the picks move every later step, and the Primagen's lair wait depends on the timing)
    arrival, added = level1(r, health=health and 1 in r.no_cheats)
    r.note(hubs[1], "60 the hub (after level 1)")
    r.anchors.append((hubs[1], 60))
    if upto < 2:
        hub(r, hubs[1], grant=GRANT[1], primagen=(1,))
        return r.write(out)
    c = hub(r, hubs[1], grant=GRANT[1], primagen=(1,), go=2)
    shift = c - OLD_CINEMA[2]
    r.anchors.append((c, 34))
    r.note(c, "34.. the River of Souls intro cinemas (level 2 route shifted by %d)" % shift)
    l2s = pt.L2_START + shift
    h2 = health and 2 in r.no_cheats
    l2segs, l2stretch = l2_segments(2 in r.no_cheats)
    a2 = level_segments(r, l2segs, l2s, merged(L2_KEYS, L2_HEALTH if h2 else {}) if extras else {},
                        L2_RETRY + L2_RETRY_B5 + (L2_HEALTH_LATE if h2 else []), l2stretch,
                        back=L2_RETURN_LATE if h2 else (), detours=det.get(2))
    t_arr = pt.L2_TOTEM_ARRIVAL + shift + a2
    r.note(t_arr, "110 the City Totem: fire taps (T2_FACE_NEAREST aims)")
    if 110 in r.cheats_on:
        r.cheat_windows.append((t_arr - 600, hubs[2]))
    r.fire.append((t_arr, t_arr + 7200))
    r.anchors.append((t_arr, 110))   # the fire window starts when 110 is reached
    # Round 13 b2: down from the arrival ledge (642 units from the totem, the attackers 450-1,150
    # away: 4-5 kills of 8 in 120 s) to the south side of the totem, where they come to the player
    r.tp(t_arr + CITY_TOTEM_TP_AFTER, *pt.CITY_TOTEM_SPOT)
    r.face_levels.append(110)
    r.note(hubs[2], "60 the hub (after level 2)")
    r.anchors.append((hubs[2], 60))
    if upto < 3:
        hub(r, hubs[2], grant=GRANT[2], primagen=(2,))
        return r.write(out)
    c = hub(r, hubs[2], grant=GRANT[2], primagen=(2,), go=3)
    shift = c - OLD_CINEMA[3]
    r.anchors.append((c, 12))
    r.note(c, "12.. the Death Marshes intro cinemas (level 3 route shifted by %d)" % shift)
    l3s = pt.L3_START + shift
    h3 = health and 3 in r.no_cheats
    a3 = level_segments(r, l3_segments(3 in r.no_cheats), l3s,
                        merged(L3_KEYS, L3_HEALTH if h3 else {}) if extras else {}, anchor=L3_ANCHOR, detours=det.get(3))
    a3x = sum(len(v) for v in L3_HEALTH.values()) * KEY_STEP if h3 and extras else 0
    for k in hubs:   # round 14 c1: the hub anchors after level 3 move with its health picks
        if k >= 3:
            hubs[k] += a3x
            if k + 1 in r.level_start:
                r.level_start[k + 1] = hubs[k]
    rel, length, lv = pt.L3_TOTEM
    r.note(l3s + rel + a3, "111 the Marsh Totem: fire taps")
    r.fire.append((l3s + rel + a3, l3s + rel + a3 + length))
    r.face_levels.append(lv)
    r.note(hubs[3], "60 the hub (after level 3)")
    r.anchors.append((hubs[3], 60))
    if upto < 4:
        hub(r, hubs[3], grant=GRANT[3], primagen=(3,))
        return r.write(out)
    c = hub(r, hubs[3], grant=GRANT[3], primagen=(3,), go=4)
    shift = c - OLD_CINEMA[4]
    r.anchors.append((c, 18))
    r.note(c, "18.. the Lair of the Blind intro cinemas (level 4 route shifted by %d)" % shift)
    l4s = pt.L4_START + shift
    h4 = health and 4 in r.no_cheats
    a4 = level_segments(r, pt.L4_SEGMENTS, l4s, merged(L4_KEYS, L4_HEALTH if h4 else {}) if extras else {},
                        anchor=L4_ANCHOR)
    a4x = sum(len(v) for v in L4_HEALTH.values()) * KEY_STEP if h4 and extras else 0
    for k in hubs:   # round 14 c1: the hub anchors after level 4 move with its health picks
        if k >= 4:
            hubs[k] += a4x
            if k + 1 in r.level_start:
                r.level_start[k + 1] = hubs[k]
    rel, length, lv = pt.L4_TOTEM
    r.note(l4s + rel + a4, "112 the Cave Totem: fire taps")
    r.fire.append((l4s + rel + a4, l4s + rel + a4 + length))
    if 4 in r.no_cheats:
        # round 14 c1: the scorpion launcher (16) for the Cave Totem and the Blind One, the plasma
        # rifle again from the hub. T2_WEAPON is applied when a level loads (T2_GIVE_WEAPONS), so
        # the switch is scheduled in 103, before the totem's level loads. The plasma rifle: the
        # totem 200 damage and a life lost, the Blind One never hurt (hp 12,800/12,800 at the end);
        # the scorpion launcher: the totem 88 damage, the Blind One dead in 5,200 frames
        r.weapon.append((l4s + rel + a4 - L4_TOTEM_WEAPON_BEFORE, "16"))
        r.weapon.append((hubs[4] - 200, "10"))
    r.face_levels.append(lv)
    rel, length, (x, y, z, ry), lv = pt.L4_BOSS
    f = l4s + rel + a4
    r.note(f, "1 the Blind One: fire held, T2_AIM_NODE=boss@1")
    r.inputs.append("Z@%d..%d" % (f + 10, f + length))
    r.tp(f, x, y, z, ry)
    r.aim_node = "boss@1/2/3/4"   # the Blind One 1, the Mantid Queen 2, the Mother 3, the Primagen 4
    r.note(hubs[4], "60 the hub (after level 4)")
    r.anchors.append((hubs[4], 60))
    end = hub(r, hubs[4], primagen=(4,), go=5 if upto >= 5 else None)
    for lvl, hook in sorted(LEVEL_HOOKS.items()):
        if lvl <= upto:
            end = hook(r, end)
    return r.write(out)


# Measured hub arrivals (the first "level 60" T2_STATE_LOG line after each level's totem/boss)
HUB_AT = {1: 23780, 2: 84124, 3: 108124, 4: 140500}

# ---- levels 5 and 6 (round 12 C27; round 13 b1: their keys, doors and items, the Primagen's portal) ----
# The hub after level 4: Primagen key 4 in its slot, then one walk over the level-5 key slots (the
# three level-5 keys of 61, 62 and 68), through the door they open and into the portal 5000. Level 5
# (routes56.py, with the detour to 92 for Primagen key 5), the Hive Totem, the Queen, the hub: a save,
# Primagen key 5 in its slot, one walk over the six level-6 key slots (100, 103, 105 of level 4; 84,
# 87, 89 of level 5), through the door and into the portal 6000. Level 6 picks up its own 16 crystal
# ion capacitors and two red laser cells (routes56.L6_ITEMS) and Primagen key 6 (114). After the
# Mother, the hub: Primagen key 6 in the sixth slot fires the hub portal cinema (level 43), which
# ends in the Primagen's lair (the warp 7602's place): no T2_WARP_TO, no T2_GIVE_ITEMS.
WARP_TO_START = {5: r56.L5_START - (r56.HUB_ARRIVAL + 240), 6: r56.L6_START - (r56.HUB_ARRIVAL + 270)}
LOAD_EARLY = 200        # each level is loaded ~200 frames before its route's start
L5_HUB_AFTER = 25800    # the hub after the Mantid Queen, from L5_START (--level 5: 43,500 - 17,700)
L5_TOTEM_AT = 14300     # 113 (the Hive Totem) reached, from L5_START
L5_QUEEN_AT = 15900     # level 2 (the Queen) reached
# After the Mother: her fight's length varies (her death 17,100 frames after her arena is loaded in
# --level 6, 21,500 in the continuous run), and where her death leads too (--level 6: 114 by her
# warp 6999, then its pad 7601 to the hub; the continuous run: the hub straight). So the steps
# after her hang on anchors: "114/60" at L6_BACK_114 after her arena (she dying earlier only means
# a wait, the anchor fires from 3,000 before), then "60" (in the hub straight, it fires at once and
# skips the 114 teleport).
L6_BACK_114 = 26000
L6_EXIT_AFTER = 500     # the exit teleport, after 114 is reached again
L6_HUB_AFTER = 800      # the hub (6999's place: the level-6 corridor)
L6_WALK_AFTER = 200     # the walk back down the corridor to the plaza, after the hub
L6_SLOT_AFTER = 1100    # Primagen key 6 into its slot, after the hub (round 13 C28: 650 -> 1100; with b2's door fix and
                        # the Leap of Faith trip the Primagen rested at z 2,923 after a 650 hop, the known no-end spot of
                        # his stage-3 lair wait; 300 and 1,100 both end it)
L6_CINEMA = 1150        # the slot -> the hub portal cinema (43) -> the Primagen's lair (level 4)
L6_HUB_END = 49500      # the hub after the credits, from the Primagen's lair (the hop's 51,000 - 1,500)
PRIMAGEN_KEY = "4@%d"   # the anchor on his lair


def level5(r, t):
    """t: the frame the hub portal 5000 warps (hub(..., go=5))."""
    s = t + WARP_TO_START[5]
    segs, add = r56.l5_with_primagen_key()
    r.anchors.append((s - LOAD_EARLY, 124))
    r.note(s - LOAD_EARLY, "124.. the Hive of the Mantids (level 5 route from %d, + the 92 detour)" % s)
    r.weapon.append((t, "9"))       # the shredder (the cocoons; a1's route)
    r.fill.append((t, "100"))
    r.steps(segs, s)
    rel, length, lv = r56.L5_TOTEM
    r.anchors.append((s + L5_TOTEM_AT + add, lv))
    r.note(s + rel + add, "113 the Hive Totem: fire taps (T2_FACE_NEAREST aims)")
    r.fire.append((s + rel + add, s + rel + add + length))
    r.face_levels.append(lv)
    rel, length = r56.L5_BOSS
    r.anchors.append((s + L5_QUEEN_AT + add, 2))
    r.note(s + rel + add, "2 the Mantid Queen: fire held, T2_AIM_NODE=boss@2, T2_BOSS_FRONT=450,200")
    r.inputs.append("Z@%d..%d" % (s + rel + add, s + rel + add + length))
    r.boss_front.append((s + rel + add - 100, "450,200"))
    hub_at = s + L5_HUB_AFTER + add
    r.level_start[6] = hub_at
    r.note(hub_at, "60 the hub (after level 5)")
    r.anchors.append((hub_at, 60))
    return hub(r, hub_at, primagen=(5,), go=6)   # a save, Primagen key 5, the level-6 door and portal


def level6(r, t):
    """t: the frame the hub portal 6000 warps (hub(..., go=6))."""
    s = t + WARP_TO_START[6]
    segs, add = r56.with_items(r56.L6_SEGMENTS, r56.L6_ITEMS)
    r.anchors.append((s - LOAD_EARLY, 114))
    r.note(s - LOAD_EARLY, "114.. the Lightship (level 6 route from %d, its items picked up: +%d)" % (s, add))
    r.weapon.append((t, "10"))
    r.fill.append((t, "600"))
    r.steps(segs, s)
    m = s + r56.L6_MOTHER + add
    r.anchors.append((m, 3))
    r.note(m, "3 the Mother: fire held, T2_AIM_NODE=boss@3, strafing with steps back")
    r.inputs += ["Z@%d..%d" % (m + 800, m + L6_BACK_114), "CL@%d..%d" % (m + 5800, m + 15000)]
    r.inputs += ["CD@%d..%d" % (f, f + 250) for f in range(m + 5800, s + r56.L6_MOTHER_END + add, 600)]
    back = m + L6_BACK_114
    r.anchors.append((back, "114/60"))
    _, x, y, z, ry = r56.L6_EXIT
    r.note(back, "114 again after the Mother (her death warps 6999), or the hub straight (anchor 114/60)")
    r.note(back + L6_EXIT_AFTER, "114: the pad 7601 by the start -> the hub")
    r.tp(back + L6_EXIT_AFTER, x, y, z, ry)
    r.inputs.append("CU@%d..%d" % (back + L6_EXIT_AFTER + 10, back + L6_EXIT_AFTER + 130))
    h = back + L6_HUB_AFTER
    r.note(h, "60 the hub (after level 6: 6999's place in the level-6 corridor)")
    r.anchors.append((h, 60))
    r.note(h + L6_WALK_AFTER, "60 the hub: back down the corridor through the level-6 door to the plaza")
    r.inputs.append("CU@%d..%d" % (h + L6_WALK_AFTER, h + L6_SLOT_AFTER - 50))
    f = h + L6_SLOT_AFTER
    x, y, z = PRIMAGEN_SLOTS[PRIMAGEN_FROM[6]]
    r.note(f, "60 the hub: Primagen key 6 slot: the six keys in: the hub portal cinema (43) -> the Primagen")
    r.pick(f, x, y, z, math.atan2(x, z))
    lair = f + L6_CINEMA
    # FALLBACK (DEBUG-ASSISTED): if the six keys are not all in (no cinema), the old hop 7602; after
    # the cinema it only puts the player back on the lair's start, where he already stands
    r.warps.append((lair + 150, 7602))
    r.anchors.append((lair, 4))
    r.note(lair, "4 the Primagen's lair: fire held, T2_BOSS_FRONT=450,600,2.5,1")
    # the spot cycles (",K,1": another one each 600 frames of a lair wait that does not end)
    r.boss_front.append((lair - 100, "450,600,2.5,1"))
    r.inputs.append("Z@%d..%d" % (lair + 200, lair + L6_HUB_END))
    end = lair + L6_HUB_END
    r.note(end - 21000, "the Primagen dead (~30,000 after his lair): the ending cinema, then the credits")
    r.note(end, "60 the hub (after the credits)")
    r.end = end + 1500
    return r.end


LEVEL_HOOKS = {5: level5, 6: level6}


def cli(argv=None):
    """The command line (also `playthrough.py OUT --all [these options]`, round 13 C28)."""
    global L6_SLOT_AFTER
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--upto", type=int, default=6)
    ap.add_argument("--no-extras", action="store_true")
    ap.add_argument("--hub", action="append", default=[], help="N=FRAME: the hub arrival after level N")
    ap.add_argument("--cheats-off", default="", help="L1,L2,...: the levels played without T2_CHEATS invincibility (round 13 b5)")
    ap.add_argument("--env", action="append", default=[], help="K=V: one more line for teleport.env (e.g. T2_HURT_LOG=1)")
    ap.add_argument("--cheats-on", default="", help="110: the City Totem invincible even in a --cheats-off level (round 13 b5)")
    ap.add_argument("--no-health", action="store_true", help="skip the health pickups (round 13 b5)")
    ap.add_argument("--talismans", nargs="?", const="2,3", default="",
                    help="the talisman chambers of these levels (default 2,3; round 13 b3)")
    ap.add_argument("--slot", type=int, default=L6_SLOT_AFTER, help="Primagen key 6 into its slot, frames after the hub (level 6)")
    a = ap.parse_args(argv)
    L6_SLOT_AFTER = a.slot
    main_all(a.out, a.upto, not a.no_extras, {int(h.split("=")[0]): int(h.split("=")[1]) for h in a.hub},
             [int(x) for x in a.cheats_off.split(",") if x], a.env, not a.no_health,
             [int(x) for x in a.cheats_on.split(",") if x], [int(v) for v in a.talismans.split(",") if v])


if __name__ == "__main__":
    cli()
