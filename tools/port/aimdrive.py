#!/usr/bin/env python3
"""Closed-loop stick aiming for a headless run (round 15 d3, docs/port/PLAYTHROUGH.md section 22).

    python3 tools/port/aimdrive.py [--probe SX,SY] [--stats FILE] -- turok2 ARGS...

Runs the game (stdout and stderr pass through) with T2_AIM_FIFO pointing at two fifos in a fresh
temporary directory. Inside the T2_AIM_STICK windows the aim assists only choose a target; after
each such frame the game writes one line, "aim F px py pz rotY ex ey ez pitch tx ty tz" (the
player position and yaw, the camera eye and pitch, the target point; exact hex floats), and waits
for the next frame's stick, which this driver computes from that line alone: the yaw error through
the measured turn rate of the stick (playthrough.TURN_FULL, cube law, dead zone 3), the pitch error
likewise (PITCH_EASE below: the pitch follows a look angle). Nothing else of the game is read and nothing is written into it: the
same input a player's thumb gives. Deterministic: the same lines give the same answers.

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


def main():
    args = sys.argv[1:]
    probe = None
    stats = None
    while args and args[0] != "--":
        if args[0] == "--probe":
            probe = tuple(int(v) for v in args[1].split(","))
            args = args[2:]
        elif args[0] == "--stats":
            stats = open(args[1], "w")
            args = args[2:]
        else:
            sys.exit("usage: aimdrive.py [--probe SX,SY] [--stats FILE] -- CMD ...")
    cmd = args[1:]
    d = tempfile.mkdtemp(prefix="aimfifo")
    os.mkfifo(os.path.join(d, "out"))
    os.mkfifo(os.path.join(d, "in"))
    env = dict(os.environ, T2_AIM_FIFO=d)
    proc = subprocess.Popen(cmd, env=env)
    n = 0
    aim = Aim()
    try:
        with open(os.path.join(d, "out")) as fin, open(os.path.join(d, "in"), "w") as fout:
            for line in fin:
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
        print("aimdrive: %d stick answers; the game exited %d" % (n, rc), file=sys.stderr)
    sys.exit(rc)


if __name__ == "__main__":
    main()
