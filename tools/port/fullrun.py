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
    3: [(-4761.5, 307.2, -1074.0, 0.0)],                                 # 62: level 5 key
    4: [(-102.4, 307.6, 870.4, 0.0)],                                    # 63: Primagen key 3 (4362)
    15: [(-2150.5, 0.0, -1381.4, 0.0)],                                  # 68: level 5 key
}
L4_KEYS = {
    0: [(6758.4, -614.4, 2304.0, 0.0)],                                  # 98: Primagen key 4 (4363)
    12: [(-8652.4, -3635.2, -149.1, 0.0)],                               # 103: level 6 key (4350)
    10: [(-5429.1, -2764.8, -3174.4, 0.0)],                              # 105: level 6 key
}


# Retries inside a segment (no shift): (segment index, dt, x, y, z, approach angle). In the
# continuous run 135's first crypt key was missed once (the walk-in was pushed aside)
L2_RETRY = [(12, 5950, -714.0, 0.0, 488.0, 0.0)]


# Longer fights (segment index, step index, extra frames): the step's fire window grows and the
# rest of the run moves. The fights of level 2 depend on the state the level is entered with
# (here: after level 1 and the hub, not a new game): in one run the zombies of 133 died ~2,000
# frames later than in --level 2 and the route left before the sister woke.
L2_STRETCH = [(6, 1, 3000), (9, 3, 2000), (12, 15, 2000)]


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


def level_segments(r, segments, start, keys, retry=(), extra=(), anchor=False, back=()):
    """A 7-tuple segment list from `start` with keys inserted; returns the frames added.
    anchor: re-anchor the schedule clock when a key segment's level is reached (T2_ANCHOR), so
    that the key pick-up waits for the level when its warp loads later than the route assumed."""
    added = 0
    starts = {}
    out = []
    for i, (name, rel, steps, more) in enumerate(stretch(segments, extra)):
        pre = []
        if i in keys:
            if anchor:
                r.anchors.append((start + rel + added, int(name.split()[0])))
            for k in keys[i]:
                pre.append((rel + added, k))
                added += KEY_STEP
        out.append((name, rel + added, steps))
        starts[i] = rel + added
        added += more
        for f, (x, y, z, ang) in pre:
            r.pick(start + f, x, y, z, ang)
    r.steps(out, start)
    for i, dt, x, y, z, ang in retry:
        r.pick(start + starts[i] + dt, x, y, z, ang)
    for i, dt, x, y, z, ry in back:
        r.tp(start + starts[i] + dt, x, y, z, ry)
    return added


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
}
CINEMA_AFTER = {2: 260, 3: 510, 4: 120}     # last leg start -> the level's first intro cinema
OLD_CINEMA = {2: 13420, 3: 13410, 4: 13420}  # the same frame in the level's own --level N run
PRIMAGEN_SLOTS = {1: (71.0, 0.0, 183.0), 2: (133.0, 0.0, 38.0), 3: (37.0, 0.0, -85.0),
                  4: (-119.0, 0.0, -71.0), 5: (-86.0, 0.0, 203.0), 6: (-185.0, 0.0, 73.0)}
SAVE_PORTAL = (-977.9, -221.0, -2035.3, 1.4423)   # the hub's save portal (warp 999 -> level 97)
SAVE_MENU = [(740, "A"), (890, "A"), (1090, "A"), (1240, "DD"), (1340, "A"), (1490, "A")]
# a later save overwrites note 1: its "overwrite?" dialog opens with the cursor on NO
SAVE_MENU_AGAIN = [(740, "A"), (890, "A"), (990, "DU"), (1090, "A"), (1290, "A"), (1440, "DD"), (1540, "A"), (1690, "A")]
SAVE_TIME = 2000


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
        x, y, z = PRIMAGEN_SLOTS[k]
        r.note(t, "60 the hub: Primagen key %d slot" % k)
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


def main_all(out, upto=6, extras=True, hubs_in=None, no_cheats=(), extra_env=(), health=True, cheats_on=()):
    """hubs: the measured hub arrivals {after level: frame} (HUB_AT). no_cheats: the levels played
    without T2_CHEATS invincibility (each from its hub visit before it); extra_env: more env lines."""
    hubs = dict(HUB_AT); hubs.update(hubs_in or {})
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
        hub(r, hubs[1], grant=(2,), primagen=(1,))
        return r.write(out)
    c = hub(r, hubs[1], grant=(2,), primagen=(1,), go=2)
    shift = c - OLD_CINEMA[2]
    r.anchors.append((c, 34))
    r.note(c, "34.. the River of Souls intro cinemas (level 2 route shifted by %d)" % shift)
    l2s = pt.L2_START + shift
    h2 = health and 2 in r.no_cheats
    a2 = level_segments(r, pt.L2_SEGMENTS, l2s, merged(L2_KEYS, L2_HEALTH if h2 else {}) if extras else {},
                        L2_RETRY + L2_RETRY_B5 + (L2_HEALTH_LATE if h2 else []), L2_STRETCH,
                        back=L2_RETURN_LATE if h2 else ())
    t_arr = pt.L2_TOTEM_ARRIVAL + shift + a2
    r.note(t_arr, "110 the City Totem: fire taps (T2_FACE_NEAREST aims)")
    if 110 in r.cheats_on:
        r.cheat_windows.append((t_arr - 600, hubs[2]))
    r.fire.append((t_arr, t_arr + 7200))
    r.anchors.append((t_arr, 110))   # the fire window starts when 110 is reached
    r.face_levels.append(110)
    r.note(hubs[2], "60 the hub (after level 2)")
    r.anchors.append((hubs[2], 60))
    if upto < 3:
        hub(r, hubs[2], grant=(3,), primagen=(2,))
        return r.write(out)
    c = hub(r, hubs[2], grant=(3,), primagen=(2,), go=3)
    shift = c - OLD_CINEMA[3]
    r.anchors.append((c, 12))
    r.note(c, "12.. the Death Marshes intro cinemas (level 3 route shifted by %d)" % shift)
    l3s = pt.L3_START + shift
    a3 = level_segments(r, pt.L3_SEGMENTS, l3s, L3_KEYS if extras else {})
    rel, length, lv = pt.L3_TOTEM
    r.note(l3s + rel + a3, "111 the Marsh Totem: fire taps")
    r.fire.append((l3s + rel + a3, l3s + rel + a3 + length))
    r.face_levels.append(lv)
    r.note(hubs[3], "60 the hub (after level 3)")
    r.anchors.append((hubs[3], 60))
    if upto < 4:
        hub(r, hubs[3], grant=(4,), primagen=(3,))
        return r.write(out)
    c = hub(r, hubs[3], grant=(4,), primagen=(3,), go=4)
    shift = c - OLD_CINEMA[4]
    r.anchors.append((c, 18))
    r.note(c, "18.. the Lair of the Blind intro cinemas (level 4 route shifted by %d)" % shift)
    l4s = pt.L4_START + shift
    a4 = level_segments(r, pt.L4_SEGMENTS, l4s, L4_KEYS if extras else {})
    rel, length, lv = pt.L4_TOTEM
    r.note(l4s + rel + a4, "112 the Cave Totem: fire taps")
    r.fire.append((l4s + rel + a4, l4s + rel + a4 + length))
    r.face_levels.append(lv)
    rel, length, (x, y, z, ry), lv = pt.L4_BOSS
    f = l4s + rel + a4
    r.note(f, "1 the Blind One: fire held, T2_AIM_NODE=boss@1")
    r.inputs.append("Z@%d..%d" % (f + 10, f + length))
    r.tp(f, x, y, z, ry)
    r.aim_node = "boss@1/2/3/4"   # the Blind One 1, the Mantid Queen 2, the Mother 3, the Primagen 4
    r.note(hubs[4], "60 the hub (after level 4)")
    r.anchors.append((hubs[4], 60))
    end = hub(r, hubs[4], primagen=(4,))
    for lvl, hook in sorted(LEVEL_HOOKS.items()):
        if lvl <= upto:
            end = hook(r, end)
    return r.write(out)


# Measured hub arrivals (the first "level 60" T2_STATE_LOG line after each level's totem/boss)
HUB_AT = {1: 23780, 2: 84124, 3: 108124, 4: 140500}

# ---- levels 5 and 6 (round 12 C27): routes56.py's routes from the hub after level 4 ----
# The level 5 and 6 doors of the hub are not opened (their keys are not all in the route: 62/68's
# level-5 keys, 103's level-6 key, levels 5/6's Primagen keys): the run teleports onto each portal's
# approach in the plaza past the door, as --level 5/6 do (DEBUG-ASSISTED). The Primagen (7602) is a
# T2_WARP_TO hop from the hub (the hub's way to him needs the six Primagen keys).
PORTAL_TO_START = {5: r56.L5_START - (r56.HUB_ARRIVAL + 200), 6: r56.L6_START - (r56.HUB_ARRIVAL + 200)}
LOAD_EARLY = 200        # each level is loaded ~200 frames before its route's start
L5_HUB_AFTER = 25800    # the hub after the Mantid Queen, from L5_START (--level 5: 43,500 - 17,700)
L5_TOTEM_AT = 14300     # 113 (the Hive Totem) reached, from L5_START
L5_QUEEN_AT = 15900     # level 2 (the Queen) reached
# After the Mother: her fight's length varies (her death 17,100 frames after her arena is loaded in
# --level 6, 21,500 in the continuous run), and where her death leads too (--level 6: 114 by her
# warp 6999, then its pad 7601 to the hub; the continuous run: the hub straight). So the steps
# after her hang on anchors: "114/60" at L6_BACK_114 after her arena (she dying earlier only means
# a wait, the anchor fires from 3,000 before), then "60" (in the hub straight, it fires at once and
# skips the 114 teleport). The hop to the Primagen a fixed time after, as in --level 6 (the hub
# 53,000, the hop 54,000, the hub after the credits 105,000).
L6_BACK_114 = 26000
L6_EXIT_AFTER = 500     # the exit teleport, after 114 is reached again
L6_HUB_AFTER = 800      # the hub
L6_HOP_AFTER = 2100     # the hop to the Primagen (1,300 frames after the hub: his stage-3 lair wait then ends
                        # by itself; from 1,000 after it he rested at z 2,924, where no T2_BOSS_FRONT spot ends it)
L6_HUB_END = 51000      # the hub after the credits, from the hop


def to_portal(r, t, lv):
    x, y, z, ry = r56.L5_PORTAL if lv == 5 else r56.L6_PORTAL
    r.note(t, "60 the hub: onto the level %d portal (warp %d000; teleported past its door)" % (lv, lv))
    r.tp(t, x, y, z, ry)
    r.inputs.append("CU@%d..%d" % (t + 10, t + 300))
    return t + PORTAL_TO_START[lv]


def level5(r, t):
    s = to_portal(r, t, 5)
    r.anchors.append((s - LOAD_EARLY, 124))
    r.note(s - LOAD_EARLY, "124.. the Hive of the Mantids (level 5 route from %d)" % s)
    r.weapon.append((t, "9"))       # the shredder (the cocoons; a1's route)
    r.fill.append((t, "100"))
    r.steps(r56.L5_SEGMENTS, s)
    rel, length, lv = r56.L5_TOTEM
    r.anchors.append((s + L5_TOTEM_AT, lv))
    r.note(s + rel, "113 the Hive Totem: fire taps (T2_FACE_NEAREST aims)")
    r.fire.append((s + rel, s + rel + length))
    r.face_levels.append(lv)
    rel, length = r56.L5_BOSS
    r.anchors.append((s + L5_QUEEN_AT, 2))
    r.note(s + rel, "2 the Mantid Queen: fire held, T2_AIM_NODE=boss@2, T2_BOSS_FRONT=450,200")
    r.inputs.append("Z@%d..%d" % (s + rel, s + rel + length))
    r.boss_front.append((s + rel - 100, "450,200"))
    hub_at = s + L5_HUB_AFTER
    r.level_start[6] = hub_at
    r.note(hub_at, "60 the hub (after level 5)")
    r.anchors.append((hub_at, 60))
    return hub(r, hub_at)          # a save; no key goes in


def level6(r, t):
    s = to_portal(r, t, 6)
    r.anchors.append((s - LOAD_EARLY, 114))
    r.note(s - LOAD_EARLY, "114.. the Lightship (level 6 route from %d)" % s)
    r.weapon.append((t, "10"))
    r.fill.append((t, "600"))
    for rel, spec in r56.L6_GIVE:   # DEBUG-ASSISTED: the capacitors and the red laser cells
        r.give.append((s + rel, spec))
    r.steps(r56.L6_SEGMENTS, s)
    m = s + r56.L6_MOTHER
    r.anchors.append((m, 3))
    r.note(m, "3 the Mother: fire held, T2_AIM_NODE=boss@3, strafing with steps back")
    r.inputs += ["Z@%d..%d" % (m + 800, m + L6_BACK_114), "CL@%d..%d" % (m + 5800, m + 15000)]
    r.inputs += ["CD@%d..%d" % (f, f + 250) for f in range(m + 5800, s + r56.L6_MOTHER_END, 600)]
    back = m + L6_BACK_114
    r.anchors.append((back, "114/60"))
    _, x, y, z, ry = r56.L6_EXIT
    r.note(back, "114 again after the Mother (her death warps 6999), or the hub straight (anchor 114/60)")
    r.note(back + L6_EXIT_AFTER, "114: the pad 7601 by the start -> the hub")
    r.tp(back + L6_EXIT_AFTER, x, y, z, ry)
    r.inputs.append("CU@%d..%d" % (back + L6_EXIT_AFTER + 10, back + L6_EXIT_AFTER + 130))
    r.note(back + L6_HUB_AFTER, "60 the hub (after level 6)")
    r.anchors.append((back + L6_HUB_AFTER, 60))
    hop = back + L6_HOP_AFTER
    r.note(hop, "60 -> 4 the Primagen (T2_WARP_TO=7602, DEBUG-ASSISTED): fire held, T2_BOSS_FRONT=450,600,2.5,1")
    r.warps.append((hop, 7602))
    # the spot cycles (",K,1": another one each 600 frames of a lair wait that does not end): the
    # first continuous run's Primagen rested at z 2,924 in stage 3, where 450 beside / 2.5 x 450 in
    # front did not see his eye
    r.boss_front.append((hop, "450,600,2.5,1"))
    r.inputs.append("Z@%d..%d" % (hop + 200, hop + L6_HUB_END))
    end = hop + L6_HUB_END
    r.note(end - 21000, "the Primagen dead (~30,000 after the hop): the ending cinema, then the credits")
    r.note(end, "60 the hub (after the credits)")
    r.end = end + 1500
    return r.end


LEVEL_HOOKS = {5: level5, 6: level6}


if __name__ == "__main__":
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
    ap.add_argument("--hop", type=int, default=L6_HOP_AFTER, help="the hop to the Primagen, frames after the hub (level 6)")
    a = ap.parse_args()
    L6_HOP_AFTER = a.hop
    main_all(a.out, a.upto, not a.no_extras, {int(h.split("=")[0]): int(h.split("=")[1]) for h in a.hub},
             [int(x) for x in a.cheats_off.split(",") if x], a.env, not a.no_health,
             [int(x) for x in a.cheats_on.split(",") if x])
