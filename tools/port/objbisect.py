#!/usr/bin/env python3
"""Find the object that makes two builds of the game play differently (round 15 d5; STABILITY.md 11).

Two build dirs of the same tree that differ in how the game code is compiled (e.g.
`-DT2_AUTO_VAR_INIT=none` and `zero`, or two optimisation levels). A mixed binary takes the
objects of a subset of t2audio + turok2_game from BAD (the build that diverges) and the rest from
GOOD, linked with ninja's own link line of the BAD build. It is bad when its det log differs from
GOOD's reference det log in the first FRAMES frames, or has fewer lines (a crash). The bisection
halves the index range until one object is left.

    objbisect.py --bad OUT/none --good OUT/zero --ref zero_det.log --route RUN --frames 16000 \\
        --work /tmp/bis --archive OUT/turok2.o2r --rom baserom.us.z64 bisect [LO HI]
    objbisect.py ... list                    # index, archive, object
    objbisect.py ... test NAME I0:I1 ...     # one mix: objects [I0, I1) from BAD

The reference: the GOOD build's run with the same route and `T2_DET_LOG=1` (2 det lines a frame).
The route: a playthrough.py output dir (playthrough.txt + teleport.env). Headless, one turok2 at a
time. Each step takes one run of FRAMES frames (a Release build: ~1 minute for 140,000 frames).
"""
import argparse, os, shutil, subprocess, sys, tempfile

LIBS = [("app/libt2audio.a", "app/CMakeFiles/t2audio.dir"), ("libturok2_game.a", "CMakeFiles/turok2_game.dir")]


def objects(a):
    """The archives' members in archive order (ninja deletes its .rsp), mapped to the object files."""
    out = []
    for lib, objdir in LIBS:
        where = {}
        for root, _, files in os.walk(os.path.join(a.bad, objdir)):
            for f in files:
                if f.endswith(".o"):
                    where[f] = os.path.relpath(os.path.join(root, f), a.bad)
        names = subprocess.run(["ar", "t", os.path.join(a.bad, lib)], capture_output=True, text=True,
                               check=True).stdout.split()
        out += [(lib, where[n]) for n in names]
    return out


def parse(ranges):
    s = set()
    for r in ranges:
        lo, hi = r.split(":")
        s.update(range(int(lo), int(hi)))
    return s


def mix(a, name, sel):
    objs = objects(a)
    d = os.path.join(a.work, name)
    os.makedirs(d, exist_ok=True)
    cmd = " " + subprocess.run(["ninja", "-C", a.bad, "-t", "commands", "app/turok2"], capture_output=True,
                               text=True, check=True).stdout.splitlines()[-1] + " "
    for lib in sorted(set(l for l, _ in objs)):
        mine = os.path.join(d, os.path.basename(lib))
        if os.path.exists(mine):
            os.remove(mine)
        paths = [os.path.join(a.bad if i in sel else a.good, p) for i, (l, p) in enumerate(objs) if l == lib]
        with open(mine + ".rsp", "w") as fp:
            fp.write("\n".join(paths))
        subprocess.run(["ar", "qcs", mine, "@" + mine + ".rsp"], check=True)
        assert (" " + lib + " ") in cmd, lib
        cmd = cmd.replace(" " + lib + " ", " " + mine + " ")
    exe = os.path.join(d, "turok2")
    assert " -o app/turok2 " in cmd
    cmd = cmd.replace(" -o app/turok2 ", " -o " + exe + " ")
    subprocess.run(cmd, shell=True, check=True, cwd=a.bad)   # ninja's own link line, from its build dir
    return exe


def env_of(route):
    """teleport.env's assignments (KEY=VALUE lines, values maybe quoted)."""
    env = {}
    for line in open(os.path.join(route, "teleport.env")):
        line = line.strip()
        if "=" in line and not line.startswith("export"):
            k, v = line.split("=", 1)
            env[k] = v[1:-1] if v.startswith('"') else v
    return env


def run(a, exe, log):
    w = tempfile.mkdtemp(dir=a.work)
    os.makedirs(os.path.join(w, "pak"))
    env = dict(os.environ, SDL_VIDEODRIVER="offscreen", T2_DET_LOG="1", HOME=w, **env_of(a.route))
    with open(log, "w") as fp:
        subprocess.run(["timeout", "-s", "KILL", "3600", exe, "--archive", os.path.abspath(a.archive),
                        "--rom", os.path.abspath(a.rom), "--game", "--pak-dir", os.path.join(w, "pak"),
                        "--frames", str(a.frames), "--input", "@" + os.path.join(os.path.abspath(a.route), "playthrough.txt")],
                       cwd=w, env=env, stdout=fp, stderr=subprocess.STDOUT)
    shutil.rmtree(w, ignore_errors=True)


def det(log, n=None):
    with open(log, errors="replace") as fp:
        lines = [l for l in fp if l.startswith("det")]
    return lines[:n] if n else lines


def test(a, name, sel):
    exe = mix(a, name, sel)
    log = os.path.join(a.work, name, "det.log")
    run(a, exe, log)
    os.remove(exe)
    ref = det(a.ref, 2 * a.frames)
    got = det(log)
    d = next(((i, x.strip()[:160], y.strip()[:160]) for i, (x, y) in enumerate(zip(got, ref)) if x != y), None)
    if d is None and len(got) < len(ref):
        d = (len(got), "stopped (a crash?)", "")
    print(name, "from BAD:", len(sel), "det lines", len(got), "/", len(ref), "->", "BAD" if d else "good", d or "",
          flush=True)
    return d is not None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bad", required=True, help="the build dir that diverges")
    ap.add_argument("--good", required=True, help="the reference build dir")
    ap.add_argument("--ref", help="the GOOD build's det log of the route")
    ap.add_argument("--route", help="a playthrough.py output dir")
    ap.add_argument("--frames", type=int, default=16000)
    ap.add_argument("--work", default="objbisect.work")
    ap.add_argument("--archive", default="OUT/turok2.o2r")
    ap.add_argument("--rom", default="baserom.us.z64")
    ap.add_argument("cmd", choices=["list", "mix", "test", "bisect"])
    ap.add_argument("args", nargs="*")
    a = ap.parse_args()
    a.bad, a.good, a.work = os.path.abspath(a.bad), os.path.abspath(a.good), os.path.abspath(a.work)
    os.makedirs(a.work, exist_ok=True)
    if a.cmd == "list":
        for i, (l, p) in enumerate(objects(a)):
            print(i, l, p)
        return
    if a.cmd == "mix":
        print(mix(a, a.args[0], parse(a.args[1:])))
        return
    if not (a.ref and a.route):
        sys.exit("test/bisect need --ref and --route")
    if a.cmd == "test":
        test(a, a.args[0], parse(a.args[1:]))
        return
    objs = objects(a)
    lo = int(a.args[0]) if a.args else 0
    hi = int(a.args[1]) if len(a.args) > 1 else len(objs)
    step = 0
    while hi - lo > 1:
        mid = (lo + hi) // 2
        step += 1
        if test(a, "b%d" % step, set(range(lo, mid))):
            hi = mid
        elif test(a, "b%dr" % step, set(range(mid, hi))):
            lo = mid
        else:
            print("neither half alone: an interaction between [%d,%d) and [%d,%d)" % (lo, mid, mid, hi))
            break
    print("RESULT", lo, hi, objs[lo] if hi - lo == 1 else "", flush=True)


if __name__ == "__main__":
    main()
