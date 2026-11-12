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

Levels 5 and 6 (parts a1/a2 of round 12) append through LEVEL_HOOKS: a function
(start_frame) -> (inputs, teleports, env_windows) like the ones below.
"""
import math, os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import playthrough as pt  # noqa: E402


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
                    elif aim:
                        self.aim_at.append("%d-%d:%.1f,%.1f,%.1f" % ((a, b) + tuple(aim)))

    def write(self, out, env_extra=()):
        for a, b in self.fire:
            for f in range(a, b, pt.FIRE_EVERY):
                self.inputs.append("Z@%d..%d" % (f, f + 6))
        env = ["T2_GIVE_WEAPONS=1", "T2_WEAPON=10", "T2_CHEATS=1", "T2_FILL_AMMO=%d" % self.fill_ammo,
               "T2_FACE_NEAREST=4000@%s" % ",".join(str(l) for l in self.face_levels),
               "T2_FACE_PITCH=1", "T2_FACE_STICKY=1", "T2_BOSS_LOG=1",
               "T2_AIM_AT=\"%s\"" % ";".join(self.aim_at), "T2_AIM_ENEMY=\"%s\"" % ";".join(self.aim_enemy)]
        if self.aim_node:
            env.append("T2_AIM_NODE=%s" % self.aim_node)
        if self.anchors:
            env.append("T2_ANCHOR=\"%s\"" % ";".join("%d@%d" % (l, f) for f, l in sorted(self.anchors)))
        if self.saves:
            env.append("T2_SAVE_PORTAL=%s" % ",".join(str(f) for f in self.saves))
        env += list(env_extra)
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
        last = max(int(t.split(":")[0]) for t in self.tps)
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


def level1(r, start_shift=0, keys=True):
    """Level 1 from the new game: (the frame level 109 is reached, the extra frames added)."""
    added = 0
    for i, (name, base, steps) in enumerate(pt.SEGMENTS):
        f0 = base + start_shift + added
        r.note(f0, name)
        if keys and i in L1_KEYS:
            for x, y, z, ang in L1_KEYS[i]:
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


def level_segments(r, segments, start, keys, retry=(), extra=(), anchor=False):
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


def main_all(out, upto=4, extras=True, hubs_in=None):
    """hubs: the measured hub arrivals {after level: frame} (HUB_AT)."""
    hubs = dict(HUB_AT); hubs.update(hubs_in or {})
    r = Route()
    r.saves_at = []
    arrival, added = level1(r)
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
    a2 = level_segments(r, pt.L2_SEGMENTS, l2s, L2_KEYS if extras else {}, L2_RETRY, L2_STRETCH)
    t_arr = pt.L2_TOTEM_ARRIVAL + shift + a2
    r.note(t_arr, "110 the City Totem: fire taps (T2_FACE_NEAREST aims)")
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
    r.aim_node = "boss@1"
    r.note(hubs[4], "60 the hub (after level 4)")
    r.anchors.append((hubs[4], 60))
    end = hub(r, hubs[4], primagen=(4,))
    for lvl, hook in sorted(LEVEL_HOOKS.items()):
        if lvl <= upto:
            end = hook(r, end)
    return r.write(out)


# Measured hub arrivals (the first "level 60" T2_STATE_LOG line after each level's totem/boss)
HUB_AT = {1: 23780, 2: 84124, 3: 108124, 4: 140500}

# Levels 5 and 6 (round 12 a1/a2): {level: function(route, start_frame) -> end frame}
LEVEL_HOOKS = {}


if __name__ == "__main__":
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--upto", type=int, default=4)
    ap.add_argument("--no-extras", action="store_true")
    ap.add_argument("--hub", action="append", default=[], help="N=FRAME: the hub arrival after level N")
    a = ap.parse_args()
    main_all(a.out, a.upto, not a.no_extras, {int(h.split("=")[0]): int(h.split("=")[1]) for h in a.hub})
