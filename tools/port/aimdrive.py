#!/usr/bin/env python3
"""Closed-loop stick aiming for a headless run (round 15 d3, docs/port/PLAYTHROUGH.md section 24).

    python3 tools/port/aimdrive.py [--simple] [--probe SX,SY] [--stats FILE] -- turok2 ARGS...

Runs the game (stdout and stderr pass through) with T2_AIM_FIFO pointing at two fifos in a fresh
temporary directory. Inside the T2_AIM_STICK windows the aim assists only choose a target; after
each such frame the game writes one line, "aim F px py pz rotY ex ey ez pitch tx ty tz" (the
player position and yaw, the camera eye and pitch, the target point; exact hex floats), and waits
for the next frame's stick, which this driver computes from that line alone: the yaw error through
the measured turn rate of the stick (playthrough.TURN_FULL, cube law, dead zone 3), the pitch error
likewise (PITCH_EASE below: the pitch follows a look angle). Nothing else of the game is read and nothing is written into it: the
same input a player's thumb gives. Deterministic: the same lines give the same answers.

T2_MOVE_DRIVE windows (round 15 C30): "move F px py pz rotY bx by bz" lines get C buttons back
from LairDrive (the Primagen's lair: keep moving over the floor, never off it).

Round 16 e2: the "aim" line also carries the candidate targets ("n", then "id x y z hp kind" each,
nearest first: every target the assist would accept, a boss's damaging node boxes) and the player's
health. The driver picks its own target among them (Picker: the one nearest the crosshair, distance
and the current target weighed in), leads a moving one (its velocity from the candidates' ids) and
fires: its answer "SX SY z1" holds Z only on target ("z0" otherwise: the script's Z taps are
overridden on every aim frame). T2_AIMD_PICK=0 aims at the assist's own target as round 15 did;
T2_AIMD_FIRE=0 leaves the script's Z alone (round 15's answers: "SX SY").

The answer protocol (round 16 merge; game.cpp, AimFifoExchange): "SX SY [TOKEN ...]", SX SY the
stick or "- -" (keep the script's); each token sets one button group over the script's for the next
frame: "cH" the C buttons (hex U 8, D 4, L 2, R 1), "z0"/"z1" fire, "r0"/"r1" jump. AimDrive answers
"SX SY [zN]", LairDrive "- - cH", WalkDrive "SX 0 cH [r1]".

--simple: the first controller (the observed error only, no prediction; round 15 d3's s1 run).
--probe SX,SY: answer a constant stick instead (calibration: the pitch/yaw change per frame).
"""
import math, os, shutil, subprocess, sys, tempfile, threading, time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from playthrough import TURN_FULL, STICK_DEAD, wrap, face_yaw  # noqa: E402

# Measured with --probe (round 15 d3, Linux Release, level 51): the game reads the pad every second
# host frame (a game tick) and a stick change shows 2 ticks later. Stick X turns at a rate (cube law:
# playthrough.TURN_FULL per host frame at full reading 80). Stick Y does not turn: the camera pitch
# eases towards a look angle set by the stick, -(reading / 80) * (pi / 2) (+ SY looks down), by
# PITCH_EASE of the difference per tick, and back towards 0 when the stick is released.
PITCH_EASE = 0.0541
TICK = 2
GAIN = 0.8              # the share of the predicted error corrected per tick
LAG = 4                 # host frames from an answer to its effect in the pose (2 ticks)


def axis(err, full, gain, ff=0.0):
    """The stick value that turns by gain * err + ff this host frame (cube law over the reading 0..80)."""
    want = err * gain + ff
    rate = min(abs(want), full)
    if rate < full * 1e-4:
        return 0
    r = 80.0 * (rate / full) ** (1.0 / 3.0)
    v = min(127, int(round(r)) + STICK_DEAD)
    return v if want > 0 else -v


def pitch_stick(err, pitch, gain=GAIN):
    """The stick Y whose look angle moves the pitch by gain * err in the next tick."""
    look = max(-math.pi / 2, min(math.pi / 2, pitch + gain * err / PITCH_EASE))
    r = abs(look) / (math.pi / 2) * 80.0
    if r < 0.5:
        return 0
    v = min(127, int(round(r)) + STICK_DEAD)
    return -v if look > 0 else v


def turn_of(sx):
    """The yaw change per host frame a stick X value gives (the inverse of axis())."""
    r = max(0, abs(sx) - STICK_DEAD)
    t = TURN_FULL * (min(r, 80) / 80.0) ** 3
    return t if sx > 0 else -t


def look_of(sy):
    """The look angle a stick Y value sets (+ SY looks down)."""
    r = max(0, abs(sy) - STICK_DEAD)
    a = min(r, 80) / 80.0 * (math.pi / 2)
    return -a if sy > 0 else a


class Aim:
    """The controller: the error predicted LAG host frames ahead (the answers already sent and not
    yet seen in the pose: a Smith predictor) and the target's own motion (its bearing and elevation
    rates while the target stays the same), then GAIN of it corrected per tick."""

    def __init__(self):
        self.sent = []          # (sx, sy) of the last LAG answers
        self.prev = None        # (frame, bearing, elevation, target point)
        self.rate = (0.0, 0.0)

    def answer(self, frame, px, pz, ry, ex, ey, ez, pitch, tx, ty, tz):
        bearing = face_yaw(px, pz, tx, tz)
        elev = math.atan2(ty - ey, math.hypot(tx - ex, tz - ez))
        if self.prev and frame - self.prev[0] <= 2 and math.dist(self.prev[3], (tx, ty, tz)) < 60.0:
            dt = frame - self.prev[0]
            if dt > 0:
                wb = wrap(bearing - self.prev[1]) / dt
                we = (elev - self.prev[2]) / dt
                self.rate = (0.5 * self.rate[0] + 0.5 * wb, 0.5 * self.rate[1] + 0.5 * we)
        else:
            self.rate = (0.0, 0.0)   # a new target (the answers in flight still count)
        self.prev = (frame, bearing, elev, (tx, ty, tz))
        # where the answers already sent will have taken the yaw and the pitch
        yaw_p, pitch_p = ry, pitch
        for sx, sy in self.sent:
            yaw_p += turn_of(sx)
            pitch_p += PITCH_EASE / TICK * (look_of(sy) - pitch_p)
        eyaw = wrap(bearing + self.rate[0] * LAG - yaw_p)
        epitch = elev + self.rate[1] * LAG - pitch_p
        sx = axis(eyaw, TURN_FULL, GAIN / TICK, self.rate[0])
        sy = pitch_stick(epitch, pitch_p)
        self.sent = (self.sent + [(sx, sy)])[-(LAG - 1):]
        return sx, sy, wrap(bearing - ry), elev - pitch


class SimpleAim:
    """--simple: the first controller (no prediction, no target motion): GAIN_SIMPLE of the observed
    error per tick."""

    def answer(self, frame, px, pz, ry, ex, ey, ez, pitch, tx, ty, tz):
        eyaw = wrap(face_yaw(px, pz, tx, tz) - ry)
        epitch = math.atan2(ty - ey, math.hypot(tx - ex, tz - ez)) - pitch
        return axis(eyaw, TURN_FULL, GAIN_SIMPLE / TICK), pitch_stick(epitch, pitch, GAIN_SIMPLE), eyaw, epitch


GAIN_SIMPLE = 0.35


def envf(name, default):
    v = os.environ.get(name)
    return float(v) if v not in (None, "") else default


# --- round 16 e2: the driver's own target and fire -------------------------------------------------------
PICK = envf("T2_AIMD_PICK", 1.0) != 0
FIRE = envf("T2_AIMD_FIRE", 1.0) != 0
W_ANG = envf("T2_AIMD_WANG", 0.5)          # the crosshair angle's weight in a target's cost (Picker)
W_KEEP = envf("T2_AIMD_WKEEP", 0.75)       # the current target's cost factor: no flicking between two
W_HP = envf("T2_AIMD_WHP", 2.0)            # the target's health (25,600 = 100) in its cost
THREAT_X = envf("T2_AIMD_THREAT", 1.5)     # a kind-0 candidate's cost factor
IMMUNE = int(envf("T2_AIMD_IMMUNE", 300))  # frames without a health drop: the target is not being hurt
IMMUNE_FOR = int(envf("T2_AIMD_IMMUNE_FOR", 900))
IMMUNE_X = envf("T2_AIMD_IMMUNE_X", 4.0)
# a threat outside the assist's set that is not hurt (the Blind One's own body and the actors of
# other phases: out of play) is left for much longer (round 16 e2, s4c: one held the aim 4,600 frames)
IMMUNE_FOR0 = int(envf("T2_AIMD_IMMUNE_FOR0", 3000))
IMMUNE_X0 = envf("T2_AIMD_IMMUNE_X0", 50.0)
# round 17 f5: "not hurt" counted per target over the frames it was aimed at (summed across switches),
# not only while it stays the current target: two targets taken in turn (the Blind One's flier 17 and
# the out-of-play actor 22 at full health) each reset the other's count, and the one that never drops
# was never left. The levels (the aim line's "lv") where it applies: default 1, the Blind One's arena
# (in level 2 it moved the sister fights and the route fell out of step); "" = nowhere (round 16).
IMMUNE_ACC = {int(v) for v in os.environ.get("T2_AIMD_IMMUNE_ACC", "1").split(",") if v.strip()}
PSPEED = envf("T2_AIMD_PSPEED", 150.0)      # the shot's speed for the lead (units per host frame)
LEAD = envf("T2_AIMD_LEAD", 1.0)           # the share of the lead applied
HIT_R = envf("T2_AIMD_HITR", 40.0)         # the radius counted as on target (units)
FIRE_EVERY = int(envf("T2_AIMD_FIRE_EVERY", 15))   # a Z tap of FIRE_HOLD frames every FIRE_EVERY, as the script
FIRE_HOLD = int(envf("T2_AIMD_FIRE_HOLD", 6))
# round 17 f5: T2_AIMD_FIRE_MAX=L:R[,L:R...]: in level L fire only at a target within R units (a --real-weapons
# run's ammo: 134's zombies walk up from 1,700 units; the driver's Z overrides T2_FIRE_GATE's range)
# round 17 f5: in a totem defense (the aim line's "tot"), a kind-2 candidate (running at the totem) costs this
# much (1: as any other; < 1 shoots the totem's attackers first). The defense is lost on time alone (8 kills in
# 120 s, TotemDefense_UpdateFight: the totem itself is never hurt), so the default keeps the nearest/weakest rule.
TOTEM_X = envf("T2_AIMD_TOTEM_X", 1.0)
# round 17 f5: T2_AIMD_THREAT_NEAR=L:R[,L:R...]: in level L a kind-0 candidate farther than R units costs
# THREAT_FAR times more. Kind 0 stands for the enemy biting the player; in the Blind One's arena (level 1) the
# kind-0 actors 400-500 units away (her body, the other phases' actors: never hurt) took 300 aimed frames each
# before the not-hurt rule left them, while her minion bit from the ledge (a --real-weapons run: two lives).
THREAT_NEAR = {int(a): float(b) for a, b in (v.split(":") for v in os.environ.get("T2_AIMD_THREAT_NEAR", "1:250").split(",")
                                             if ":" in v)}
THREAT_FAR = envf("T2_AIMD_THREAT_FAR", 6.0)
FIRE_MAX = {int(a): float(b) for a, b in (v.split(":") for v in os.environ.get("T2_AIMD_FIRE_MAX", "").split(",") if ":" in v)}
# round 18 g5: T2_AIMD_PRIMARY=L[,L...]: in level L the candidate at the assist's own target (the step's objective:
# level 5's cocoons and embryos, the pylons, the Queen's node; level 6's Mother parts) costs PRIMARY_X times as much.
# In 95 the picker spent ~150 of embryo 2's 600 frames on actor 104 (179 units, full health, never hurt) and the
# embryo lived (8,412 of 65,280 left at the window's end): the hive's exit 7500 then leads back to 124, not the
# Hive Totem. The assist's target is the nearest candidate within PRIMARY_R of the aim line's target point.
PRIMARY = {int(v) for v in os.environ.get("T2_AIMD_PRIMARY", "").split(",") if v.strip()}
PRIMARY_X = envf("T2_AIMD_PRIMARY_X", 0.2)
PRIMARY_R = envf("T2_AIMD_PRIMARY_R", 600.0)
# ... and when that candidate is chosen, the aim point is the assist's own point (T2_AIM_NODE's node: the Queen's
# body origin is never hurt, her node is; s6c: 11,100 frames and a life at the Queen aiming at the origin)
# round 18 g5: T2_AIMD_PRIMARY_ONLY=L[,L...]: in level L the stick aims at the assist's point whenever there is one (the
# Mother: her parts are nodes 900-1,500 units from both candidate origins; aimed at the origins her six parts took
# 5,400 frames instead of 2,000 while she hit the pinned player every ~80: 307 damage, 2 lives in s6d)
PRIMARY_ONLY = {int(v) for v in os.environ.get("T2_AIMD_PRIMARY_ONLY", "").split(",") if v.strip()}
CAND_EVERY = int(envf("T2_AIMD_CAND_EVERY", 50))   # --stats: the candidates every this many frames


def parse_cands(f):
    """The candidates after the 13 fixed fields: [(id, x, y, z, hp, kind)] (kind 1: the assist's own
    choice set, 0: a threat outside it within 600 units), and the player's health."""
    if len(f) <= 13:
        return [], None
    n = int(f[13])
    c = []
    for k in range(n):
        i = 14 + 6 * k
        if i + 5 >= len(f) or "hp" in f[i:i + 6]:
            break   # a line cut short
        c.append((int(f[i]), float.fromhex(f[i + 1]), float.fromhex(f[i + 2]), float.fromhex(f[i + 3]), int(f[i + 4]),
                  int(f[i + 5])))
    hp = int(f[f.index("hp") + 1]) if "hp" in f else None
    return c, hp


def line_level(f):
    """The aim line's level ("lv N", round 17 f5), or None."""
    return int(f[f.index("lv") + 1]) if "lv" in f[13:] else None


def line_totem(f):
    """A totem defense in progress ("tot HP TIME KILLS NEED", round 17 f5): (hp, seconds left, kills,
    kills needed), or None."""
    if "tot" not in f[13:]:
        return None
    i = f.index("tot")
    return tuple(int(v) for v in f[i + 1:i + 5])


class Picker:
    """Chooses the target among the candidates as a player would. Cost: (distance / 1,000 + 0.1)
    times (1 + W_ANG * the crosshair's angle to it): the near ones threaten, so they win even
    behind a half-turn, and among those at about the same distance the one under the crosshair
    wins. The current target's cost is multiplied by W_KEEP (no flicking between two). A target
    whose health has not dropped for IMMUNE frames (out of reach, behind a wall, not hurt by this
    weapon) costs IMMUNE_X times more for IMMUNE_FOR frames. Each candidate's velocity comes from
    its last position (same id, no jump); the aim point is led by it over the stick's lag and the
    shot's flight (PSPEED)."""

    def __init__(self):
        self.cur = None
        self.seen = {}          # id -> (frame, (x, y, z), (vx, vy, vz))
        self.switches = 0
        self.hp = None          # the current target's (health, the frame it last dropped or was taken)
        self.immune = {}        # id -> (until frame, cost factor)
        self.immunes = 0
        self.aimed = {}         # IMMUNE_ACC: id -> [health at its last drop, frames aimed at since]
        self.last = None        # the last aim frame

    def vel(self, frame, cid, pos):
        old = self.seen.get(cid)
        v = (0.0, 0.0, 0.0)
        if old and 0 < frame - old[0] <= 4 and math.dist(old[1], pos) < 80.0 * (frame - old[0]):
            dt = frame - old[0]
            raw = tuple((a - b) / dt for a, b in zip(pos, old[1]))
            v = tuple(0.3 * a + 0.7 * b for a, b in zip(raw, old[2]))
        self.seen[cid] = (frame, pos, v)
        return v

    def pick(self, frame, px, pz, ry, ex, ey, ez, pitch, cands, primary, lv=None, totem=None):
        acc = lv in IMMUNE_ACC
        if lv in PRIMARY_ONLY and primary is not None:
            return primary, None
        if not cands:
            self.cur = None
            return primary, None
        best, bcost, pts = None, None, {}
        prim = None
        if lv in PRIMARY and primary is not None:
            near = [(math.dist((x, y, z), primary), cid) for cid, x, y, z, hp, kind in cands]
            near = [c for c in near if c[0] < PRIMARY_R]
            prim = min(near)[1] if near else None
        for cid, x, y, z, hp, kind in cands:
            v = self.vel(frame, cid, (x, y, z))
            d = math.hypot(x - ex, z - ez)
            t = d / PSPEED   # the stick lag is the controller's (Aim: the bearing rate)
            lx, ly, lz = x + LEAD * v[0] * t, y + LEAD * v[1] * t, z + LEAD * v[2] * t
            pts[cid] = (lx, ly, lz)
            if acc:
                rec = self.aimed.get(cid)
                if rec is None or hp < rec[0]:
                    self.aimed[cid] = [hp, 0]
            elif cid == self.cur:
                if self.hp is None or hp < self.hp[0]:
                    self.hp = (hp, frame)
                elif frame - self.hp[1] > IMMUNE and len(cands) > 1:
                    self.immune[cid] = (frame + (IMMUNE_FOR if kind else IMMUNE_FOR0), IMMUNE_X if kind else IMMUNE_X0)
                    self.immunes += 1
                    self.hp = (hp, frame)
            eyaw = wrap(face_yaw(px, pz, lx, lz) - ry)
            epit = math.atan2(ly - ey, math.hypot(lx - ex, lz - ez)) - pitch
            cost = (d / 1000.0 + 0.1) * (1.0 + W_ANG * (abs(eyaw) + 0.5 * abs(epit)))
            if kind == 2 and totem is not None:
                cost *= TOTEM_X   # round 17 f5: an attacker of the totem
            if not kind:
                cost *= THREAT_X
                if d > THREAT_NEAR.get(lv, 1e9):
                    cost *= THREAT_FAR   # a threat outside the assist's set (a boss phase's group, the sister first)
            cost *= 1.0 + W_HP * min(hp, 25600) / 25600.0   # the weak ones first: fewer attackers sooner
            if cid == self.cur:
                cost *= W_KEEP
            if cid == prim:
                cost *= PRIMARY_X   # round 18 g5: the assist's target (T2_AIMD_PRIMARY)
            if self.immune.get(cid, (-1, 1.0))[0] > frame:
                cost *= self.immune[cid][1]
            if bcost is None or cost < bcost:
                best, bcost = cid, cost
        if best != self.cur:
            self.switches += 1
            self.cur = best
            self.hp = None
        if acc:
            rec = self.aimed[best]
            rec[1] += min(frame - self.last, 4) if self.last is not None and frame > self.last else 1
            if rec[1] > IMMUNE and len(cands) > 1:
                kind = [c[5] for c in cands if c[0] == best][0]
                self.immune[best] = (frame + (IMMUNE_FOR if kind else IMMUNE_FOR0), IMMUNE_X if kind else IMMUNE_X0)
                self.immunes += 1
                rec[1] = 0
        self.last = frame
        if best == prim:
            return primary, best   # round 18 g5: the assist's point (a boss node)
        return pts[best], best


class Trigger:
    """Z only on target: the crosshair within HIT_R of the (led) aim point at its distance, in yaw
    and pitch (pitch with half again the margin: the camera eases), then a tap as the script's."""

    def __init__(self):
        self.until = -1
        self.next = -1
        self.shots = 0

    def z(self, frame, eyaw, epitch, dist):
        tol = max(0.03, math.atan2(HIT_R, max(dist, 1.0)))
        if frame < self.until:
            return 1
        if abs(eyaw) < tol and abs(epitch) < 1.5 * tol and frame >= self.next:
            self.until, self.next = frame + FIRE_HOLD, frame + FIRE_EVERY
            self.shots += 1
            return 1
        return 0


def answer(line, aim, probe=None, picker=None, trig=None):
    f = line.split()
    frame = int(f[1])
    px, py, pz, ry, ex, ey, ez, pitch, tx, ty, tz = (float.fromhex(v) for v in f[2:13])
    if probe:
        on = len(probe) < 3 or getattr(answer, "n", 0) < probe[2]
        answer.n = getattr(answer, "n", 0) + 1
        return frame, probe[0] if on else 0, probe[1] if on else 0, 0.0, 0.0, pitch, ry, None, None
    cid = None
    if picker is not None:
        cands, _ = parse_cands(f)
        (tx, ty, tz), cid = picker.pick(frame, px, pz, ry, ex, ey, ez, pitch, cands, (tx, ty, tz), line_level(f), line_totem(f))
    sx, sy, eyaw, epitch = aim.answer(frame, px, pz, ry, ex, ey, ez, pitch, tx, ty, tz)
    dist = math.hypot(tx - ex, tz - ez)
    z = trig.z(frame, eyaw, epitch, dist) if trig is not None else None
    if z is not None and dist > FIRE_MAX.get(line_level(f), 1e9):
        z = 0   # too far for this level's weapon (round 17 f5): no fire
    return frame, sx, sy, eyaw, epitch, pitch, ry, cid, z


# --- the lair driver (round 15 C30, PLAYTHROUGH.md 26): T2_MOVE_DRIVE windows ------------------------
# The Primagen's lair (level 4) is a twelve-sided floor at y 0 (tools/assets/levels.py: corners at radius
# ~2,325 at 15 + 30k degrees from +z, edges 2,246 from the middle; the entrance ramp behind z -2,150), with
# pits all round, in front of his home (0, 937, 2,918) too. His home waits end only when his own search
# sees the player past the lair's back structure (static object 1911; PLAYTHROUGH.md 21 and 26): from most
# of the floor it does not, the spots that do are narrow wedges that fan out from his eye, and they move
# with where he hovers. So the player keeps strafing round him: this driver walks arcs centred on his home
# (radius LAIR_R0 to LAIR_R1, LAIR_DR apart), each clipped to the floor less LAIR_MARGIN, back and forth:
# facing him (the aim assist), an arc is a strafe, and an arc crosses every wedge that reaches the floor
# at its distance. It reads only the "move" line (the player's position and yaw, the boss's: what a
# player sees) and answers C buttons. A player pushed off the clipped floor walks back to the middle.
LAIR_HOME = (0.0, 2918.4)
LAIR_R0, LAIR_R1, LAIR_DR = 1000.0, 4900.0, 300.0
LAIR_STEP = 150.0           # waypoint spacing along an arc
LAIR_MARGIN = 300.0         # the clipped floor: every edge LAIR_MARGIN in
LAIR_SAFE_MARGIN = 150.0    # beyond this: back to the middle first
LAIR_NEAR = 110.0
LAIR_GUARD_BACK = 250.0     # "guard": back to the script this far inside the safe margin
LAIR_KEEP_PROBE = 200.0     # T2_LAIR_KEEP: the step ahead that must be floor for the walk away from him
C_U, C_D, C_L, C_R = 8, 4, 2, 1
STATS_EVERY = int(os.environ.get("T2_MOVE_STATS_EVERY", "50"))


def lair_floor(x, z, margin):
    """Inside the lair's twelve-sided floor with every edge `margin` in (edge normals at 30k degrees)."""
    return all(x * math.sin(math.radians(30 * k)) + z * math.cos(math.radians(30 * k)) <= 2246.0 - margin
               for k in range(12))


def lair_arcs():
    """The waypoints: arcs round his home, nearest first, alternate arcs walked the other way."""
    pts = []
    r, n = LAIR_R0, 0
    while r <= LAIR_R1:
        dt = LAIR_STEP / r
        arc = [(LAIR_HOME[0] + r * math.sin(i * dt), LAIR_HOME[1] - r * math.cos(i * dt))
               for i in range(-int(math.pi / 2 / dt), int(math.pi / 2 / dt) + 1)]
        arc = [p for p in arc if lair_floor(p[0], p[1], LAIR_MARGIN)]
        pts += arc if n % 2 == 0 else arc[::-1]
        r += LAIR_DR
        n += 1
    return pts


class LairDrive:
    """The C buttons for one "move" line. The strafe's world direction is calibrated from the game's
    own response (the sign of the motion along the player's right while only C-left/C-right is held)."""

    def __init__(self, kind=None):
        # T2_LAIR_DRIVE (fullrun.py --lair-drive): "guard" (default) leaves the C buttons to the input
        # script (routes56.lair_strafe) and only takes over near the floor's edge, until the player is
        # LAIR_GUARD_BACK further in; "arcs" walks the arcs all the time
        self.kind = kind or os.environ.get("T2_LAIR_DRIVE", "guard")
        self.guard = False
        self.overrides = 0     # frames answered with the driver's own buttons in "guard"
        self.pts = lair_arcs()
        self.side = 1.0        # +1: C-right moves along (-cos rotY, sin rotY)
        self.cal = 0.0         # the calibration sum
        self.cal_n = 0
        self.prev = None       # (frame, x, z, ry, buttons)
        self.i = None          # the waypoint
        self.dir = 1           # through the list (+1) or back (-1)
        self.mode = ""
        # T2_LAIR_KEEP=D (round 17 f2, fullrun.py on the N64's stream): in "guard", while he is within D
        # (horizontally) of the player, walk away from him (or along the floor's tangent when the step
        # away leaves the safe floor): his melee (two 10s within 409.6 / 716.8) was half to two thirds of
        # the lair's damage on that stream and one run lost a life there
        self.keep = float(os.environ.get("T2_LAIR_KEEP", "0") or 0)
        self.keeps = 0         # frames answered by the keep-away rule
        # T2_LAIR_PUSH=toward|jump|both (round 18 g5; "" = off, the default: the routes' streams unchanged):
        # the Primagen's push (his mode 20, the move line's "bm": Primagen_UpdateMode20) adds 716.8 (on the
        # ground) or 179.2 (in the air) to the player's velocity away from him for ~11 frames after a
        # 15-frame wind-up; from 1,550-1,750 away it threw the player off the floor (PLAYTHROUGH.md 33.2).
        # "toward": C-up/strafe towards him while he pushes; "jump": the jump (r1) held from PUSH_JUMP_AT
        # frames into the mode (in the air the push is a quarter); "both".
        self.push = os.environ.get("T2_LAIR_PUSH", "")
        # (measured, f37: the velocity jumps to ~1,000 64 frames after the mode starts, and the player leaves
        # the floor ~90 frames later; the keep-away rule had walked him away from him, towards the edge)
        self.push_at = int(os.environ.get("T2_LAIR_PUSH_JUMP_AT", "55"))
        self.push_len = int(os.environ.get("T2_LAIR_PUSH_LEN", "110"))
        self.pushes = 0        # frames answered by the push rule
        self.bm = None         # (his mode, the frame it started)
        self.jump = False      # this answer's r1

    def answer(self, frame, px, pz, ry, boss=None, bm=None):
        fwd = (-math.sin(ry), -math.cos(ry))
        self.jump = False
        if bm is not None and (self.bm is None or self.bm[0] != bm):
            self.bm = (bm, frame)
        if self.prev and frame - self.prev[0] <= 2 and self.cal_n < 40:
            b = self.prev[4]
            if b in (C_L, C_R):
                pr = (-math.cos(self.prev[3]), math.sin(self.prev[3]))
                d = (px - self.prev[1]) * pr[0] + (pz - self.prev[2]) * pr[1]
                if abs(d) < 60:   # not a push or a teleport
                    self.cal += d if b == C_R else -d
                    self.cal_n += 1
                    if self.cal_n == 40:
                        self.side = 1.0 if self.cal >= 0 else -1.0
        right = (-math.cos(ry) * self.side, math.sin(ry) * self.side)
        if self.push and self.bm and self.bm[0] == 20 and frame - self.bm[1] < self.push_len and boss is not None \
                and not any(v != v for v in boss):
            k = frame - self.bm[1]
            self.pushes += 1
            self.mode = "push"
            if self.push in ("jump", "both") and k >= self.push_at:
                self.jump = True
            if self.push in ("toward", "both"):
                return self.press(frame, px, pz, ry, (boss[0] - px, boss[2] - pz), fwd, right)
            self.prev = (frame, px, pz, ry, None)
            return None
        if self.kind == "guard":
            if not lair_floor(px, pz, LAIR_SAFE_MARGIN):
                self.guard = True
            elif lair_floor(px, pz, LAIR_SAFE_MARGIN + LAIR_GUARD_BACK):
                self.guard = False
            if not self.guard:
                want = self.keep_away(px, pz, boss)
                if want is None:
                    self.mode = "script"
                    self.prev = (frame, px, pz, ry, None)
                    return None
                self.keeps += 1
                self.mode = "keep"
                return self.press(frame, px, pz, ry, want, fwd, right)
            self.overrides += 1
        if self.i is None:   # start from the nearest waypoint
            self.i = min(range(len(self.pts)), key=lambda k: math.dist(self.pts[k], (px, pz)))
        if self.guard or not lair_floor(px, pz, LAIR_SAFE_MARGIN):
            want, self.mode = (-px, -pz), "safe"
        else:
            tx, tz = self.pts[self.i]
            if math.hypot(tx - px, tz - pz) < LAIR_NEAR:
                if not 0 <= self.i + self.dir < len(self.pts):
                    self.dir = -self.dir
                self.i += self.dir
                tx, tz = self.pts[self.i]
            want, self.mode = (tx - px, tz - pz), "arc"
        return self.press(frame, px, pz, ry, want, fwd, right)

    def keep_away(self, px, pz, boss):
        """T2_LAIR_KEEP: the direction away from him, or None (no rule, he is far, no boss)."""
        if not self.keep or boss is None or any(v != v for v in boss):
            return None
        dx, dz = px - boss[0], pz - boss[2]
        d = math.hypot(dx, dz)
        if d >= self.keep or d < 1e-3:
            return None
        ux, uz = dx / d, dz / d
        for cand in ((ux, uz), (-uz, ux), (uz, -ux)):
            if lair_floor(px + cand[0] * LAIR_KEEP_PROBE, pz + cand[1] * LAIR_KEEP_PROBE, LAIR_SAFE_MARGIN):
                return cand
        return None

    def press(self, frame, px, pz, ry, want, fwd, right):
        n = math.hypot(*want) or 1.0
        a = (want[0] * fwd[0] + want[1] * fwd[1]) / n
        c = (want[0] * right[0] + want[1] * right[1]) / n
        b = (C_U if a > 0.38 else C_D if a < -0.38 else 0) | (C_R if c > 0.38 else C_L if c < -0.38 else 0)
        self.prev = (frame, px, pz, ry, b)
        return b

def move_answer(line, drive):
    f = line.split()
    frame = int(f[1])
    px, py, pz, ry = (float.fromhex(v) for v in f[2:6])
    boss = tuple(float.fromhex(v) if v != "nan" else float("nan") for v in f[6:9]) if len(f) >= 9 else None
    bm = int(f[f.index("bm") + 1]) if "bm" in f[9:] else None   # round 18 g5: the boss's AI mode
    return frame, px, py, pz, drive.answer(frame, px, pz, ry, boss, bm)


# --- the waypoint walker (round 16 e3, PLAYTHROUGH.md 30): T2_WALK_PLAN windows -------------------
# T2_WALK_PLAN="F1-F2@LEVEL:X,Y,Z,ROTY;..." (fullrun.py --nav): inside each window (also a T2_MOVE_DRIVE
# window, so the game sends "move" lines) the player walks to (X, Z) of sub-level LEVEL and turns to ROTY.
# At the window's first line the path is planned from where the player really stands (tools/port/nav.py:
# the level's floor regions, A*, the funnel); every frame the stick turns him towards the next waypoint
# (the yaw through the measured stick turn rate, with the answers still in flight predicted as in Aim) and
# C-up walks while he faces it. Stuck (no progress for WALK_STUCK frames): back off, sidestep (the side
# alternates), plan again from there. Input only, deterministic.
WALK_NEAR = 60.0        # a waypoint is passed this close
WALK_ARRIVE = 30.0      # the goal: stop pressing C-up (the coast, ~20 units, takes him the rest)
WALK_FACE = 0.03        # the closing turn's tolerance (rad)
WALK_GO = 0.6           # C-up only while the yaw error is below this (rad)
WALK_STUCK = 36         # frames of C-up with under WALK_STUCK_D units moved: stuck
WALK_STUCK_D = 25.0
WALK_BACK, WALK_SIDE = 10, 24   # the recovery: C-down frames, then sidestep frames
WALK_BLOCK = 50.0       # a stuck point becomes a box this half-wide, this far ahead of him
# round 19 h5: the jumps (a plan entry "F1-F2@SUBj:..." plans with nav.py's jump edges): R is pressed
# WALK_JUMP_AT units before a jump or gap edge while he faces it (C-up held), for WALK_JUMP_HOLD frames;
# stuck with jumps on, every other recovery is a jump forward instead of the back-off
WALK_JUMP_AT = 70.0
WALK_JUMP_FACE = 0.35
WALK_JUMP_HOLD = 8


def walk_plan(spec):
    out = []
    for part in (spec or "").split(";"):
        if not part.strip():
            continue
        win, rest = part.split("@")
        lvl, pt_ = rest.split(":")
        a, b = (int(v) for v in win.split("-"))
        jumps = lvl.endswith("j")   # round 19 h5: plan with the jump edges, press R at them
        out.append((a, b, int(lvl.rstrip("j")), tuple(float(v) for v in pt_.split(",")), jumps))
    return out


class WalkDrive:
    def __init__(self, plan, rom):
        self.plan = plan
        self.rom = rom
        self.cur = None          # the plan entry being walked
        self.pts = []
        self.wi = 0
        self.sent = []
        self.state = "go"
        self.hist = []           # (frame, x, z) while C-up is held
        self.rec = 0             # recovery frames left
        self.side = C_L
        self.stucks = 0
        self.done = None         # the frame the goal was reached
        self.report = []
        self.extra = []
        self.jpts = []
        self.jhold = 0           # round 19 h5: frames of R left
        self.njumps = 0

    def active(self, frame):
        for e in self.plan:
            if e[0] <= frame <= e[1]:
                return e
        return None

    def replan(self, px, py, pz):
        g = self.cur[3]
        try:
            sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
            import nav
            n = nav.load(self.rom, self.cur[2], len(self.cur) > 4 and self.cur[4])
            base = n.boxes
            n.boxes = base + self.extra    # the places he got stuck at, as obstacles
            try:
                res = n.path((px, py, pz), g[:3])
            finally:
                n.boxes = base
        except Exception as e:   # no level data: walk straight
            print("walkdrive: no plan (%s)" % e, file=sys.stderr)
            res = None
        self.pts = res[0][1:] if res else [(g[0], g[2])]
        self.plen = res[2] if res else math.hypot(g[0] - px, g[2] - pz)
        self.jpts = list(res[3]) if res and len(res) > 3 else []   # round 19 h5: (x, z, kind, edge)
        self.wi = 0
        self.hist = []

    def finish(self, frame, px, pz):
        if self.cur is None:
            return
        g = self.cur[3]
        err = math.hypot(g[0] - px, g[2] - pz)
        self.report.append("walkdrive: %d-%d level %d -> (%.0f, %.0f): %s, %d stuck, %d waypoints %.0f units%s; "
                           "error %.0f at the window's end" % (
                               self.cur[0], self.cur[1], self.cur[2], g[0], g[2],
                               "arrived at %d" % self.done if self.done else "NOT ARRIVED",
                               self.stucks, self.npts, self.plen0,
                               ", %d jumps planned, %d pressed" % (self.njp0, self.njumps) if len(self.cur) > 4 and self.cur[4] else "",
                               err))
        print(self.report[-1], file=sys.stderr)
        self.cur = None

    def turn(self, ry, want):
        yaw_p = ry
        for sx in self.sent:
            yaw_p += turn_of(sx)
        e = wrap(want - yaw_p)
        sx = axis(e, TURN_FULL, GAIN / TICK)
        self.sent = (self.sent + [sx])[-(LAG - 1):]
        return sx, wrap(want - ry)

    def answer(self, frame, px, py, pz, ry):
        """(sx, buttons) for this frame, or None outside the plan's windows."""
        self.py = py
        if self.cur is not None and frame > self.cur[1]:
            self.finish(frame, px, pz)   # its last frame sent no line (a warp, a cinema)
        e = self.active(frame)
        if e is None:
            return None
        if e is not self.cur:
            self.finish(frame, px, pz)
            self.cur, self.state, self.stucks, self.done, self.sent, self.rec = e, "go", 0, None, [], 0
            self.extra = []
            self.jhold, self.njumps = 0, 0
            self.replan(px, py, pz)
            self.npts, self.plen0, self.njp0 = len(self.pts), self.plen, len(self.jpts)
        ans = self._step(frame, px, pz, ry)
        if e[1] == frame:
            self.finish(frame, px, pz)
        return ans

    def _step(self, frame, px, pz, ry):
        g = self.cur[3]
        jumps = len(self.cur) > 4 and self.cur[4]
        if self.jhold > 0:   # round 19 h5: a jump in the air: R and C-up held, the stick still steering
            self.jhold -= 1
            tx, tz = self.pts[self.wi] if self.pts else (g[0], g[2])
            sx, _ = self.turn(ry, face_yaw(px, pz, tx, tz))
            return sx, C_U, 1
        if self.state == "face" or math.hypot(g[0] - px, g[2] - pz) < WALK_ARRIVE:
            if self.state != "face":
                self.state, self.done = "face", frame
            sx, err = self.turn(ry, g[3])
            return (sx if abs(err) > WALK_FACE else 0), 0
        if self.rec > 0:   # stuck: back off, then sidestep
            self.rec -= 1
            return 0, (C_D if self.rec >= WALK_SIDE else self.side)
        while self.wi < len(self.pts) - 1 and math.hypot(self.pts[self.wi][0] - px, self.pts[self.wi][1] - pz) < WALK_NEAR:
            self.wi += 1
        tx, tz = self.pts[self.wi]
        sx, err = self.turn(ry, face_yaw(px, pz, tx, tz))
        go = abs(err) < WALK_GO
        if jumps and self.jpts and go:
            jx, jz = self.jpts[0][:2]
            dj = math.hypot(jx - px, jz - pz)
            # a jump edge just ahead (or one he walked past: drop it)
            if len(self.jpts) > 1 and math.hypot(self.jpts[1][0] - px, self.jpts[1][1] - pz) < dj:
                self.jpts.pop(0)
            elif dj < WALK_JUMP_AT and abs(wrap(face_yaw(px, pz, jx, jz) - ry)) < WALK_JUMP_FACE + math.pi / 2:
                self.jpts.pop(0)
                self.jhold = WALK_JUMP_HOLD - 1
                self.njumps += 1
                self.hist = []
                return sx, C_U, 1
        if go:
            self.hist.append((frame, px, pz))
            old = [h for h in self.hist if frame - h[0] >= WALK_STUCK]
            if old and math.hypot(px - old[-1][1], pz - old[-1][2]) < WALK_STUCK_D:
                self.stucks += 1
                self.hist = []
                if jumps and self.stucks % 2 == 1:   # round 19 h5: try a jump forward first
                    self.jhold = WALK_JUMP_HOLD - 1
                    self.njumps += 1
                    return sx, C_U, 1
                self.rec = WALK_BACK + WALK_SIDE
                # what stopped him: an obstacle WALK_BLOCK wide just ahead (the next plan goes round it)
                fx, fz = px - math.sin(ry) * WALK_BLOCK, pz - math.cos(ry) * WALK_BLOCK
                self.extra.append((fx - WALK_BLOCK, fx + WALK_BLOCK, fz - WALK_BLOCK, fz + WALK_BLOCK, -1e9, 1e9))
                self.side = C_R if self.side == C_L else C_L
                self.pending = True
                return 0, C_D
            self.hist = [h for h in self.hist if frame - h[0] <= WALK_STUCK + 4]
        else:
            self.hist = []
        if getattr(self, "pending", False) and self.rec == 0:
            self.pending = False
            self.replan(px, self.py, pz)
        return sx, (C_U if go else 0)


def fifo_watchdog(proc, d, opened):
    """Round 18 g2: the driver opens the two fifos with blocking opens; a game that exits (or is
    killed) before it opens them used to leave the driver waiting forever. This thread waits on the
    child; if it exits before both fifos are open, it opens the other end of each (non-blocking) so
    the driver's open returns and its read sees EOF. T2_AIM_OPEN_TIMEOUT=SECONDS also kills (SIGKILL:
    the game ignores SIGTERM) a game that has not opened the fifos after that long."""
    limit = float(os.environ.get("T2_AIM_OPEN_TIMEOUT", "0") or 0)
    t0 = time.monotonic()
    while not opened.is_set():
        try:
            proc.wait(timeout=0.2)
            break
        except subprocess.TimeoutExpired:
            if limit > 0 and time.monotonic() - t0 > limit:
                print("aimdrive: the game did not open the fifos in %.0f s: killing it" % limit, file=sys.stderr)
                proc.kill()
                proc.wait()
                break
    if opened.is_set():
        return
    print("aimdrive: the game exited (%s) before it opened the fifos" % proc.returncode, file=sys.stderr)
    fds = []
    while not opened.is_set():
        for name, flags in (("out", os.O_WRONLY), ("in", os.O_RDONLY)):
            try:
                fds.append(os.open(os.path.join(d, name), flags | os.O_NONBLOCK))
            except OSError:   # ENXIO: nobody is waiting on the read side yet
                pass
        opened.wait(0.05)
    for fd in fds:
        os.close(fd)


def main():
    args = sys.argv[1:]
    probe = None
    simple = False
    stats = None
    while args and args[0] != "--":
        if args[0] == "--simple":
            simple = True
            args = args[1:]
        elif args[0] == "--probe":
            probe = tuple(int(v) for v in args[1].split(","))
            args = args[2:]
        elif args[0] == "--stats":
            stats = open(args[1], "w")
            args = args[2:]
        else:
            sys.exit("usage: aimdrive.py [--simple] [--probe SX,SY] [--stats FILE] -- CMD ...")
    cmd = args[1:]
    d = tempfile.mkdtemp(prefix="aimfifo")
    os.mkfifo(os.path.join(d, "out"))
    os.mkfifo(os.path.join(d, "in"))
    env = dict(os.environ, T2_AIM_FIFO=d)
    if "SHIP_HOME" not in env:
        # round 17 merge: each game process its own settings folder (LUS writes turok2.cfg.json,
        # imgui.ini and logs/ into SHIP_HOME, else the current directory: two runs from one OUT/
        # raced on turok2.cfg.json, round 17 f5). The current directory's turok2.cfg.json is copied
        # in, so the run sees the same settings; T2_KEEP_CWD_CFG=1 keeps the old behaviour.
        if os.environ.get("T2_KEEP_CWD_CFG") != "1":
            home = os.path.join(d, "home")
            os.mkdir(home)
            if os.path.exists("turok2.cfg.json"):
                shutil.copy("turok2.cfg.json", home)
            env["SHIP_HOME"] = home
    proc = subprocess.Popen(cmd, env=env)
    opened = threading.Event()
    threading.Thread(target=fifo_watchdog, args=(proc, d, opened), daemon=True).start()
    n = 0
    aim = SimpleAim() if simple else Aim()
    picker = Picker() if PICK and not probe else None
    trig = Trigger() if FIRE and not probe else None
    drive = LairDrive()
    rom = cmd[cmd.index("--rom") + 1] if "--rom" in cmd else "baserom.us.z64"
    walk = WalkDrive(walk_plan(os.environ.get("T2_WALK_PLAN")), rom)
    moves = 0
    try:
        with open(os.path.join(d, "out")) as fin, open(os.path.join(d, "in"), "w") as fout:
            opened.set()
            for line in fin:
                if line.startswith("move ") and walk.plan:   # T2_WALK_PLAN: the waypoint walker
                    fl = line.split()
                    frame = int(fl[1])
                    wpx, wpy, wpz, wry = (float.fromhex(v) for v in fl[2:6])
                    wa = walk.answer(frame, wpx, wpy, wpz, wry)
                    if wa is not None:
                        fout.write("%d 0 c%x%s\n" % (wa[0], wa[1], " r1" if len(wa) > 2 and wa[2] else ""))  # (sx, C[, jump])
                        fout.flush()
                        moves += 1
                        if stats and frame % STATS_EVERY == 0:
                            stats.write("walk %d %.1f %.1f %.1f ry %.2f sx %d c %x wp %d/%d %s\n" % (
                                frame, wpx, wpy, wpz, wry, wa[0], wa[1], walk.wi, len(walk.pts), walk.state))
                        continue
                if line.startswith("move "):   # T2_MOVE_DRIVE: the lair driver's C buttons
                    frame, px, py, pz, b = move_answer(line, drive)
                    if drive.jump:   # round 18 g5 (T2_LAIR_PUSH): the jump against the Primagen's push
                        fout.write("- - r1\n" if b is None else "- - c%x r1\n" % b)
                    else:
                        fout.write("- -\n" if b is None else "- - c%x\n" % b)
                    fout.flush()
                    moves += 1
                    if stats and frame % STATS_EVERY == 0:
                        fl = line.split()
                        stats.write("move %d %.1f %.1f %.1f ry %.2f %s %s spot %s side %+d boss %s\n" % (
                            frame, px, py, pz, float.fromhex(fl[5]), "-" if b is None else "%x" % b, drive.mode,
                            drive.i, drive.side,
                            " ".join("%.0f" % float.fromhex(v) if v != "nan" else v for v in fl[6:9])))
                    continue
                if not line.startswith("aim "):
                    continue
                frame, sx, sy, ey, ep, pitch, ry, cid, z = answer(line, aim, probe, picker, trig)
                fout.write("%d %d\n" % (sx, sy) if z is None else "%d %d z%d\n" % (sx, sy, z))
                fout.flush()
                n += 1
                if stats:
                    stats.write("%d %d %d %.4f %.4f %.4f %.4f %s %s\n" % (frame, sx, sy, ey, ep, pitch, ry,
                                                                        "-" if cid is None else cid, "-" if z is None else z))
                    if frame % CAND_EVERY == 0:   # the candidates as seen: "# F id:dist:hp ..."
                        fl = line.split()
                        cs, php = parse_cands(fl)
                        ex_, ez_ = float.fromhex(fl[6]), float.fromhex(fl[8])
                        tot = line_totem(fl)   # round 17 f5: "a" = an attacker of the totem, then the totem
                        stats.write("# %d hp %s %s%s\n" % (frame, php, " ".join(
                            "%d:%.0f:%d%s" % (c[0], math.hypot(c[1] - ex_, c[3] - ez_), c[4], {0: "t", 2: "a"}.get(c[5], ""))
                            for c in cs), "" if tot is None else " tot %d %ds %d/%d" % tot))
    finally:
        opened.set()
        rc = proc.wait()
        if walk.cur is not None:
            walk.finish(walk.cur[1], 0.0, 0.0)
        if walk.plan:
            print("aimdrive: %d walks, %d arrived" % (len(walk.report), sum("arrived at" in r for r in walk.report)),
                  file=sys.stderr)
        shutil.rmtree(d, ignore_errors=True)
        if picker or trig:
            print("aimdrive: %d target switches, %d left as not hurt, %d shots" % (
                picker.switches if picker else 0, picker.immunes if picker else 0, trig.shots if trig else 0),
                file=sys.stderr)
        print("aimdrive: %d stick answers, %d move answers (%d with the lair driver's own C buttons, %d keeping "
              "away); the game exited %d" % (n, moves, drive.overrides + drive.keeps if drive.kind == "guard" else moves,
                                             drive.keeps, rc), file=sys.stderr)
        if drive.push:
            print("aimdrive: %d frames answered against the Primagen's push (%s)" % (drive.pushes, drive.push),
                  file=sys.stderr)
    sys.exit(rc)


if __name__ == "__main__":
    main()
