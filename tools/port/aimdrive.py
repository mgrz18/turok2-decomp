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

--simple: the first controller (the observed error only, no prediction; round 15 d3's s1 run).
--probe SX,SY: answer a constant stick instead (calibration: the pitch/yaw change per frame).
"""
import math, os, subprocess, sys, tempfile

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


def answer(line, aim, probe=None):
    f = line.split()
    frame = int(f[1])
    px, py, pz, ry, ex, ey, ez, pitch, tx, ty, tz = (float.fromhex(v) for v in f[2:13])
    if probe:
        on = len(probe) < 3 or getattr(answer, "n", 0) < probe[2]
        answer.n = getattr(answer, "n", 0) + 1
        return frame, probe[0] if on else 0, probe[1] if on else 0, 0.0, 0.0, pitch, ry
    sx, sy, eyaw, epitch = aim.answer(frame, px, pz, ry, ex, ey, ez, pitch, tx, ty, tz)
    return frame, sx, sy, eyaw, epitch, pitch, ry


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

    def answer(self, frame, px, pz, ry):
        fwd = (-math.sin(ry), -math.cos(ry))
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
        if self.kind == "guard":
            if not lair_floor(px, pz, LAIR_SAFE_MARGIN):
                self.guard = True
            elif lair_floor(px, pz, LAIR_SAFE_MARGIN + LAIR_GUARD_BACK):
                self.guard = False
            if not self.guard:
                self.mode = "script"
                self.prev = (frame, px, pz, ry, None)
                return None
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
    return frame, px, py, pz, drive.answer(frame, px, pz, ry)


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
    proc = subprocess.Popen(cmd, env=env)
    n = 0
    aim = SimpleAim() if simple else Aim()
    drive = LairDrive()
    moves = 0
    try:
        with open(os.path.join(d, "out")) as fin, open(os.path.join(d, "in"), "w") as fout:
            for line in fin:
                if line.startswith("move "):   # T2_MOVE_DRIVE: the lair driver's C buttons
                    frame, px, py, pz, b = move_answer(line, drive)
                    fout.write("- -\n" if b is None else "- - %x\n" % b)
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
                frame, sx, sy, ey, ep, pitch, ry = answer(line, aim, probe)
                fout.write("%d %d\n" % (sx, sy))
                fout.flush()
                n += 1
                if stats:
                    stats.write("%d %d %d %.4f %.4f %.4f %.4f\n" % (frame, sx, sy, ey, ep, pitch, ry))
    finally:
        rc = proc.wait()
        for name in ("out", "in"):
            os.unlink(os.path.join(d, name))
        os.rmdir(d)
        print("aimdrive: %d stick answers, %d move answers (%d with the lair driver's own C buttons); the game "
              "exited %d" % (n, moves, drive.overrides if drive.kind == "guard" else moves, rc), file=sys.stderr)
    sys.exit(rc)


if __name__ == "__main__":
    main()
