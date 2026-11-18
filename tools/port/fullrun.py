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


STICK_AIM = set()   # round 15 d3: the levels aimed with the stick (--stick-aim)
STICK_FREE_WALK = {5, 6}   # round 18 g5: --stick-aim levels whose route walks keep the script's yaw
# round 18 g5: per route level, the sub-levels where the stick driver prefers the assist's target (T2_AIMD_PRIMARY):
# level 5's pylons (124), the cocoons and embryos (94, 95, 96) and the Queen's node (2); level 6's Mother (3) and the Primagen (4)
STICK_PRIMARY = {5: (124, 94, 95, 96, 2), 6: (3, 4)}
# round 19 h3: on the N64 stream (n64_full()), the boss arenas of the --stick-aim levels where the stick aims only
# at the boss's damage node, led, with Z held on target, unless a candidate comes within the radius
# (aimdrive.py T2_AIMD_BOSS=L:R): the Mantid Queen (2) and the Primagen's lair (4). --stick-boss "" turns it off.
STICK_BOSS = {5: "2:%s" % os.environ.get("T2H3_QR", "150"), 6: "4:%s" % os.environ.get("T2H3_LR", "350")}
STICK_BOSS_ON = None   # None: on the N64 stream only; else the set of route levels (--stick-boss)
LAIR_PUSH = ""       # round 18 g5: --lair-push toward|jump|both (T2_LAIR_PUSH; "" = off: the routes unchanged)
ASSIST_KEEP = []    # ... except these fights (--assist-keep: Route.fights keys)
NAV = set()         # round 16 e3: the levels whose NAV_WALKS teleports the waypoint walker replaces (--nav)
NAV_ROM = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "baserom.us.z64")


class Route:
    """Inputs, teleports and the frame-window debug switches of one run, in absolute frames."""

    def __init__(self):
        self.inputs = [pt.NEW_GAME]
        self.shake_switch = {}   # round 17 f2: {level: the schedule frame --n64-shake-from switches at}
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
        self.fights = {}         # round 15 d3: name -> (first, last frame) (--assist-keep)
        self.fight_level = {}    # name -> its level (1-4)
        self.cheats_on = set()   # sub-levels (the totems: 109 110 111 112) kept invincible (--cheats-on)
        self.walked = 0
        self.walks = []          # round 18 g5: (first, last frame) of each teleport's C-up walk
        self.walk_probe = False
        self.walk = set()        # round 15 d5 (--walk): the levels whose open-floor teleports are walked
        # round 15 d2 (--real-weapons): no T2_GIVE_WEAPONS / T2_FILL_AMMO before this frame (the
        # weapons and ammo picked up on the way, the weapon changes in the input stream)
        self.real_until = None
        self.move_drive = []     # round 15 C30 (--lair-drive): T2_MOVE_DRIVE windows
        self.walk_plan = []      # round 16 e3 (--nav): T2_WALK_PLAN windows (f1, f2, sub-level, x, y, z, rotY)

    def note(self, frame, text):
        self.inputs.append("# %d: %s" % (frame, text))
        self.timeline.append((frame, text))

    def walk_to(self, f, frm, x, y, z, ry, walk=0, sub=None):
        """Instead of a teleport to (x, z, ry) at the end: walk there from frm (x, z, rotY) with the
        stick (playthrough.stick_walk) from frame f, then the step's own C-up walk. Returns the
        frames the walk takes (the step happens that much later).
        sub (round 16 e3, --nav): the sub-level; the walk is then aimdrive.py's waypoint walker, closed
        loop from where the player really is, in a window of nav_frames() frames."""
        if sub is not None:
            n = nav_frames(sub, frm, (x, y, z), ry)
            self.walk_plan.append((f, f + n - 1, sub, x, y, z, ry))
            self.move_drive.append((f, f + n - 1))
            self.note(f, "%d: walk to (%.0f, %.0f) [%d frames]" % (sub, x, z, n))
            self.walked += 1
            if self.walk_probe:   # --walk-probe: the teleport too, at the window's end (each walk measured alone)
                self.tps.append("%d:%.1f,%.1f,%.1f,%.4f" % (f + n, x, y, z, ry))
            if walk:
                self.inputs.append("CU@%d..%d" % (f + n + 10, f + n + 10 + walk))
            return n
        else:
            ins, n = pt.stick_walk(f, frm[:2], frm[2], (x, z), ry)
            self.inputs += ins
        self.walked += 1
        if self.walk_probe:   # --walk-probe: the teleport too, at the walk's end (its log line: the arrival)
            self.tps.append("%d:%.1f,%.1f,%.1f,%.4f" % (f + n, x, y, z, ry))
        if walk:
            self.inputs.append("CU@%d..%d" % (f + n + 10, f + n + 10 + walk))
        return n

    def tp(self, f, x, y, z, ry, walk=0, trig=False):
        self.tps.append("%d:%.1f,%.1f,%.1f,%.4f%s" % (f, x, y, z, ry, "!" if trig else ""))
        if walk:
            self.inputs.append("CU@%d..%d" % (f + 10, f + 10 + walk))
            self.walks.append((f, f + 20 + walk))   # round 18 g5: no stick turn during the walk (STICK_FREE_WALK)

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
                if o.get("walkfrom"):   # round 15 d5: walked (the step was moved by the walk's frames)
                    self.walk_to(f - o["walkframes"], o["walkfrom"], x, y, z, ry, walk, o.get("walksub"))
                else:
                    self.tp(f, x, y, z, ry, walk, o.get("trig"))
                if o.get("stick"):   # round 14 c5: aim with the stick only (playthrough.stick_aim)
                    self.inputs += pt.stick_aim(x, z, ry, o["stick"], f + 2)
                if o.get("weapon") is not None:   # round 15 d2: a weapon chosen on the wheel before the step
                    self.inputs += wheel(o["weapon"], f - WHEEL_BEFORE)
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
        os.makedirs(out, exist_ok=True)  # round 16 e4: RUN/ need not exist
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
        give, fill = "1", self.fill
        if self.real_until is not None:   # round 15 d2: --real-weapons
            give = "\"0;1@%d\"" % self.real_until
            fill = [(0, "0")] + [(f if f > 0 else self.real_until, v) for f, v in self.fill if f == 0 or f >= self.real_until]
        env = ["T2_GIVE_WEAPONS=%s" % give, "T2_WEAPON=\"%s\"" % sched(self.weapon), "T2_CHEATS=\"%s\"" % sched(cheats),
               "T2_FILL_AMMO=\"%s\"" % sched(fill),
               "T2_FACE_NEAREST=4000@%s" % ",".join(str(l) for l in self.face_levels),
               "T2_FACE_PITCH=1", "T2_FACE_STICKY=1", "T2_BOSS_LOG=1",
               "T2_AIM_AT=\"%s\"" % ";".join(self.aim_at), "T2_AIM_ENEMY=\"%s\"" % ";".join(self.aim_enemy)]
        if self.aim_node:
            env.append("T2_AIM_NODE=%s" % self.aim_node)
        if STICK_AIM:
            # round 15 d3 (--stick-aim): in these levels the assists above only pick the target and
            # tools/port/aimdrive.py aims with the stick (run the game under it: PLAYTHROUGH.md 24)
            starts = sorted(self.level_start.items())
            wins = [(f, ([g for l, g in starts if l == lv + 1] or [10 ** 7])[0] - 1)
                    for lv, f in starts if lv in STICK_AIM]
            # --assist-keep: these fights keep the assists' yaw and pitch; so do the fights of the
            # levels not in --stick-aim (a fight can outlast its level's window: the City Totem's
            # fire window runs past the hub frame the next level's window starts from)
            keep = ASSIST_KEEP + [k for k, lv in self.fight_level.items() if lv not in STICK_AIM]
            for k in keep:
                a, b = self.fights[k]
                wins = [w for w in wins if w[1] < a or w[0] > b] + \
                       [(w[0], a - 1) for w in wins if w[0] < a <= w[1]] + [(b + 1, w[1]) for w in wins if w[0] <= b < w[1]]
            # round 18 g5: in the levels of STICK_FREE_WALK the stick leaves the teleports' walks alone (level 5's
            # embryo window ran on into 91's walk to the exit 7500: the stick turned to a new target and the
            # walk went back to 124). Levels 1-4 keep round 17's windows (their validated runs).
            def lv_of(fr):
                return max([l for l, g in starts if g <= fr] or [1])
            for a, b in self.walks:
                if lv_of(a) in STICK_FREE_WALK & STICK_AIM:
                    wins = [w for w in wins if w[1] < a or w[0] > b] + \
                           [(w[0], a - 1) for w in wins if w[0] < a <= w[1]] + [(b + 1, w[1]) for w in wins if w[0] <= b < w[1]]
            env.append("T2_AIM_STICK=\"%s\"" % ";".join("%d-%d" % w for w in sorted(wins)))
            # round 18 g5: the stick driver takes the assist's own target first in these sub-levels (aimdrive.py
            # T2_AIMD_PRIMARY: the objectives, not the sleeping or out-of-play actors near them)
            prim = sorted(set(l for lv in STICK_AIM for l in STICK_PRIMARY.get(lv, ())))
            if prim:
                env.append("T2_AIMD_PRIMARY=%s" % ",".join(str(l) for l in prim))
            if 6 in STICK_AIM:   # round 18 g5: the Mother's parts: always the assist's node (aimdrive.py)
                env.append("T2_AIMD_PRIMARY_ONLY=3")
            bl = (STICK_BOSS_ON if STICK_BOSS_ON is not None else set(STICK_BOSS) if n64_full() else set()) & STICK_AIM
            if bl:   # round 19 h3: the boss's node only, led, fire held on target (T2_AIMD_BOSS)
                env.append("T2_AIMD_BOSS=%s" % ",".join(STICK_BOSS[lv] for lv in sorted(bl) if lv in STICK_BOSS))
        if self.move_drive:
            env.append("T2_MOVE_DRIVE=\"%s\"" % ";".join("%d-%d" % w for w in sorted(self.move_drive)))
        if self.walk_plan:   # round 16 e3: aimdrive.py's waypoint walker (WalkDrive)
            env.append("T2_WALK_PLAN=\"%s\"" % ";".join("%d-%d@%d:%.1f,%.1f,%.1f,%.4f" % w for w in self.walk_plan))
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
        # round 16 e1: the camera shake as before round 16 (CCamera__Construct radius 0, no shake draws):
        # the route was tuned on that RNG stream (PLAYTHROUGH.md 27); --n64-shake (round 16 merge,
        # PLAYTHROUGH.md 31) leaves it out: the N64's camera shake and RNG, with the N64_* tables
        if not N64_SHAKE:
            if N64_FROM is not None:
                # round 17 f2 (--n64-shake-from L): the old stream up to the hub before level L (7: the
                # Primagen's lair), the N64's from there (port/game/t2debug.c T2Port_LegacyShakeTick)
                env.append("T2_LEGACY_SHAKE=1,%d" % self.shake_switch[N64_FROM])
            else:
                env.append("T2_LEGACY_SHAKE=1")
        # round 17 f3: the type-9 (interactive anim) props as before round 17 (their intelligence body left
        # big-endian: they start on animation 0); every route so far was tuned on that, the --n64-shake
        # one included. --n64-props leaves it out (the N64's props, FIDELITY.md 7)
        if not N64_PROPS:
            env.append("T2_LEGACY_INTEL9=1")
        # round 17 merge: headless runs as before round 17, with no sound world (no sound random draws);
        # every route was tuned on that silent stream. --n64-sound leaves it out (the sound requests draw
        # as with --audio, a player with sound and the N64)
        if not N64_SOUND:
            env.append("T2_LEGACY_NOSOUND=1")
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
        print("%d inputs, %d teleports, %d walks (%d by the waypoint walker); last frame %d" % (
            len(self.inputs), len(self.tps), self.walked, len(self.walk_plan), last))
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

# ---- round 15 d5 (--walk 1,2): teleports replaced by stick walking on open floor ----
# (segment index, step index): where the player stands, and faces, when the step's teleport fires in
# the default run (its teleport log and det log: x, z, rotY; tools/port/walkcands.py lists them). The
# walk starts there at the step's frame and the step (and every later one) moves by the walk's frames.
# Probed (--walk 1,2 --walk-probe, Linux Release; PLAYTHROUGH.md 22): of the 19 teleports of levels
# 1-2 that stay on one floor, only two arrive within 200 units; the others stop against walls, cages,
# graves and crypt doors 150-3,400 units short (they are not plain walks), or follow an aim-assisted
# fight whose end yaw the open-loop walk cannot know. Kept: the one that arrives and resyncs.
L1_WALKS = {}
# 53: the beacon-2 walk stops at a wall (z 7,121, 1,422 units short), the two cage walks against
#     the cages' bars (233 and 955 short).
L2_WALKS = {
    (9, 2): (78.7, -870.5, -1.5306),       # 134: from the start to the gate (1,571 units; arrives
                                           # ~200 short; the zombie-4 fight there opens gate 8 and
                                           # the next step teleports back to the gate spot)
}
# ---- round 16 e3 (--nav L,...): teleports replaced by the closed-loop waypoint walker ----
# {level: {(segment index, step index): (x, z, rotY, y) where the player stands when the step's teleport
# fires in the default run}} (tools/port/navcands.py RUNLOG --table: the route steps of levels 1-4 whose
# two ends tools/port/nav.py joins on the floor). The walk starts at the step's frame from wherever the
# player really is; the step (and every later one of the level) moves by the window (nav_frames).
NAV_WALKS = {
    1: {
        (2, 2): (1498.6, 6950.4, -0.0126, 0.0),   # 14900 in 53: 1601 straight, 1824 by the path
        (2, 5): (460.8, -47.8, 0.0000, 0.0),   # 15290 in 53: 304 straight, 304 by the path
        (4, 0): (2664.2, 1557.9, 0.0000, 921.6),   # 16400 in 55: 5475 straight, 6878 by the path
        (5, 0): (768.0, -3225.6, 1.5707, 614.4),   # 16850 in 54: 5053 straight, 6681 by the path
        (5, 1): (-3961.7, -1737.9, -1.5708, 0.0),   # 17000 in 54: 1219 straight, 1932 by the path
        (7, 1): (-2321.3, -1021.3, -1.5708, -614.4),   # 18050 in 57: 1184 straight, 5816 by the path
        (8, 0): (1201.3, -1544.1, 0.0000, 307.2),   # 18800 in 58: 1469 straight, 1710 by the path
    },
    2: {
        (6, 0): (920.5, -1335.8, 0.0000, -4761.5),   # 36680 in 133: 2452 straight, 3690 by the path
        (6, 1): (-1504.0, -972.0, -4.8374, -4761.6),   # 38080 in 133: 2629 straight, 3723 by the path
        (6, 2): (-452.9, 1304.6, 0.4067, -4761.6),   # 47680 in 133: 3199 straight, 7298 by the path
        (9, 0): (78.7, -870.5, -1.5707, 1024.0),   # 49280 in 134: 0 straight, 0 by the path
        (9, 1): (78.7, -870.5, -1.9397, 1024.0),   # 50480 in 134: 0 straight, 0 by the path
        (9, 2): (78.7, -870.5, -1.5306, 1024.0),   # 51780 in 134: 1571 straight, 1716 by the path
        (9, 3): (997.3, -933.3, 1.1943, 1024.0),   # 54880 in 134: 656 straight, 778 by the path
        (9, 4): (1650.0, -870.0, -2.3920, 1024.1),   # 59880 in 134: 632 straight, 632 by the path
        (12, 1): (141.7, -5650.5, 0.0000, 0.0),   # 63080 in 135: 719 straight, 719 by the path
        (12, 2): (-37.6, -4953.9, 3.6074, 0.0),   # 64680 in 135: 4222 straight, 4381 by the path
        (12, 3): (894.3, -669.5, 3.0697, 0.2),   # 64880 in 135: 2435 straight, 3089 by the path
        (12, 4): (2822.9, -2353.8, -0.3950, 0.2),   # 65080 in 135: 5750 straight, 6576 by the path
        (12, 5): (-2921.4, -1332.8, 2.7377, 0.2),   # 65280 in 135: 5826 straight, 6678 by the path
        (12, 6): (2809.4, 60.0, -0.8506, 0.2),   # 65480 in 135: 5656 straight, 7065 by the path
        (12, 7): (-2922.0, 202.2, 2.6991, 0.2),   # 65680 in 135: 1385 straight, 2569 by the path
        (12, 8): (-2924.3, 1640.3, 1.5749, 0.2),   # 65880 in 135: 5520 straight, 5520 by the path
        (12, 9): (2824.0, 1417.2, -1.5329, 0.2),   # 66080 in 135: 3898 straight, 3967 by the path
        (12, 10): (-1043.0, 926.0, 5.3412, 0.2),   # 68680 in 135: 451 straight, 451 by the path
        (12, 11): (-781.7, 465.2, -0.7420, 0.2),   # 68980 in 135: 2136 straight, 2197 by the path
        (12, 12): (1336.0, 743.0, 1.4499, 0.2),   # 71580 in 135: 392 straight, 392 by the path
        (12, 13): (-673.8, 264.6, 0.7710, 0.1),   # 71880 in 135: 2021 straight, 3278 by the path
        (12, 14): (-1109.6, 2171.3, 0.6540, 0.0),   # 72180 in 135: 2361 straight, 2363 by the path
        (12, 15): (1314.6, 2173.7, -0.6800, 0.0),   # 72580 in 135: 1872 straight, 2423 by the path
    },
    3: {
        (0, 0): (4616.5, -3914.2, -3.1416, -431.0),   # 97644 in 61: 366 straight, 366 by the path
        (1, 1): (640.3, -2464.5, -1.5707, 308.4),   # 98924 in 69: 2371 straight, 4823 by the path
        (1, 2): (-1492.1, -1428.2, 3.1416, -305.2),   # 99074 in 69: 0 straight, 0 by the path
        (5, 0): (-1280.0, 4475.4, -1.5707, 2.8),   # 101444 in 64: 186 straight, 186 by the path
        (6, 1): (1231.1, -1283.0, 0.0000, -921.4),   # 102274 in 70: 3341 straight, 4772 by the path
        (6, 2): (-887.1, 1300.7, -0.0187, -614.2),   # 102404 in 70: 0 straight, 0 by the path
        (8, 1): (-1068.2, -5287.7, -1.5707, 18.3),   # 103444 in 65: 3740 straight, 5620 by the path
        (9, 1): (-368.1, -1084.5, 0.0000, -252.7),   # 104194 in 66: 387 straight, 655 by the path
        (9, 3): (524.4, 2697.8, 2.5131, -903.3),   # 104594 in 66: 2353 straight, 2740 by the path
        (11, 1): (710.2, -899.5, 0.0000, -1533.8),   # 105724 in 71: 1864 straight, 5215 by the path
        (11, 2): (-800.8, 191.7, 1.1009, -919.4),   # 105874 in 71: 0 straight, 0 by the path
        (13, 0): (-2211.6, -1543.4, 0.0000, 0.0),   # 106894 in 68: 3353 straight, 3353 by the path
    },
    4: {
        (7, 0): (629.8, -2867.2, -2.0720, -2457.4),   # 122994 in 107: 3068 straight, 3614 by the path
    },
}
NAV_SPEED = 0.85     # nav_frames: the walking speed reached on a path (the corners)
NAV_SLACK = 40       # nav_frames: frames on top
# Probed (--nav 1,2,3,4 --walk-probe: each walk from the default route's place, the teleport kept at the
# window's end; Linux Release; PLAYTHROUGH.md 30): the walks that do not arrive (arrival error in units),
# and the ones that arrive but break the full run (tried with --nav-only on the default command). Left:
# 135's (12, 1) (--nav 2: END 2, all five totems)
NAV_SKIP = {   # {level: {(i, j): why}}: candidates the walker does not take
    1: {(7, 1): "57: arrives (error 48), but a death in 133's zombie fight follows (level 2 starts from another state)",
        (2, 2): "53: a closed door across the power cell's alcove (z 7,120), 1,422",
        (2, 5): "53: the cage's bars, 237",
        (4, 0): "55: a ledge and a climb on the way, 3,830",
        (5, 0): "54: the way down from the ledge, 2,971",
        (5, 1): "54: the cage's bars, 961",
        (8, 0): "58: a drop the walker does not find, 218-3,286"},
    2: {(6, 0): "133: arrives (error 25); not run in the full run (133's fights)",
        (9, 0): "134: in place; a 40-frame window: deaths at the gate (53,513)",
        (9, 1): "134: in place; as (9, 0)", (9, 2): "134: arrives (error 50); death at the gate (53,755)",
        (9, 3): "134: after the gate fight, 256-609 short", (9, 4): "134: arrives (error 43); with (9, 2) only",
        (12, 10): "135: arrives (error 44); a death in sister 3's fight (72,097)",
        (12, 12): "135: arrives (error 49); in sister 3's fight", (12, 15): "135: arrives (error 49); in sister 3's fight",
        (6, 1): "133: a closed door between the zombies' rooms (x -1,547), 2,639",
        (6, 2): "133: doors, 3,365",
        (12, 2): "135: the gate posts at z -3,350, 2,715",
        (12, 3): "135: the crypts, 1,803", (12, 4): "135: the crypts, 4,939", (12, 5): "135: the crypts, 3,339",
        (12, 6): "135: the crypts, 4,843", (12, 7): "135: the crypts, 1,912", (12, 8): "135: the crypts, 4,680",
        (12, 9): "135: the crypts, 3,112", (12, 11): "135: the graves, 1,984", (12, 13): "135: the graves, 2,028",
        (12, 14): "135: stops 127 short (the fight there needs the spot)"},
    3: {**{k: "level 3: the walks start while the player is elsewhere (warps and cinemas move under the anchors)"
           for k in ((0, 0), (1, 1), (5, 0), (6, 1), (6, 2), (8, 1), (9, 3), (11, 1), (11, 2), (13, 0))},
        (1, 2): "69: in place; with (9, 1) a death in 61 (122,021), game over",
        (9, 1): "66: arrives (error 41); as (1, 2)"},
    4: {(7, 0): "107: 2,286 (stuck 5 times)"},
}


def nav_frames(sub, frm, to, ry_end=None):
    """The waypoint walker's window for a walk from frm (x, z, rotY[, y]) to `to` (x, y, z) in sub-level sub,
    facing ry_end at the end: the path at NAV_SPEED of the walking speed, the two turns at the stick's full
    rate, NAV_SLACK more (the route's fights go on round a walk: the window is kept short)."""
    import nav
    y0 = frm[3] if len(frm) > 3 else to[1]
    try:
        res = nav.load(NAV_ROM, sub).path((frm[0], y0, frm[1]), tuple(to))
    except Exception:
        res = None
    pts = res[0] if res else [(frm[0], frm[1]), (to[0], to[2])]
    ln = res[2] if res else math.hypot(to[0] - frm[0], to[2] - frm[1])
    turns = 0.0
    yaw = frm[2]
    if ln > 1.0:
        first = pt.face_yaw(pts[0][0], pts[0][1], pts[1][0], pts[1][1])
        last = pt.face_yaw(pts[-2][0], pts[-2][1], pts[-1][0], pts[-1][1])
        turns += abs(pt.wrap(first - yaw))
        yaw = last
    if ry_end is not None:
        turns += abs(pt.wrap(ry_end - yaw))
    walk = int(ln / (NAV_SPEED * pt.WALK_SPEED)) + pt.WALK_LAG if ln > 1.0 else 0
    return walk + int(turns / pt.TURN_FULL) + NAV_SLACK


# Probed and failed (arrival error in units): 133 (6, 0) 2,143, (6, 1) 2,151, (6, 2) 5,191 (the
# zombies' rooms: doors); 134 (9, 3) 867, (9, 4) 1,669 (after the aim-assisted fights: the yaw is
# not the default run's); 135 (12, 1) 1,479, (12, 3) 1,674, (12, 7) 971, (12, 9) 3,383, (12, 10)
# 156, (12, 11) 1,978, (12, 12) 1,611, (12, 13) 2,171, (12, 14) 1,030, (12, 15) 813 (graves,
# crypts, the water hall's walls). A closed-loop walker (the d3 input pipe) is what they need.

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


NAV_ONLY = None      # --nav-only L:I.J,...: just these walks (testing one at a time)


def nav_table(level):
    """NAV_WALKS[level] less NAV_SKIP[level] (or just --nav-only's)."""
    if NAV_ONLY is not None:
        return {k: v for k, v in NAV_WALKS.get(level, {}).items() if (level,) + k in NAV_ONLY}
    return {k: v for k, v in NAV_WALKS.get(level, {}).items() if k not in NAV_SKIP.get(level, {})}


def merged(*dicts):
    out = {}
    for d in dicts:
        for k, v in d.items():
            out.setdefault(k, []).extend(v)
    return out


def level1(r, start_shift=0, keys=True, health=True, arms=False):
    """Level 1 from the new game: (the frame level 109 is reached, the extra frames added).
    arms (round 15 d2, --real-weapons 1): L1_ARMS's weapons and ammo picked up too."""
    added = 0
    for i, (name, base, steps) in enumerate(pt.SEGMENTS):
        f0 = base + start_shift + added
        r.note(f0, name)
        picks = merged(L1_KEYS, L1_HEALTH if health else {}, L1_ARMS if arms else {}) if keys else {}
        if i in picks:
            for k in picks[i]:   # (x, y, z, ang[, d, walk]): round 15 d2's ARMS give d and walk too
                r.pick(f0, *k)
                f0 += KEY_STEP
                added += KEY_STEP
        for j, (dt, x, y, z, ry, walk) in enumerate(steps):
            if 1 in NAV and (i, j) in nav_table(1):   # round 16 e3: the waypoint walker
                n = r.walk_to(f0 + dt, nav_table(1)[(i, j)], x, y, z, ry, walk, int(name.split()[0]))
                f0 += n
                added += n
            elif 1 in r.walk and (i, j) in L1_WALKS:
                n = r.walk_to(f0 + dt, L1_WALKS[(i, j)], x, y, z, ry, walk)
                f0 += n
                added += n
            else:
                r.tp(f0 + dt, x, y, z, ry, walk)
    arrival = pt.TOTEM_ARRIVAL + start_shift + added
    r.note(arrival, "109 the Harbor Totem: fire taps (T2_FACE_NEAREST aims)")
    r.fire.append((arrival, arrival + 7150))
    r.fights["109"] = (arrival, arrival + 7150)
    r.fight_level["109"] = 1
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

TOTEM_WEAPON_BEFORE = 600   # the scorpion launcher this long before a totem's fire window (levels 3, 4)

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
# --n64-shake (round 16 merge, PLAYTHROUGH.md 31): the route on the N64's RNG stream (no T2_LEGACY_SHAKE).
# The extra fire windows it needs, as L2_STRETCH entries (segment, step, frames)
N64_PROPS = False  # round 17 f3: --n64-props
N64_SOUND = False  # round 17 merge: --n64-sound
N64_SHAKE = False
N64_FROM = None       # --n64-shake-from L (round 17 f2): the N64's stream only from level L's hub visit on


N64_CAVE_WEAPON = "9"   # the Cave Totem: the shredder on the N64 stream (85-135 damage with the scorpion launcher, 18-64 with it; round 17 f2)
N64_MOTHER_MOVE = 4600   # the strafing and steps back at the Mother from her arena + this (5,800 legacy)


def n64(lv):
    """Round 17 f2: whether level lv (7: the Primagen's lair) is played on the N64's random numbers
    (--n64-shake, or --n64-shake-from L with lv >= L): the route changes for that stream apply."""
    return N64_SHAKE or (N64_FROM is not None and lv >= N64_FROM)
# Round 18 g1 (--n64): the weapon of a level-2 segment on the N64's stream {segment: (weapon, frames after its
# start)}; the plasma rifle (10) again 50 frames before the next segment. T2G1_L2W="9:9:0,6:8:0" overrides (tests)
N64_L2_WEAPON = {9: ("9", -400)}   # 134: the shredder (21 damage instead of 118 and a life with the plasma rifle)
if os.environ.get("T2G1_L2W") is not None:
    N64_L2_WEAPON = {int(e.split(":")[0]): (e.split(":")[1], int(e.split(":")[2]))
                     for e in os.environ["T2G1_L2W"].split(",") if e}


def n64_full():
    """Round 18 g1: no legacy switch at all (--n64): the stream a human player gets."""
    return N64_SHAKE and N64_PROPS and N64_SOUND


# Round 18 g1 (--n64): 133 (sister 1) leaves the player with ~18 health on this stream and 130's ultra health
# (+100 to 200) comes next: the health +10 (object 542, +10 below 100) of 133 and 130 first, so the ultra health
# lifts a full 100 (tools/assets/levels.py grid sections). 133's two by its exit (picks before the exit step),
# 130's ten at the start of its second visit (before the ultra health of L2_HEALTH[7])
N64_FULL_L2_PICKS = [(6, 2, [(644.9, -4761.6, -1156.9, 0.0), (1190.2, -4761.6, -1155.7, 0.0)])]
N64_FULL_L2_HEALTH = {7: [(-2405.3, -4761.6, 5545.7, 0.0), (-2135.0, 204.8, 3506.0, 0.0), (-581.3, -5.2, 3644.5, 0.0),
                          (-512.0, -11.3, -2560.0, 0.0), (-512.0, -10.7, -2764.8, 0.0), (2246.1, -2150.4, 2102.3, 0.0),
                          (5581.0, -3993.6, 1285.7, 0.0), (5580.1, -3379.2, 1275.8, 0.0), (6451.2, 307.2, 3527.7, 0.0),
                          (6448.8, 307.2, 4057.7, 0.0)],
                      # 132's last visit (after sister 3: 135 leaves ~70 health and the City Totem is next)
                      # (the two on 1,331 and the two on 1,106 stand the player over a lower floor with a damage region: dropped)
                      13: [(941.4, 716.8, -1907.8, 0.0), (512.4, 716.8, -1904.1, 0.0), (-1382.4, 179.4, -1792.0, 0.0),
                           (-1382.4, 179.4, -1331.2, 0.0), (4410.7, 409.6, -4062.9, 0.0)]}
# Round 18 g1 (--n64): the weapon for the Mantid Queen (None: level 5 s shredder); T2G1_QW overrides ("": none)
N64_QUEEN_WEAPON = os.environ.get("T2G1_QW", "16") or None   # the scorpion launcher: 6-95 damage, 43-138 (a life) with the shredder
N64_L2_STRETCH = []   # none yet: +800 frames on zombie 44 and +1000 on zombies 45 and 44 (135) did not save the crypt steps
# Round 17 f1: pick steps put before a step of a segment (segment, before step, [(x, y, z, approach
# angle)]): each pick is a KEY_STEP-frame step (pick()'s stand and walk-in); the step and the later
# ones move, and so do the segment's retries and returns at or after that step's frame
N64_L2_PICKS = [
    # 135, after zombie 44's fight (the player leaves it with ~25 health), before crypt 76's key: 7 health
    # +10 (only taken below 100) around the crypts; the approach angles were probed (some angles stand
    # the player in a wall: no region, no teleport)
    (12, 12, [(1554.3, 0.0, 589.6, 0.0), (1556.3, 0.0, 76.6, 0.0), (2554.0, 0.2, 92.6, 0.0), (867.5, 0.0, 1146.0, 3.1416),
              (-545.3, 0.0, 1148.1, 3.1416), (-1269.4, 0.0, 1146.0, 3.1416), (-1345.9, 0.0, 457.2, 1.5708)]),
]


def insert_picks(segments, picks, extra=(), retry=(), back=()):
    """segments with pick steps inserted (N64_L2_PICKS); returns (segments, stretch entries that only
    move the later steps (no fire), retry, back) with the in-segment teleports moved as well."""
    segs = [list(s) for s in segments]
    shifts = []                       # (segment, original dt, frames)
    for si, sj, items in sorted(picks, key=lambda p: (p[0], -p[1])):
        name, rel, steps = segs[si]
        steps = [list(st) for st in steps]
        at = steps[sj][0]
        new = []
        for k, (x, y, z, ang) in enumerate(items):
            new.append([at + k * KEY_STEP, x + math.sin(ang) * 90.0, y, z + math.cos(ang) * 90.0, ang, 50, {}])
        more = len(items) * KEY_STEP
        for st in steps[sj:]:
            st[0] += more
        steps[sj:sj] = new
        segs[si] = (name, rel, [tuple(st) for st in steps])
        shifts.append((si, at, more, sj + len(new)))
    # the stretch entries: no fire window, only the later steps move (stretch() with fire=False); the
    # steps already moved here, so the entry sits on the segment's last step
    ext = [(si, len(segs[si][2]) - 1, more, False) for si, at, more, j in shifts]

    def moved(entries):
        out = []
        for e in entries:
            e = list(e)
            e[1] += sum(m for si, at, m, j in shifts if si == e[0] and e[1] >= at)
            out.append(tuple(e))
        return out
    # the stretch entries of a segment with picks: the step indices past an insertion move too
    ext0 = []
    for e in extra:
        e = list(e)
        e[1] += sum(len(items) for si, sj, items in picks if si == e[0] and e[1] >= sj)
        ext0.append(tuple(e))
    return [tuple(s) for s in segs], ext0 + ext, moved(retry), moved(back)


# Round 17 f5 (--l2-heal): 135's health +10 pickups (object 542, item 1702: +10, up to 100; tools/assets/levels.py
# level actors) on the graveyard floor, walked into after sister 3 and before the exit 2608, KEY_STEP frames each:
# the City Totem's fight (110) comes next with no health on the way, and a stick or real-weapons run reached it
# with 18-67 health (two lives lost there). Not the six beside sister 3's spot (the pit's damage region).
L2_HEAL = False
# (x, y, z, approach angle): the stand point 90 units out along the angle must be graveyard floor (y 0; the
# graves are y 87; outside every region the teleport fails)
L2_HEAL_PICKS = [(102.4, 0.0, -358.4, 0.0), (1556.3, 0.0, 76.6, 0.0), (1554.3, 0.0, 589.6, 0.0),
                 (867.5, 0.0, 1146.0, math.pi), (-545.3, 0.0, 1148.1, math.pi), (-1269.4, 0.0, 1146.0, math.pi),
                 (-1345.9, 0.0, 457.2, math.pi / 2), (2066.9, 0.2, 1323.4, 0.0), (2554.0, 0.2, 92.6, 0.0),
                 (1235.9, 0.2, -839.6, 0.0)]


def l2_heal(segs):
    """--l2-heal: L2_HEAL_PICKS before 135's exit step; the later segments move by the frames added.
    Returns (segments, frames added)."""
    segs = list(segs)
    i = [k for k, sg in enumerate(segs) if sg[0].startswith("135 ")][0]
    name, rel, steps = segs[i]
    warp = steps[-1]
    picks = [(warp[0] + KEY_STEP * k, x + math.sin(a) * 90.0, y, z + math.cos(a) * 90.0, a, 50, {})
             for k, (x, y, z, a) in enumerate(L2_HEAL_PICKS)]
    d = KEY_STEP * len(picks)
    segs[i] = (name, rel, list(steps[:-1]) + picks + [(warp[0] + d,) + tuple(warp[1:])])
    for k in range(i + 1, len(segs)):
        segs[k] = (segs[k][0], segs[k][1] + d, segs[k][2])
    return segs, d


def l2_segments(nocheats):
    """playthrough.L2_SEGMENTS, with round 12's sister-1 steps when level 2 has no invincibility."""
    if not nocheats:
        return pt.L2_SEGMENTS, L2_STRETCH
    segs = list(pt.L2_SEGMENTS)
    name, rel, steps = segs[6]
    assert name.startswith("133 ")
    segs[6] = (name, rel, L2_133_NOCHEATS)
    return segs, L2_STRETCH_NOCHEATS


def merge_stretch(a, b):
    """Two L2_STRETCH lists as one (the same step's frames added)."""
    d = {}
    for si, sj, m in list(a) + list(b):
        d[(si, sj)] = d.get((si, sj), 0) + m
    return [(si, sj, m) for (si, sj), m in sorted(d.items())]


def stretch(segments, extra):
    segs = []
    for i, (name, rel, steps) in enumerate(segments):
        steps = [list(st) for st in steps]
        for e in extra:
            si, sj, more = e[:3]
            if si != i:
                continue
            if len(e) < 4 or e[3]:   # round 17 f1: (si, sj, frames, False) only moves the later steps
                o = dict(steps[sj][6])
                o["fire"] = o.get("fire", 0) + more
                steps[sj][6] = o
            for k in range(sj + 1, len(steps)):
                steps[k][0] += more
        segs.append((name, rel, [tuple(st) for st in steps], sum(e[2] for e in extra if e[0] == i)))
    return segs


def level_segments(r, segments, start, keys, retry=(), extra=(), anchor=(), back=(), detours=None, walks=None,
                   nav_walks=False):
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
        if walks:   # round 15 d5: walked steps move this and the later steps by the walk's frames
            steps = [list(st) for st in steps]
            for j in range(len(steps)):
                if (i, j) not in walks:
                    continue
                st = steps[j]
                o = dict(st[6] if len(st) > 6 else {})
                frm = walks[(i, j)]
                o["walkfrom"] = frm
                if nav_walks:   # round 16 e3: the waypoint walker's window
                    o["walksub"] = int(name.split()[0])
                    o["walkframes"] = nav_frames(o["walksub"], frm, (st[1], st[2], st[3]), st[4])
                else:
                    o["walkframes"] = pt.stick_walk(0, frm[:2], frm[2], (st[1], st[3]), st[4])[1]
                st = st[:6] + [o]
                steps[j] = st
                for k in range(j, len(steps)):
                    steps[k][0] += o["walkframes"]
                more += o["walkframes"]
            steps = [tuple(st) for st in steps]
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
        for f, k in pre:
            r.pick(start + f, *k)
    for f, lv in trips:
        talisman(r, start + f, lv)
    r.steps(out, start)
    r.seg_starts = {i: start + f for i, f in starts.items()}   # round 18 g1: the segments' schedule frames
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
# round 14 c3: each Primagen key slot takes ONE key (its action's m_GoPickupNeeded, read in the hub
# with gdb): key K goes into the slot whose required item is 4359 + K. Round 13's map was by ring
# order (its "slot 2" was actor 5, which needs key 6): a slot touched without its key refuses it
# (Action_CheckRequiredItem returns 0, "Primagen key N required") and stays solid, which is why the
# walks at "slots 2 and 3" slid round them. Key K: (x, y, z) of its slot.
PRIMAGEN_SLOTS = {1: (71.0, 0.0, 183.0),     # actor 4  (object 154), item 4360
                  2: (-85.7, 0.0, 202.8),    # actor 11 (object 291), item 4361
                  3: (-184.9, 0.0, 72.6),    # actor 12 (object 292), item 4362
                  4: (-119.0, 0.0, -71.0),   # actor 7  (object 161), item 4363
                  5: (36.7, 0.0, -84.5),     # actor 6  (object 159), item 4364
                  6: (133.2, 0.0, 38.0)}     # actor 5  (object 158), item 4365
# Round 14 c3: keys 2 and 3 are picked up in levels 2 and 3 but went into their slots only in the hub visit after
# level 5 (with key 5). Put in on the visits after levels 2 and 3, they changed the totem fights that
# follow (the Marsh Totem 7/8 -> 4/8 kills: lost; the defences are marginal, and any change of the
# game state before them moves their kills; see PLAYTHROUGH.md 20). Those two visits keep round 13's
# walks instead (ROUND13_BUMP: they bump the slots of keys 6 and 5, "Primagen key N required"), so
# levels 1-5 play exactly as in round 13; the real order is only a schedule choice of this route.
# Round 14 C29: the natural order is the default again (PRIMAGEN_LATE = ()): with the round-14 routes of
# levels 3-6 the totem defences are won with keys 2 and 3 in on time (two runs, all five totems, END 2).
# --late-keys brings back c3's late insertion and ROUND13_BUMP.
PRIMAGEN_LATE = ()
ROUND13_BUMP = {2: (71.0, 0.0, 183.0, -0.4040), 3: (133.0, 0.0, 38.0, 0.6620)}


def key_visit(k):
    """The hub visit after level k (2 or 3): its Primagen key in its own slot (round 14
    C29, the default), or round 13's walk (ROUND13_BUMP) with the key put in after level 5 (--late-keys)."""
    if k in PRIMAGEN_LATE:
        return {"bump": ROUND13_BUMP[k]}
    return {"primagen": (k,)}
HOP = False                     # --hop: round 13's fallback warp 7602 (DEBUG-ONLY)
LAIR_FRONT = "450,3000,2.5,1"   # --lair-front: T2_BOSS_FRONT in the lair (round 15 d1)
LAIR_STRAFE = None              # --lair-strafe P[,S]: routes56.lair_strafe's period and z sweep (round 15 d1);
                                # None: 90 with the Primagen not invincible, 80 with --cheats-on 4 (C30)
LAIR_KEEP = 800                 # --lair-keep D: T2_LAIR_KEEP on the N64's stream (round 17 f2; 0: off)
LAIR_PEEK = 2400                # --lair-peek W: T2_LAIR_PEEK with the lair driver (round 15 C30)
LAIR_DRIVE = None               # --lair-drive guard|arcs|none: the closed-loop lair driver (aimdrive.LairDrive;
                                # round 15 C30); None: "guard" with the Primagen not invincible, else none


def primagen_slot(r, t, k):
    """Primagen key k into its slot: stand 90 outside the slot (on the line from the plaza's origin),
    walk in (every slot is reached this way: a new game with the six keys given fills all six)."""
    x, y, z = PRIMAGEN_SLOTS[k]
    r.pick(t, x, y, z, math.atan2(x, z))


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


def hub(r, arrival, grant=(), primagen=(), save=True, go=None, bump=None):
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
        primagen_slot(r, t, k)
        t += 150
    if bump:     # round 14 c3: round 13's walk of this visit (ROUND13_BUMP), no key goes in
        x, y, z, ry = bump
        r.note(t, "60 the hub: round 13's walk by the Primagen key slots (no key: the RNG of round 13)")
        r.tp(t, x, y, z, ry, 40)
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


# ---- round 15 d2: levels 1-3 on the weapons and ammo picked up on the way (--real-weapons 1[,2[,3]]) ----
# Without T2_GIVE_WEAPONS the player starts with the talon (weapon 0) and the bow. The weapons and the
# ammo lie as level actors (tools/assets/levels.py's grid sections; cart entry 2 maps the object to the
# pickup id: 2001-2122 weapons, sWeaponPickups {weapon, ammo type, amount}; 2200-3027 ammo, sAmmoPickups
# {type, amount}). Pickup_GiveWeapon selects a new weapon when its index is higher than the current
# one's; the others are chosen on the weapon wheels with the input stream (WHEEL below; T2_WEAPON only
# acts with T2_GIVE_WEAPONS). When a weapon runs dry the game falls back to another one with ammo.
# Picked up like the keys: (x, y floor, z, approach
# angle) at a segment's start, KEY_STEP frames each; T2_GIVE_WEAPONS / T2_FILL_AMMO start in the hub
# after the last such level.
# (x, y, z, ang[, d, walk]): a pick d units out (default 90) and a walk of `walk` frames (default 50,
# ~6 units a frame) through a row of pickups
L1_ARMS = {
    2: [(-3987.8, 79.4, 230.0, 1.5708, 50, 40)],      # 53: on the platform: bullets x10, the pistol (4), bullets x10
    4: [(1075.2, 0.0, -921.6, 0.0), (1075.2, 0.0, -614.4, 3.1416)],          # 55: explosive shells x3, x3
    5: [(-3368.8, 0.0, -1843.0, 1.5708), (-2867.2, 0.0, -1843.2, 1.5708)],   # 54: shells x3, the shotgun (8) +10 shells
    6: [(-585.0, 164.1, 2108.9, 0.0)],                                       # 56: explosive shells x3
    # 59 by the level exit 7100: shells x10, bullets x40, explosive shells x3, tek arrows x5 in a row
    9: [(-9851.8, -307.2, 5427.2, 1.5708, 60, 90)],
}
# round 17 f5: 131's plasma rifle (-1996.8, -1126.4, 5785.6) lies at the bottom of a pool (floor -1,126, water
# -532; round 15 d2: "a death there"); with the 150 plasma cells of 129 it is the weapon for 134 (zombies 2-4 and
# sister 2: the mag 60's 50 bullets do not kill zombies 2 and 3). L2_PLASMA = False: round 16's picks.
L2_PLASMA = False   # tried: the pool is deadly (10 health every 2 frames: 116 -> 0 in 22 frames)
L2_131_PLASMA = [(-1996.8, -1126.4, 5785.6, 0.0, 60, 40)] if L2_PLASMA else []
L2_ARMS = {
    0: [(3408.8, 614.4, -521.9, 0.0), (3406.2, 614.4, -1324.7, -1.5708),   # 125: shells x10, explosive shells x3,
        (2816.0, 614.4, 9984.0, 0.0)],                                      #      shells x10
    # not taken: 128's grenade launcher (-153.6, -1382.4, -5273.6) floats over a deadly floor (-1536)
    4: [(3844.0, 0.0, 2764.2, 1.5708), (4403.2, 0.0, 2662.4, 0.0)],        # 129: the grenade launcher +5, plasma cells x150
    # 130's armory row (y 204.8, z 4736): the mag 60 (5); the tek bow (3), the tranquilizer gun (6), the shotgun
    5: [(-2764.8, 204.8, 4736.0, -1.5708), (-2918.4, 204.8, 4736.0, 1.5708, 78, 60)],
    # 133: the shotgun +10 shells and the mag 60 +bullets: steps of L2_133_REAL (round 16 e5)
    # round 16 e5: ammo for sister 2 (134) after sister 1 has taken it all: 131's terrace (y 102.4),
    # bullets x40, shells x3 and explosive shells x3 (130's bullets x40 and shells x10 by its exit,
    # (6529.9, -3379.2, 478.5) and (6516.3, -3379.2, 2080.0), were not reached by such picks)
    8: [(2581.0, 102.4, 1159.2, 0.0), (1420.5, 102.4, 1161.3, 0.0), (1971.6, 102.4, 1772.7, 0.0)] + L2_131_PLASMA,
}
L3_ARMS = {}
ARMS = {1: L1_ARMS, 2: L2_ARMS, 3: L3_ARMS}
# the weapons chosen on the wheel per level: [(frames after the totem's arrival, weapon), ...]
ARMS_SWITCH = {1: [], 2: [], 3: []}
# the hub arrivals of a --real-weapons run (the first "level 60" T2_STATE_LOG=50 line after the totem)
REAL_HUB_AT = {1: 23750}
BLIND_WEAPON_AFTER = 2336   # round 16 e5: the shredder for the Blind One, this long after the Cave Totem's fire starts
ARMS_FIRE_END = 50      # a totem's fire taps end this long before the hub arrival
# frames idled in the hub after level 1 (--hub-wait): the level-2 fights depend on the state they
# start from; with --real-weapons 1, waits of 40, 50, 120 frames keep sisters 1 and 2 (133, 134)
# without a death, 0, 20, 30, 60, 90, 200 do not; 40 then loses the City Totem, 50 and 120 win it
ARMS_HUB_WAIT = 50


# The weapon wheels (the default control style, CBarrel__RunStick): holding A (0x8000) opens
# sBarrelLayoutGroup1, B (0x4000) sBarrelLayoutGroup0; the analog stick's direction picks one of 8
# slots (CBarrel__GetStickDirection: 0 up, then clockwise; diagonals when ||x| - |y|| < 46), and the
# slot's weapon becomes the requested one (if it has ammo). {weapon: (button, slot)}:
WHEEL = {0: ("A", 0), 23: ("A", 1), 2: ("A", 2), 4: ("A", 3), 6: ("A", 4), 8: ("A", 5), 10: ("A", 6), 11: ("A", 7),
         12: ("B", 0), 13: ("B", 1), 14: ("B", 2), 15: ("B", 3), 16: ("B", 4), 20: ("B", 5), 21: ("B", 6), 22: ("B", 7)}
WHEEL_STICK = [(0, 100), (90, 90), (100, 0), (90, -90), (0, -100), (-90, -90), (-100, 0), (-90, 90)]
WHEEL_BEFORE = 50       # the wheel is worked this long before a route step's teleport (which sets the yaw again)


def wheel(weapon, f):
    """The inputs that choose `weapon` on its wheel from frame f (30 frames: the button held, the
    stick pushed towards the slot from its 10th frame)."""
    b, slot = WHEEL[weapon]
    x, y = WHEEL_STICK[slot]
    return ["%s@%d..%d" % (b, f, f + 30), "SX=%d@%d..%d" % (x, f + 10, f + 25), "SY=%d@%d..%d" % (y, f + 10, f + 25)]


# a weapon chosen on the wheel before a route step: {(segment index, step index): weapon}
L2_TAPS = {(4, 0): 8,       # 129's soul gate: the shotgun (the grenade launcher's grenades fly over the gate)
           (6, 2): 4,       # 133's zombies 22-24 (L2_133_REAL): the mag 60 (the pistol's slot)
           (6, 6): 15,      # 133's sister 1: the grenade launcher
           (9, 0): 10 if L2_PLASMA else 4,   # 134's zombies: the plasma rifle (round 17 f5) / the mag 60
           (9, 3): 10 if L2_PLASMA else 8}   # 134's sister 2: the plasma rifle / the shotgun
# round 16 e5: 133 in a --real-weapons 1,2 run: the zombies of the second room (22, 23, 24: 80 health
# each, they open door 28; the nearest awake actor, T2_FIRE_GATE), then sister 1 (20, 50 health). The round-12 steps
# (the sister aimed from the start, the nearest actor else) spent all the ammo on doors 27 and 28.
L2_133_REAL = [
    # zombies 9, 16, 17 (5 health) wake here (region info 4) and are left behind door 27: no fire
    (0, -1504.0, -4761.6, -972.0, -1.5708, 0, {"trig": 1}),
    # the shotgun (+10 shells) at (-153.6, -4761.6, -1075.2) after the first shells are spent (at the
    # segment's start the shells are full and it is not taken)
    (1200, -213.6, -4761.6, -1075.2, -1.5708, 40, {}),
    (1400, -847.0, -4761.6, 1574.0, 3.1416, 0, {"trig": 1, "fire": 1500, "aim": "enemy"}),
    (2950, -847.0, -4761.6, 1574.0, 3.1416, 0, {"fire": 1500, "aim": "enemy"}),
    (4500, -847.0, -4761.6, 1574.0, 3.1416, 0, {"fire": 1500, "aim": "enemy"}),
    # the mag 60 (+bullets) on its ledge (-1996.5, -4454.4, 447.7), the bullets spent by now
    (5850, -1996.5, -4454.4, 537.7, 0.0, 50, {}),
    (6050, -847.0, -4761.6, 1574.0, 3.1416, 0, {"fire": 1900, "aim": "enemy:20"}),
    (8000, 827.5, -4761.6, -1627.5, -2.8692, 150, {}),    # warp 2615 -> 130
]
L2_134_GATE = os.environ.get("T2H3_134") == "gate"   # round 19 h3: DEBUG-ONLY test hook (see main_all)
L2_134_FIRE_R = 450     # round 17 f5: 134's fire range in a --real-weapons 2 run (T2_FIRE_GATE, T2_AIMD_FIRE_MAX)
L2_STRETCH_REAL = [(6, 6, 3000), (9, 3, 2000), (12, 15, 2000)]


def with_taps(segs, taps):
    out = []
    for i, (name, rel, steps) in enumerate(segs):
        steps = list(steps)
        for (si, sj), t in taps.items():
            if si == i:
                st = list(steps[sj]) + [{}] * (7 - len(steps[sj]))
                st[6] = dict(st[6], weapon=t)
                steps[sj] = tuple(st)
        out.append((name, rel, steps))
    return out


def arms_frames(lv):
    return sum(len(v) for v in ARMS[lv].values()) * KEY_STEP


def arms_switch(r, lv, t, real):
    """The wheel choices of level lv (ARMS_SWITCH), from frame t."""
    if lv not in real:
        return
    for dt, w in ARMS_SWITCH[lv]:
        r.inputs += wheel(w, t + dt)


def main_all(out, upto=6, extras=True, hubs_in=None, no_cheats=(), extra_env=(), health=True, cheats_on=(),
             talismans=(), walk=(), walk_probe=False, real_weapons=()):
    """hubs: the measured hub arrivals {after level: frame} (HUB_AT). no_cheats: the levels played
    without T2_CHEATS invincibility (each from its hub visit before it); extra_env: more env lines.
    talismans: the levels whose chamber trip is made (TALISMAN: 2, 3; each moves the later hub
    arrivals by TALISMAN_TRIP). real_weapons (round 15 d2): the levels (1, 2, 3, from 1 on) played on
    the weapons and ammo picked up on the way (ARMS; each pick moves the later hub arrivals by KEY_STEP)."""
    hubs = dict(HUB_AT)
    for lv in talismans:
        for k in hubs:
            if k >= lv:
                hubs[k] += TALISMAN_TRIP
    real = set(real_weapons)
    assert real == set(range(1, len(real) + 1)) and len(real) <= 3, "--real-weapons: 1 / 1,2 / 1,2,3"
    for lv in sorted(real):
        # the hub arrival after a level played on its own weapons (its totem won later or earlier than
        # with the plasma rifle): REAL_HUB_AT, measured; else the default moved by the picks
        d = REAL_HUB_AT[lv] - hubs[lv] if lv in REAL_HUB_AT else arms_frames(lv)
        for k in hubs:
            if k >= lv:
                hubs[k] += d
    hubs.update(hubs_in or {})
    det = {lv: {TALISMAN[lv][0]: lv} for lv in talismans}
    r = Route()
    r.no_cheats = set(no_cheats)
    r.cheats_on = set(cheats_on)
    r.walk = set(walk)
    r.walk_probe = walk_probe
    r.extra_env = list(extra_env)
    for lv in (2, 3, 4, 5):
        r.level_start[lv] = hubs[lv - 1]
    r.saves_at = []
    r.health = health
    # the health pickups only in the levels played without invincibility (with it the route stays
    # round 12's: the picks move every later step, and the Primagen's lair wait depends on the timing)
    arrival, added = level1(r, health=health and 1 in r.no_cheats, arms=1 in real)
    arms_switch(r, 1, arrival, real)
    if 1 in real:   # no fire taps in the hub (the ammo kept for level 2)
        r.fire[-1] = (r.fire[-1][0], min(r.fire[-1][1], hubs[1] - ARMS_FIRE_END))
    if real == {1}:
        r.real_until = hubs[1]
    r.note(hubs[1], "60 the hub (after level 1)")
    # round 15 d2: a --real-weapons run may idle ARMS_HUB_WAIT frames in the hub (the anchor is earlier, the
    # steps are not): the level-2 fights depend on the game state they start from
    r.anchors.append((hubs[1] - (ARMS_HUB_WAIT if 1 in real else 0), 60))
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
    if N64_SHAKE:
        l2stretch = merge_stretch(l2stretch, N64_L2_STRETCH)
    if 2 in real:
        # round 16 e5: 133 as round 13 b2's zombies-one-by-one steps (pt.L2_SEGMENTS), the wheel per
        # step (L2_TAPS), and the aims never on a door (T2_AIM_SKIP_TYPES=6)
        name, rel, steps = l2segs[6]
        l2segs = list(l2segs)
        l2segs[6] = (name, rel, L2_133_REAL)
        l2stretch = L2_STRETCH_REAL
        l2segs = with_taps(l2segs, L2_TAPS)
        if L2_134_GATE:
            # round 19 h3 (--l2-134 gate): only zombie 4 (it holds gate 8's key 7) is fought: from the segment's
            # start at the gate spot, zombie 4 the assist's (and the stick's primary) target; zombies 2 and 3 are
            # shot only when they come within the fire range. Then sister 2 and the exit as before.
            name, rel, steps = l2segs[9]
            gate = steps[2]
            l2segs[9] = (name, rel, [st[:1] + tuple(gate[1:6]) + (dict(st[6], aim="enemy:4"),) for st in steps[:2]] +
                         list(steps[2:]))   # the step count kept (L2_STRETCH_REAL, the retries index them)
            STICK_PRIMARY[2] = tuple(STICK_PRIMARY.get(2, ())) + (134,)
        r.extra_env.append("T2_AIM_SKIP_TYPES=6")
        # round 17 f5: in 134 the zombies walk up from 1,700 units: no fire beyond 450 (the mag 60 emptied
        # on zombie 2 from afar); the stick driver's own fire likewise (T2_AIMD_FIRE_MAX)
        r.extra_env.append("T2_FIRE_GATE=133,134:%d,135" % L2_134_FIRE_R)
        r.extra_env.append("T2_AIMD_FIRE_MAX=134:%d" % L2_134_FIRE_R)
    l2retry = L2_RETRY + L2_RETRY_B5 + (L2_HEALTH_LATE if h2 else [])
    l2back = L2_RETURN_LATE if h2 else ()
    if N64_SHAKE and N64_L2_PICKS:   # round 17 f1: health picks between 135's crypt steps
        picks = N64_L2_PICKS + (N64_FULL_L2_PICKS if n64_full() and h2 else [])   # round 18 g1: 133's too
        l2segs, l2stretch, l2retry, l2back = insert_picks(l2segs, picks, l2stretch, l2retry, l2back)
    heal = 0
    if L2_HEAL:   # round 17 f5 (after f1's picks: these go before 135's exit step)
        l2segs, heal = l2_heal(l2segs)
    a2 = level_segments(r, l2segs, l2s, merged(L2_KEYS, N64_FULL_L2_HEALTH if h2 and n64_full() else {}, L2_HEALTH if h2 else {}, L2_ARMS if 2 in real else {}) if extras else {},
                        l2retry, l2stretch,
                        back=l2back, detours=det.get(2),
                        walks=nav_table(2) if 2 in NAV else L2_WALKS if 2 in walk else None, nav_walks=2 in NAV)
    if n64_full():   # round 18 g1: weapons per level-2 segment on the N64's stream (N64_L2_WEAPON)
        for si, (w, dt) in sorted(N64_L2_WEAPON.items()):
            r.weapon.append((r.seg_starts[si] + dt, w))
            r.weapon.append((r.seg_starts[si + 1] - 50, "10"))
    a2 += heal
    if n64_full() and h2:   # round 18 g1: 133's and 130's health picks move the later hub arrivals as well
        heal += KEY_STEP * (sum(len(it) for si, sj, it in N64_FULL_L2_PICKS) + sum(len(v) for v in N64_FULL_L2_HEALTH.values()))
    for k in hubs:   # round 17 f5: --l2-heal's picks move the City Totem and every later hub arrival
        if k >= 2:
            hubs[k] += heal
    t_arr = pt.L2_TOTEM_ARRIVAL + shift + a2
    r.note(t_arr, "110 the City Totem: fire taps (T2_FACE_NEAREST aims)")
    arms_switch(r, 2, t_arr, real)
    if real == {1, 2}:
        r.real_until = hubs[2]
    if 110 in r.cheats_on:
        r.cheat_windows.append((t_arr - 600, hubs[2]))
    r.fire.append((t_arr, t_arr + 7200))
    r.fights["110"] = (t_arr - CITY_TOTEM_TP_AFTER, t_arr + 7200)
    r.fight_level["110"] = 2
    r.anchors.append((t_arr, 110))   # the fire window starts when 110 is reached
    # Round 13 b2: down from the arrival ledge (642 units from the totem, the attackers 450-1,150
    # away: 4-5 kills of 8 in 120 s) to the south side of the totem, where they come to the player
    r.tp(t_arr + CITY_TOTEM_TP_AFTER, *pt.CITY_TOTEM_SPOT)
    r.face_levels.append(110)
    r.note(hubs[2], "60 the hub (after level 2)")
    r.anchors.append((hubs[2], 60))
    if upto < 3:
        hub(r, hubs[2], grant=GRANT[2], **key_visit(2))
        return r.write(out)
    c = hub(r, hubs[2], grant=GRANT[2], go=3, **key_visit(2))
    shift = c - OLD_CINEMA[3]
    r.anchors.append((c, 12))
    r.note(c, "12.. the Death Marshes intro cinemas (level 3 route shifted by %d)" % shift)
    l3s = pt.L3_START + shift
    h3 = health and 3 in r.no_cheats
    a3 = level_segments(r, l3_segments(3 in r.no_cheats), l3s,
                        merged(L3_KEYS, L3_HEALTH if h3 else {}, L3_ARMS if 3 in real else {}) if extras else {}, anchor=L3_ANCHOR, detours=det.get(3),
                        walks=nav_table(3) if 3 in NAV else None, nav_walks=3 in NAV)
    a3x = sum(len(v) for v in L3_HEALTH.values()) * KEY_STEP if h3 and extras else 0
    for k in hubs:   # round 14 c1: the hub anchors after level 3 move with its health picks
        if k >= 3:
            hubs[k] += a3x
            if k + 1 in r.level_start:
                r.level_start[k + 1] = hubs[k]
    rel, length, lv = pt.L3_TOTEM
    arms_switch(r, 3, l3s + rel + a3, real)
    if real == {1, 2, 3}:
        r.real_until = hubs[3]
    r.note(l3s + rel + a3, "111 the Marsh Totem: fire taps")
    r.fire.append((l3s + rel + a3, l3s + rel + a3 + length))
    r.fights["111"] = (l3s + rel + a3, l3s + rel + a3 + length)
    r.fight_level["111"] = 3
    if 3 in r.no_cheats:
        # round 14 c1: the scorpion launcher (16) for the Marsh Totem (scheduled in 68, before the
        # totem's level loads; see level 4): 15 damage instead of 62-272 and a life
        r.weapon.append((l3s + rel + a3 - TOTEM_WEAPON_BEFORE, "16"))
        r.weapon.append((hubs[3] - 200, "10"))
    r.face_levels.append(lv)
    r.note(hubs[3], "60 the hub (after level 3)")
    r.anchors.append((hubs[3], 60))
    if upto < 4:
        hub(r, hubs[3], grant=GRANT[3], **key_visit(3))
        return r.write(out)
    r.shake_switch[4] = hubs[3]
    c = hub(r, hubs[3], grant=GRANT[3], go=4, **key_visit(3))
    shift = c - OLD_CINEMA[4]
    r.anchors.append((c, 18))
    r.note(c, "18.. the Lair of the Blind intro cinemas (level 4 route shifted by %d)" % shift)
    l4s = pt.L4_START + shift
    h4 = health and 4 in r.no_cheats
    a4 = level_segments(r, pt.L4_SEGMENTS, l4s, merged(L4_KEYS, L4_HEALTH if h4 else {}) if extras else {},
                        anchor=L4_ANCHOR, walks=nav_table(4) if 4 in NAV else None, nav_walks=4 in NAV)
    a4x = sum(len(v) for v in L4_HEALTH.values()) * KEY_STEP if h4 and extras else 0
    for k in hubs:   # round 14 c1: the hub anchors after level 4 move with its health picks
        if k >= 4:
            hubs[k] += a4x
            if k + 1 in r.level_start:
                r.level_start[k + 1] = hubs[k]
    rel, length, lv = pt.L4_TOTEM
    r.note(l4s + rel + a4, "112 the Cave Totem: fire taps")
    r.fire.append((l4s + rel + a4, l4s + rel + a4 + length))
    r.fights["112"] = (l4s + rel + a4, l4s + rel + a4 + length)
    r.fight_level["112"] = 4
    if 4 in r.no_cheats:
        # round 14 c1: the scorpion launcher (16) for the Cave Totem and the Blind One, the plasma
        # rifle again from the hub. T2_WEAPON is applied when a level loads (T2_GIVE_WEAPONS), so
        # the switch is scheduled in 103, before the totem's level loads. The plasma rifle: the
        # totem 200 damage and a life lost, the Blind One never hurt (hp 12,800/12,800 at the end);
        # the scorpion launcher: the totem 88 damage, the Blind One dead in 5,200 frames
        r.weapon.append((l4s + rel + a4 - TOTEM_WEAPON_BEFORE, N64_CAVE_WEAPON if n64(4) else "16"))
        r.weapon.append((hubs[4] - 200, "10"))
    r.face_levels.append(lv)
    if real or n64(4):
        # round 17 f2: on the N64's stream too (the Cave Totem costs ~100 health instead of ~55 there and
        # the scorpion launcher leaves 11 health after the Blind One: a life lost on 124's pylons)
        # round 16 e5: the shredder (9) at the Blind One in a --real-weapons run. T2_WEAPON is applied when
        # a level loads: scheduled inside the Cave Totem's fire window (after 112 has loaded), it is
        # applied when her arena (level 1) loads. With the scorpion launcher her minions (intelligence
        # 7401, type 0x1ff) bite the player on the ledge ~1 every 40 frames until she dies: 121 damage,
        # 25 health left, and 124's pylons then kill him (game over at 150,990). The shredder's discs
        # bounce into the minions too: 32 damage, she dies ~1,500 frames earlier, 114 health left.
        r.weapon.append((l4s + pt.L4_TOTEM[0] + a4 + BLIND_WEAPON_AFTER, "9"))
    rel, length, (x, y, z, ry), lv = pt.L4_BOSS
    f = l4s + rel + a4
    r.note(f, "1 the Blind One: fire held, T2_AIM_NODE=boss@1")
    r.fights["1"] = (f, hubs[4])
    r.fight_level["1"] = 4
    r.inputs.append("Z@%d..%d" % (f + 10, f + length))
    r.tp(f, x, y, z, ry)
    r.aim_node = "boss@1/2/3/4"   # the Blind One 1, the Mantid Queen 2, the Mother 3, the Primagen 4
    r.note(hubs[4], "60 the hub (after level 4)")
    r.anchors.append((hubs[4], 60))
    r.shake_switch[5] = hubs[4]
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
MOTHER_SHREDDER_END = 16000   # round 15 d1: the shredder's fire window at the Mother (her death ~+7,500 standalone)
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
    if 5 in r.no_cheats and getattr(r, "health", True):
        # round 14 c2: the ultra healths of 92 and 95 (every later step moves by the time added)
        segs, more = r56.with_items(segs, r56.L5_HEALTH)
        add += more
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
    if n64_full() and N64_QUEEN_WEAPON:   # round 18 g1: requested in 113, taken when her level loads
        r.weapon.append((s + L5_QUEEN_AT + add - 800, N64_QUEEN_WEAPON))
    r.anchors.append((s + L5_QUEEN_AT + add, 2))
    r.note(s + rel + add, "2 the Mantid Queen: fire held, T2_AIM_NODE=boss@2, T2_BOSS_FRONT=450,200")
    r.inputs.append("Z@%d..%d" % (s + rel + add, s + rel + add + length))
    r.boss_front.append((s + rel + add - 100, "450,200"))
    hub_at = s + L5_HUB_AFTER + add
    r.level_start[6] = hub_at
    r.shake_switch[6] = hub_at
    r.note(hub_at, "60 the hub (after level 5)")
    r.anchors.append((hub_at, 60))
    # round 14 c3: with --late-keys, Primagen keys 2 and 3 go in here, with key 5 (PRIMAGEN_LATE)
    return hub(r, hub_at, primagen=PRIMAGEN_LATE + (5,), go=6)   # a save, the keys, the level-6 door and portal


def level6(r, t):
    """t: the frame the hub portal 6000 warps (hub(..., go=6))."""
    s = t + WARP_TO_START[6]
    items = r56.L6_ITEMS
    if n64(6):
        # round 17 f2: on the N64's stream the Queen leaves 13-87 health and 117's fights cost up to ~100
        # (a life lost in the --n64-shake-from 5 run): one of 114's two other ultra healths at the start
        items = dict(items)
        items[0] = r56.L6_N64_HEALTH + items.get(0, [])
    segs, add = r56.with_items(r56.L6_SEGMENTS, items)
    r.anchors.append((s - LOAD_EARLY, 114))
    r.note(s - LOAD_EARLY, "114.. the Lightship (level 6 route from %d, its items picked up: +%d)" % (s, add))
    r.weapon.append((t, "10"))
    r.fill.append((t, "600"))
    r.steps(segs, s)
    m = s + r56.L6_MOTHER + add
    r.anchors.append((m, 3))
    r.note(m, "3 the Mother: fire held, T2_AIM_NODE=boss@3, strafing with steps back")
    fire_end = m + L6_BACK_114
    if 3 not in r.cheats_on:
        # round 15 d1: the Mother without invincibility: the shredder (9, level 5's weapon). In her
        # phase 0 the player cannot move and she hits him (10) every ~80 frames until her six parts are
        # down: the plasma rifle takes ~3,500 frames (16-20 hits), the shredder ~2,000 (3 hits); her
        # whole fight ~7,500 frames, 38 damage (standalone level 6; PLAYTHROUGH.md 23). The fire stops
        # after her death (the shredder's discs bounce) and the plasma rifle comes back for the hub.
        r.weapon.append((m - 100, "9"))
        r.weapon.append((m + MOTHER_SHREDDER_END, "10"))
        fire_end = m + MOTHER_SHREDDER_END
    # round 17 f2: on the N64's stream the strafing starts earlier (N64_MOTHER_MOVE): standing still at
    # ~+5,400 (her mode 19-25 attack) cost up to four 20s there, one 20 on the legacy stream
    move = N64_MOTHER_MOVE if n64(6) else 5800
    r.inputs += ["Z@%d..%d" % (m + 800, fire_end), "CL@%d..%d" % (m + move, m + 15000)]
    r.inputs += ["CD@%d..%d" % (f, f + 250) for f in range(m + move, s + r56.L6_MOTHER_END + add, 600)]
    back = m + L6_BACK_114
    if 3 in r.cheats_on:
        # round 14 c2: the Mother invincible in a --cheats-off 6 run (--cheats-on 3): she costs ~250-330
        # health in rescue runs (her 10-damage hits from ~1,240 units), more than the 200 the route has
        r.cheat_windows.append((m - 200, back))
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
    r.note(f, "60 the hub: Primagen key 6 slot: the six keys in: the hub portal cinema (43) -> the Primagen")
    primagen_slot(r, f, 6)
    lair = f + L6_CINEMA
    r.shake_switch[7] = lair - 300
    if HOP:   # DEBUG-ONLY (--hop): round 13's warp 7602, for a run whose six keys are not all in
        r.warps.append((lair + 150, 7602))
    r.anchors.append((lair, 4))
    if 4 in r.cheats_on:
        # round 14 c2: the Primagen invincible in a --cheats-off 6 run (--cheats-on 4): ~280 damage and
        # a life lost in a rescue run (the ending's own life loss at his death is not this)
        r.cheat_windows.append((lair - 200, lair + L6_HUB_END))
    r.note(lair, "4 the Primagen's lair: fire held, strafing, T2_BOSS_FRONT=450,3000,2.5,1")
    # round 18 g5: the lair as a fight (--assist-keep lair: the assists' yaw in a --stick-aim 6 run)
    r.fights["lair"] = (lair, lair + L6_HUB_END)
    r.fight_level["lair"] = 6
    if LAIR_PUSH:   # round 18 g5 (--lair-push): aimdrive.py's LairDrive against the Primagen's push (mode 20)
        r.extra_env.append("T2_LAIR_PUSH=%s" % LAIR_PUSH)
    if 4 not in r.cheats_on:
        # round 15 d1: the Primagen without invincibility: the shredder (9) as at the Mother. The
        # Primagen recipe (T2_WARP_TO=7602): his death at 27,856 instead of 43,586 (plasma rifle), his
        # melee (two 10s within 409.6 / 716.8, Primagen_HandleAnimEvent) 2 hits instead of 28
        r.weapon.append((lair - 100, "9"))
    # the spot cycles (",K,1": another one each 3,000 frames of a lair wait that does not end; round 14 C29:
    # 600 -> 3,000: the strafing by the lair start (lair_strafe) ends his home waits, the front spots do not)
    drive = LAIR_DRIVE or ("guard" if 4 not in r.cheats_on else "none")
    strafe = LAIR_STRAFE or ((90, 0) if 4 not in r.cheats_on else (80, 0))
    # with the lair driver: no relocation at all (an empty spec ends the Queen's, which would act in the lair)
    r.boss_front.append((lair - 100, "" if drive != "none" else LAIR_FRONT))
    r.inputs.append("Z@%d..%d" % (lair + 200, lair + L6_HUB_END))
    if drive != "none":
        # round 15 C30 (--lair-drive): tools/port/aimdrive.py's LairDrive answers the C buttons from the
        # player's position (closed loop; run the game under aimdrive.py, PLAYTHROUGH.md 26) until about
        # his death. "guard": the strafing below plays, and the driver takes over only near the floor's
        # edge (d1's fall off the right edge at 228,464); "arcs": it walks arcs round his home instead
        r.move_drive.append((lair + 200, lair + L6_HUB_END - 18000))
        r.extra_env.append("T2_LAIR_DRIVE=%s" % drive)
        if drive == "guard" and n64(7) and LAIR_KEEP:
            # round 17 f2: on the N64's stream, walk away from him while he is within LAIR_KEEP (his melee)
            r.extra_env.append("T2_LAIR_KEEP=%d" % LAIR_KEEP)
        # DEBUG-ONLY (port/game/t2bosslog.c): a home wait not ended after LAIR_PEEK frames (his hover hides
        # him from the whole floor) is ended by peeks from over the front pits, the player put back each time
        r.extra_env.append("T2_LAIR_PEEK=\"%d@%d\"" % (LAIR_PEEK, lair - 100))
    if drive in ("none", "guard"):
        # round 14 C29: strafing until his death (his home waits need a sight line: routes56.lair_strafe)
        r.inputs += r56.lair_strafe(lair + 200, lair + L6_HUB_END - 18000, *strafe)
    end = lair + L6_HUB_END
    r.note(end - 21000, "the Primagen dead (~30,000 after his lair): the ending cinema, then the credits")
    r.note(end, "60 the hub (after the credits)")
    r.end = end + 1500
    return r.end


LEVEL_HOOKS = {5: level5, 6: level6}


def cli(argv=None):
    """The command line (also `playthrough.py OUT --all [these options]`, round 13 C28)."""
    global LAIR_PUSH
    global NAV, NAV_ONLY, L6_SLOT_AFTER, HOP, PRIMAGEN_LATE, LAIR_STRAFE, LAIR_FRONT, LAIR_DRIVE, LAIR_PEEK, STICK_AIM, ASSIST_KEEP, ARMS_HUB_WAIT, N64_SHAKE, N64_PROPS, N64_SOUND, N64_FROM, LAIR_KEEP, L2_HEAL
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--upto", type=int, default=6)
    ap.add_argument("--no-extras", action="store_true")
    ap.add_argument("--hub", action="append", default=[], help="N=FRAME: the hub arrival after level N")
    ap.add_argument("--cheats-off", default="", help="L1,L2,...: the levels played without T2_CHEATS invincibility (round 13 b5)")
    ap.add_argument("--env", action="append", default=[], help="K=V: one more line for teleport.env (e.g. T2_HURT_LOG=1)")
    ap.add_argument("--cheats-on", default="", help="110: the City Totem invincible even in a --cheats-off level (round 13 b5); "
                    "3 / 4: the Mother / the Primagen in a --cheats-off 6 run (round 14 c2)")
    ap.add_argument("--no-health", action="store_true", help="skip the health pickups (round 13 b5)")
    ap.add_argument("--walk", default="", help="L1,L2: walk with the stick instead of the open-floor teleports "
                    "of L1_WALKS/L2_WALKS (levels 1, 2; round 15 d5)")
    ap.add_argument("--walk-probe", action="store_true", help="with --walk: teleport too at each walk's end "
                    "(the teleport log line gives the arrival; every walk starts from the default route's place)")
    ap.add_argument("--nav", default="", help="L1,L2,...: the route teleports of NAV_WALKS in these levels (1-4) "
                    "walked by the closed-loop waypoint walker (run under tools/port/aimdrive.py; round 16 e3)")
    ap.add_argument("--nav-only", default=None, help="L:I.J,...: with --nav, only these walks (level, segment, "
                    "step), NAV_SKIP's included")
    ap.add_argument("--talismans", nargs="?", const="2,3", default="",
                    help="the talisman chambers of these levels (default 2,3; round 13 b3)")
    ap.add_argument("--slot", type=int, default=L6_SLOT_AFTER, help="Primagen key 6 into its slot, frames after the hub (level 6)")
    ap.add_argument("--late-keys", action="store_true",
                    help="Primagen keys 2 and 3 in after level 5, round 13's walks after levels 2 and 3 (round 14 c3)")
    ap.add_argument("--lair-push", default="", help="toward|jump|both: the lair driver answers the Primagen's push "
                    "(his mode 20) by walking towards him and/or jumping (round 18 g5; needs the lair driver)")
    ap.add_argument("--stick-aim", default="", help="L1,L2,...: these levels aim with the stick only, closed-loop "
                    "(run under tools/port/aimdrive.py; round 15 d3)")
    ap.add_argument("--stick-boss", default=None, help="L,...: with --stick-aim, the boss arenas of these levels (5: "
                    "the Mantid Queen, 6: the Primagen's lair) aim only at the boss's node unless a candidate comes "
                    "near (round 19 h3); default 5,6 on the N64 stream, none with --legacy; \"\" = none")
    ap.add_argument("--assist-keep", default="", help="with --stick-aim: these fights keep the aim assists "
                    "(1: the Blind One, 110: the City Totem; round 15 d3)")
    ap.add_argument("--hop", action="store_true", help="DEBUG-ONLY: round 13's warp 7602 after the slot (no cinema 43)")
    ap.add_argument("--lair-strafe", default=None, help="P[,S]: the lair strafing's period (C-left/C-right) and an "
                    "optional z sweep of S frames (C-down then C-up; round 15 d1); default 90 with the Primagen "
                    "not invincible, else 80")
    ap.add_argument("--lair-drive", default=None, choices=("guard", "arcs", "none"),
                    help="the closed-loop lair driver (run under tools/port/aimdrive.py; round 15 C30): guard (the "
                    "default without --cheats-on 4: the strafing, kept off the floor's edge), arcs, none (the "
                    "default with --cheats-on 4: the timed strafing and T2_BOSS_FRONT)")
    ap.add_argument("--lair-peek", type=int, default=LAIR_PEEK, help="with the lair driver: T2_LAIR_PEEK, the "
                    "frames of one home wait before the peeks from over the front pits (round 15 C30)")
    ap.add_argument("--lair-front", default=LAIR_FRONT, help="T2_BOSS_FRONT in the Primagen lair (round 15 d1)")
    ap.add_argument("--real-weapons", default="", help="1 / 1,2 / 1,2,3: these levels on the weapons and ammo "
                    "picked up on the way, no T2_GIVE_WEAPONS / T2_FILL_AMMO before the hub after them (round 15 d2)")
    ap.add_argument("--n64-shake", action="store_true", help="the N64's camera shake (no T2_LEGACY_SHAKE) and "
                    "the route changes it needs (N64_L2_STRETCH, N64_L2_PICKS; PLAYTHROUGH.md 31, 32)")
    ap.add_argument("--n64-props", action="store_true", help="the type-9 props as on the N64 (no "
                    "T2_LEGACY_INTEL9; round 17 f3, FIDELITY.md 7): not tuned yet")
    ap.add_argument("--n64-sound", action="store_true", help="the sound world in the headless run (no "
                    "T2_LEGACY_NOSOUND: the sound requests draw their random numbers as with --audio and on the "
                    "N64; round 17 merge): not tuned yet")
    ap.add_argument("--n64", action="store_true", help="no legacy switch at all: --n64-shake --n64-props "
                    "--n64-sound together, the stream a human player gets (round 18 g1; the default since the "
                    "round 18 merge: kept as an explicit spelling)")
    ap.add_argument("--legacy", action="store_true", help="the old stream the routes before round 18 were tuned on "
                    "(round 18 merge): T2_LEGACY_SHAKE, T2_LEGACY_INTEL9 and T2_LEGACY_NOSOUND; --n64-shake, "
                    "--n64-props, --n64-sound then lift one each (the validated combinations of rounds 13-18)")
    ap.add_argument("--n64-shake-from", type=int, default=None, choices=(4, 5, 6, 7),
                    help="DEBUG-ONLY (round 17 f2): the old stream (T2_LEGACY_SHAKE) up to the hub visit before "
                    "level L (7: the Primagen's lair), the N64's from there; with --n64-shake's level 4-6 changes")
    ap.add_argument("--lair-keep", type=int, default=LAIR_KEEP, help="on the N64's stream (--n64-shake, "
                    "--n64-shake-from): the lair driver walks away from him within this distance (0: off; round 17 f2)")
    ap.add_argument("--l2-heal", action="store_true", help="135: the health +10 pickups after sister 3, before "
                    "the City Totem (round 17 f5, L2_HEAL_PICKS)")
    ap.add_argument("--hub-wait", type=int, default=ARMS_HUB_WAIT,
                    help="--real-weapons 1: frames idled in the hub after level 1 (default %d)" % ARMS_HUB_WAIT)
    a = ap.parse_args(argv)
    # round 18 merge: the N64's stream (no T2_LEGACY_* line, g1's --n64) is the default; --legacy is the old one
    if a.legacy and a.n64:
        ap.error("--legacy and --n64 exclude each other")
    if a.n64_shake_from is not None and not a.legacy:
        ap.error("--n64-shake-from needs --legacy (the old stream up to level L)")
    N64_SHAKE = a.n64_shake or not a.legacy
    N64_PROPS = a.n64_props or not a.legacy
    N64_SOUND = a.n64_sound or not a.legacy
    N64_FROM = a.n64_shake_from
    LAIR_KEEP = a.lair_keep
    LAIR_FRONT = a.lair_front
    LAIR_DRIVE = a.lair_drive
    LAIR_PEEK = a.lair_peek
    LAIR_STRAFE = tuple(int(v) for v in (a.lair_strafe + ",0").split(",")[:2]) if a.lair_strafe else None
    L6_SLOT_AFTER = a.slot
    ARMS_HUB_WAIT = a.hub_wait
    L2_HEAL = a.l2_heal
    HOP = a.hop
    STICK_AIM = {int(x) for x in a.stick_aim.split(",") if x}
    LAIR_PUSH = a.lair_push
    NAV = {int(x) for x in a.nav.split(",") if x}
    if a.nav_only is not None:
        NAV_ONLY = {tuple(int(v) for v in k.replace(":", ".").split(".")) for k in a.nav_only.split(",") if k}
    ASSIST_KEEP = [x for x in a.assist_keep.split(",") if x]
    global STICK_BOSS_ON
    STICK_BOSS_ON = None if a.stick_boss is None else {int(x) for x in a.stick_boss.split(",") if x}
    if a.late_keys:
        PRIMAGEN_LATE = (2, 3)
    main_all(a.out, a.upto, not a.no_extras, {int(h.split("=")[0]): int(h.split("=")[1]) for h in a.hub},
             [int(x) for x in a.cheats_off.split(",") if x], a.env, not a.no_health,
             [int(x) for x in a.cheats_on.split(",") if x], [int(v) for v in a.talismans.split(",") if v],
             [int(v) for v in a.walk.split(",") if v], a.walk_probe,
             [int(v) for v in a.real_weapons.split(",") if v])


if __name__ == "__main__":
    cli()
