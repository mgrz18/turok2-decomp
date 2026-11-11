#!/usr/bin/env python3
"""soak.py - long-run / fuzz harness for the native port (docs/port/STABILITY.md).

Runs `turok2 --game` many times headless with random but valid input scripts, under gdb, and
collects every crash (signal, LUS crash handler, trap, hang, non-zero exit) with the crashing
thread's backtrace. Crashes are deduplicated by their top game frames.

    python3 tools/port/soak.py --exe OUT/build/app/turok2 --archive OUT/turok2.o2r \
        --rom baserom.us.z64 --out OUT/soak [--kind attract|newgame|warp] [--seeds 0:40]
        [--frames N] [--low-memory | --both-memory] [--draw] [--jobs 4] [--warps OUT/warp_dests.json]

Kinds:
  attract  no input: the boot loop and the demos (use --frames 100000)
  newgame  a new game (START@4400 A@4500 A@4700 A@4800), then random play from frame 13000:
           walk/strafe/turn/look, fire, jump, the weapon wheel (A/B + stick), pause + menu
           moves, the map; T2_GIVE_WEAPONS on even seeds
  warp     newgame + T2_WARP_TO with 1-3 random warp ids from warp_dests.json (levels.py),
           T2_GIVE_WEAPONS=1 and T2_WAKE_ENEMIES=1500
  levels   newgame + T2_WARP_TO through level ids (~level: the level's start), 4 per seed in
           order (seeds 0-40 cover levels 1-164), short random play in each, T2_GIVE_WEAPONS=1,
           T2_WAKE_ENEMIES=1500 (use --frames 13000 + 4 x the time per level, e.g. 25000)

A run that does not exit within --timeout (default 600 s; a normal run takes seconds) is
interrupted with SIGINT: gdb prints every thread, and the class (HANG:...) is the busy game
thread's top frames.

Everything a run needs to be repeated is in OUT/<kind>-<seed>[-lm]/cmd.sh (input.txt, env).
The summary is OUT/summary.json and OUT/crashes.txt (one entry per crash class: the key,
the runs that hit it and one backtrace).
"""
import argparse
import concurrent.futures as cf
import json
import os
import random
import re
import shlex
import subprocess
import sys
import time

NEW_GAME = "START@4400 A@4500 A@4700 A@4800"
PLAY_FROM = 13000

MOVES = ["CU", "CD", "CL", "CR"]


def gen_play(rng, start, end):
    """Random but valid play input from `start` to `end` (frames)."""
    out = []
    f = start
    while f < end - 40:
        seg = rng.randint(30, 300)
        last = min(f + seg, end - 1)
        r = rng.random()
        # movement (most of the time)
        if r < 0.75:
            out.append("%s@%d..%d" % (rng.choice(MOVES), f, last))
        if rng.random() < 0.5:
            v = rng.choice([-100, -80, -50, 50, 80, 100])
            a = min(f + rng.randint(0, max(0, seg // 2)), last)
            out.append("SX=%d@%d..%d" % (v, a, min(a + rng.randint(5, 40), last)))
        if rng.random() < 0.2:
            v = rng.choice([-80, 80])
            a = min(f + rng.randint(0, max(0, seg // 2)), last)
            out.append("SY=%d@%d..%d" % (v, a, min(a + rng.randint(5, 20), last)))
        # one extra action
        x = rng.random()
        if x < 0.35:  # fire: taps or a hold
            for _ in range(rng.randint(1, 4)):
                a = rng.randint(f, last)
                out.append("Z@%d..%d" % (a, min(a + rng.randint(3, 60), last)))
        elif x < 0.5:  # jump
            out.append("R@%d" % rng.randint(f, last))
        elif x < 0.65:  # weapon wheel: hold A/B, stick to a slot
            a = rng.randint(f, max(f, last - 40))
            b = rng.choice(["A", "B"])
            sx = rng.choice([-80, 0, 80])
            sy = rng.choice([-80, 0, 80]) if sx else rng.choice([-80, 80])
            out.append("%s@%d..%d" % (b, a, a + 30))
            if sx:
                out.append("SX=%d@%d..%d" % (sx, a + 5, a + 25))
            if sy:
                out.append("SY=%d@%d..%d" % (sy, a + 5, a + 25))
        elif x < 0.72:  # pause, move in the menu, back out
            a = rng.randint(f, max(f, last - 60))
            out.append("START@%d" % a)
            t = a + 30
            for _ in range(rng.randint(0, 4)):
                out.append("%s@%d" % (rng.choice(["DU", "DD", "DL", "DR"]), t))
                t += 15
            if rng.random() < 0.5:
                out.append("A@%d" % t)
                t += 30
                out.append("B@%d" % t)
                t += 30
            out.append("START@%d" % t)
            if rng.random() < 0.5:
                out.append("B@%d" % (t + 30))
        elif x < 0.78:  # the map
            a = rng.randint(f, last)
            out.append("L@%d" % a)
            out.append("L@%d" % min(a + rng.randint(30, 120), end - 5))
        f = last + 1
    return out


LEVELS = range(1, 165)  # levels.py index.json: 165 levels, 0 is the menu backdrop
LEVELS_PER_SEED = 4


def make_run(kind, seed, frames, warps):
    rng = random.Random(seed * 7919 + {"attract": 1, "newgame": 2, "warp": 3, "levels": 4}[kind])
    env = {}
    items = []
    if kind != "attract":
        items.append(NEW_GAME)
        items += gen_play(rng, PLAY_FROM, frames)
        if seed % 2 == 0 or kind in ("warp", "levels"):
            env["T2_GIVE_WEAPONS"] = "1"
        if kind == "levels":
            # the levels in order: seed s takes 4s..4s+3 (mod 164), each warp after the last one's
            # span; a negative T2_WARP_TO id is ~level (the level's start, t2debug.c)
            ids = []
            span = max(1, (frames - PLAY_FROM - 600) // LEVELS_PER_SEED)
            for i in range(LEVELS_PER_SEED):
                lv = LEVELS[(seed * LEVELS_PER_SEED + i) % len(LEVELS)]
                ids.append("%d@%d" % (~lv, PLAY_FROM + 200 + i * span))
            env["T2_WARP_TO"] = ",".join(ids)
            env["T2_WAKE_ENEMIES"] = "1500"
        if kind == "warp":
            n = rng.randint(1, 3)
            ids = []
            span = max(1, (frames - PLAY_FROM - 600) // n)
            for i in range(n):
                w = rng.choice(warps)
                ids.append("%d@%d" % (w, PLAY_FROM + 200 + i * span + rng.randint(0, span // 3)))
            env["T2_WARP_TO"] = ",".join(ids)
            env["T2_WAKE_ENEMIES"] = "1500"
    return env, items


GDB_CMDS = [
    "set pagination off",
    "set confirm off",
    "handle SIGPIPE nostop noprint",
    "handle SIGUSR1 nostop noprint",
    "handle SIGUSR2 nostop noprint",
    "run",
    "echo \\n=== SOAK-BT ===\\n",
    "bt 30",
    "echo \\n=== SOAK-REGS ===\\n",
    "info registers rip rsp",
    "echo \\n=== SOAK-END ===\\n",
    "thread apply all bt 10",
    "echo \\n=== SOAK-THREADS-END ===\\n",
]

IDLE_FUNCS = ("pthread_cond_wait", "wait_baton", "condition_variable", "__futex", "epoll_wait",
              "poll", "nanosleep", "clock_nanosleep", "t2os_run_until_idle")


def hang_frames(text):
    """The busy thread of an interrupted hang: the first one in `thread apply all bt` whose stack
    has no wait (the game threads that wait sit in wait_baton/pthread_cond_wait)."""
    part = text.split("=== SOAK-END ===", 1)[-1]
    for block in re.split(r"\n(?=Thread \d+ )", part):
        frames = []
        for line in block.splitlines():
            fm = FRAME_RE.match(line.strip())
            if fm:
                frames.append((fm.group(2), fm.group(3) or ""))
        if any(f != "??" for f, _ in frames) and not any(f.startswith(IDLE_FUNCS) for f, _ in frames):
            return frames
    return []


FRAME_RE = re.compile(r"^#(\d+)\s+(?:0x[0-9a-f]+ in )?(\S+) \(.*?\)(?: at (\S+))?")
SKIP_FUNCS = ("raise", "abort", "__pthread_kill", "pthread_kill", "__GI_", "gsignal",
              "__restore_rt", "<signal")


def classify(text, rc, timed_out):
    m = re.search(r"received signal (\w+)", text)
    if timed_out:
        return "HANG", hang_frames(text)
    if m is None:
        if rc == 0:
            return None, []
        # LUS's crash handler or an exit(1) path
        kind = "EXIT%d" % rc
        if "CrashHandler" in text or "Received signal" in text:
            kind = "LUSCRASH"
        return kind, []
    sig = m.group(1)
    frames = []
    bt = text.split("=== SOAK-BT ===", 1)[-1].split("=== SOAK-REGS ===", 1)[0]
    for line in bt.splitlines():
        fm = FRAME_RE.match(line.strip())
        if fm:
            frames.append((fm.group(2), fm.group(3) or ""))
    return sig, frames


def crash_key(sig, frames, depth=4):
    names = [f for f, _ in frames if not f.startswith(SKIP_FUNCS)]
    return sig + ":" + "<".join(names[:depth]) if names else sig


def run_one(job):
    a, kind, seed, lm = job
    tag = "%s-%d%s" % (kind, seed, "-lm" if lm else "")
    d = os.path.join(a.out, tag)
    os.makedirs(d, exist_ok=True)
    frames = a.frames
    env, items = make_run(kind, seed, frames, a.warp_ids)
    if a.env:
        for kv in a.env:
            k, v = kv.split("=", 1)
            env[k] = v
    with open(os.path.join(d, "input.txt"), "w") as fp:
        for it in items:
            fp.write(it + "\n")
    args = [os.path.abspath(a.exe), "--archive", os.path.abspath(a.archive), "--rom",
            os.path.abspath(a.rom), "--game", "--pak-blank", "--frames", str(frames)]
    if items:
        args += ["--input", "@" + os.path.abspath(os.path.join(d, "input.txt"))]
    if lm:
        args.append("--low-memory")  # Linux debug: the game memory below 2 GB (the default is high)
    if a.draw:
        args.append("--draw")
        env.setdefault("SDL_VIDEODRIVER", "offscreen")
        env.setdefault("vblank_mode", "0")
    else:
        args.append("--no-window")
    env.setdefault("T2_STATE_LOG", "600")
    full_env = dict(os.environ)
    full_env.update(env)
    gdb = ["gdb", "-nx", "-batch", "-return-child-result"]
    for c in GDB_CMDS:
        gdb += ["-ex", c]
    gdb += ["--args"] + args
    with open(os.path.join(d, "cmd.sh"), "w") as fp:
        fp.write("#!/bin/sh\n# soak.py %s seed %d\ncd %s\n" % (kind, seed, shlex.quote(d)))
        fp.write(" ".join("%s=%s" % (k, shlex.quote(v)) for k, v in sorted(env.items())))
        fp.write(" " + " ".join(shlex.quote(x) for x in args) + "\n")
    t0 = time.time()
    timed_out = False
    logp = os.path.join(d, "gdb.out")
    with open(logp, "wb") as lf:
        p = subprocess.Popen(gdb, cwd=d, env=full_env, stdout=lf, stderr=subprocess.STDOUT,
                             start_new_session=True)
        try:
            rc = p.wait(timeout=a.timeout)
        except subprocess.TimeoutExpired:
            timed_out = True
            rc = -1
            # SIGINT to the game (gdb's child): gdb stops it and prints every thread
            try:
                kids = open("/proc/%d/task/%d/children" % (p.pid, p.pid)).read().split()
                for k in kids:
                    os.kill(int(k), 2)
                p.wait(timeout=120)
            except (OSError, subprocess.TimeoutExpired):
                pass
            if p.poll() is None:
                os.killpg(p.pid, 9)  # its own session: gdb and the game, nothing else
                p.wait()
    with open(logp, "rb") as lf:
        out = lf.read().decode("utf-8", "replace")
    os.unlink(logp)
    dt = time.time() - t0
    # keep only the useful tail of a passing run's log
    lines = [l for l in out.splitlines() if "[info]" not in l and "[debug]" not in l]
    with open(os.path.join(d, "run.log"), "w") as fp:
        fp.write("\n".join(lines) + "\n")
    sig, frames_bt = classify(out, rc, timed_out)
    last_frame = 0
    for m in re.finditer(r"^\s*frame (\d+):|^game: frame (\d+):|^state: f(?:rame)? ?(\d+)", out, re.M):
        last_frame = int(next(g for g in m.groups() if g))
    levels = sorted(set(int(x) for x in re.findall(r"\blevel (\d+)", out)))
    res = {"tag": tag, "kind": kind, "seed": seed, "low_memory": lm, "rc": rc, "secs": round(dt, 1),
           "last_frame": last_frame, "levels": levels[:40], "env": env}
    if sig:
        res["signal"] = sig
        res["key"] = crash_key(sig, frames_bt)
        res["bt"] = ["%s %s" % fr for fr in frames_bt[:12]]
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe", required=True)
    ap.add_argument("--archive", required=True)
    ap.add_argument("--rom", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--kind", default="newgame", choices=["attract", "newgame", "warp", "levels"])
    ap.add_argument("--seeds", default="0:8", help="A:B (B exclusive)")
    ap.add_argument("--frames", type=int, default=30000)
    ap.add_argument("--low-memory", action="store_true", help="--low-memory (Linux debug; high memory is the default)")
    ap.add_argument("--both-memory", action="store_true", help="every seed with the default (high) AND --low-memory")
    ap.add_argument("--draw", action="store_true", help="--draw (SDL offscreen, vblank_mode=0)")
    ap.add_argument("--jobs", type=int, default=4)
    ap.add_argument("--timeout", type=int, default=600)
    ap.add_argument("--warps", help="warp_dests.json from tools/assets/levels.py (--kind warp)")
    ap.add_argument("--env", action="append", help="K=V for every run")
    a = ap.parse_args()
    a.jobs = min(a.jobs, 4)  # the box: python pools <= 4
    a.warp_ids = []
    if a.kind == "warp":
        if not a.warps:
            ap.error("--kind warp needs --warps")
        a.warp_ids = sorted(set(w["warp_id"] for w in json.load(open(a.warps))))
    s0, s1 = (int(x) for x in a.seeds.split(":"))
    os.makedirs(a.out, exist_ok=True)
    mems = [False, True] if a.both_memory else [a.low_memory]
    jobs = [(a, a.kind, s, hm) for s in range(s0, s1) for hm in mems]
    results = []
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        for r in ex.map(run_one, jobs):
            results.append(r)
            print("%-22s rc=%-4s %6.1fs last %-6d %s" % (r["tag"], r["rc"], r["secs"], r["last_frame"],
                                                        r.get("key", "ok")), flush=True)
    sp = os.path.join(a.out, "summary.json")
    old = []
    if os.path.exists(sp):
        old = [r for r in json.load(open(sp)) if r["tag"] not in {x["tag"] for x in results}]
    allr = old + results
    json.dump(allr, open(sp, "w"), indent=1)
    classes = {}
    for r in allr:
        if "key" in r:
            classes.setdefault(r["key"], []).append(r)
    with open(os.path.join(a.out, "crashes.txt"), "w") as fp:
        for k, rs in sorted(classes.items(), key=lambda kv: -len(kv[1])):
            fp.write("%s  (%d runs: %s)\n" % (k, len(rs), " ".join(x["tag"] for x in rs[:10])))
            for l in rs[0].get("bt", []):
                fp.write("    %s\n" % l)
    tot = sum(r["last_frame"] for r in allr)
    print("runs %d, crashed %d, classes %d, frames %d" % (len(allr), sum(1 for r in allr if "key" in r),
                                                         len(classes), tot))


if __name__ == "__main__":
    sys.exit(main())
