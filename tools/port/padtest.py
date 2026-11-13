#!/usr/bin/env python3
"""padtest.py - the controls a human uses, checked headless (round 14 c4, docs/port/MACOS.md 16).

Drives `turok2 --game --no-window --pad-test FILE` (port/app/t2controls.cpp): an SDL virtual
gamepad, LUS keyboard events and a fake mouse go through LUS's real ControlDeck and the port's
controller read, and T2_PAD_LOG=1 logs the N64 pad the game receives. Checks:

  map      every default binding alone (keyboard, gamepad buttons, sticks, triggers, captured
           mouse), unbound buttons give nothing, the pad unplugged and plugged back in
  newgame  a new game from the title with the gamepad only, then with the keyboard only: the
           game must reach engine mode 11 (in a level), as `--input START@4400 A@4500 ...` does

  padtest.py --bin OUT/rel/app/turok2 --archive OUT/turok2.o2r [--rom baserom.us.z64] [--work DIR]

Each run gets a fresh directory (so LUS's turok2.cfg.json starts from the port's defaults).
Exit 0 when every check passes.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys

# (device kind, name, expected N64 buttons, expected stick) - the port's defaults
# (t2controls.cpp T2Controls_DefaultMappings). Buttons: A 8000 B 4000 Z 2000 START 1000
# DU 0800 DD 0400 DL 0200 DR 0100 L 0020 R 0010 CU 0008 CD 0004 CL 0002 CR 0001.
KEYS = [("W", 0x0008), ("S", 0x0004), ("A", 0x0002), ("D", 0x0001), ("SPACE", 0x0010), ("CTRL", 0x2000),
        ("Q", 0x8000), ("E", 0x4000), ("TAB", 0x0020), ("ENTER", 0x9000), ("ESC", 0x1000), ("T", 0x0800),
        ("G", 0x0400), ("F", 0x0200), ("H", 0x0100)]
KEY_STICK = [("UP", (0, 85)), ("DOWN", (0, -85)), ("LEFT", (-85, 0)), ("RIGHT", (85, 0))]
BUTTONS = [("SOUTH", 0x8000), ("EAST", 0x4000), ("WEST", 0), ("NORTH", 0), ("BACK", 0x0020), ("START", 0x1000),
           ("LB", 0x0020), ("RB", 0x0010), ("DUP", 0x0800), ("DDOWN", 0x0400), ("DLEFT", 0x0200), ("DRIGHT", 0x0100)]
AXES = [("LX", -32768, 0x0002, None), ("LX", 32767, 0x0001, None), ("LY", -32768, 0x0008, None),
        ("LY", 32767, 0x0004, None), ("RX", -32768, 0, (-85, 0)), ("RX", 32767, 0, (85, 0)),
        ("RY", -32768, 0, (0, 85)), ("RY", 32767, 0, (0, -85)), ("LT", 32767, 0x0010, None),
        ("RT", 32767, 0x2000, None)]
MOUSE = [((10, 0), (20, 0)), ((-10, 0), (-20, 0)), ((0, -10), (0, 20)), ((0, 10), (0, -20)), ((100, 0), (80, 0))]


def build_map_script():
    """Returns (script lines, checks): a check is (label, frame, buttons, stick)."""
    lines, checks, f = [], [], 200

    def press(kind, name, label, buttons, stick=(0, 0)):
        nonlocal f
        lines.append(f"{f} {kind} {name} down")
        lines.append(f"{f + 20} {kind} {name} up")
        checks.append((label, f + 10, buttons, stick))
        checks.append((label + " released", f + 30, 0, (0, 0)))
        f += 40

    for k, b in KEYS:
        press("key", k, f"key {k}", b)
    for k, st in KEY_STICK:
        press("key", k, f"key {k}", 0, st)
    for k, b in BUTTONS:
        press("btn", k, f"pad {k}", b)
    for a, v, b, st in AXES:
        lines.append(f"{f} axis {a} {v}")
        rest = -32768 if a in ("LT", "RT") else 0  # a trigger at rest is the axis minimum
        lines.append(f"{f + 20} axis {a} {rest}")
        checks.append((f"pad {a}={v}", f + 10, b, st or (0, 0)))
        checks.append((f"pad {a} released", f + 30, 0, (0, 0)))
        f += 40
    lines.append(f"{f} mouse 20 0")
    checks.append(("mouse moved, not captured", f, 0, (0, 0)))
    f += 40
    lines.append(f"{f} capture 1")
    f += 5
    for (dx, dy), st in MOUSE:
        lines.append(f"{f} mouse {dx} {dy}")
        checks.append((f"captured mouse {dx},{dy}", f, 0, st))
        checks.append((f"captured mouse {dx},{dy} next frame", f + 1, 0, (0, 0)))
        f += 40
    for m, b in (("LEFT", 0x2000), ("RIGHT", 0x0010)):
        press("mbtn", m, f"mouse {m} (captured)", b)
    lines.append(f"{f} capture 0")
    f += 10
    lines.append(f"{f} unplug")
    f += 20
    press("btn", "SOUTH", "pad SOUTH while unplugged", 0)
    lines.append(f"{f} plug")
    f += 20
    press("btn", "SOUTH", "pad SOUTH after plugging back in", 0x8000)
    return lines, checks, f + 20


def run(args, name, frames, script_lines, extra=()):
    work = os.path.join(args.work, name)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)
    script = os.path.join(work, "pad.txt")
    with open(script, "w") as fp:
        fp.write("\n".join(script_lines) + "\n")
    cmd = [args.bin, "--archive", args.archive, "--game", "--no-window", "--pak-blank", "--frames", str(frames),
           "--pad-test", script, *extra]
    if args.rom:
        cmd += ["--rom", args.rom]
    env = dict(os.environ, T2_PAD_LOG="1", SDL_VIDEODRIVER="offscreen")
    p = subprocess.run(cmd, cwd=work, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                       timeout=600)
    with open(os.path.join(work, "log.txt"), "w") as fp:
        fp.write(p.stdout)
    return p.returncode, p.stdout


def pad_states(log):
    """frame -> (buttons, (x, y)), from the change lines (a state holds until the next one)."""
    changes = []
    for m in re.finditer(r"^pad: frame (\d+) buttons ([0-9A-F]{4}) stick (-?\d+) (-?\d+)", log, re.M):
        changes.append((int(m[1]), int(m[2], 16), (int(m[3]), int(m[4]))))
    return changes


def state_at(changes, frame):
    cur = (0, (0, 0))
    for f, b, st in changes:
        if f > frame:
            break
        cur = (b, st)
    return cur


def check_map(args):
    lines, checks, frames = build_map_script()
    rc, log = run(args, "map", frames, lines)
    changes = pad_states(log)
    bad = 0
    print(f"map: exit {rc}, {len(changes)} pad changes, {len(checks)} checks")
    for label, frame, buttons, stick in checks:
        got_b, got_st = state_at(changes, frame)
        ok = got_b == buttons and got_st == stick
        bad += not ok
        if not ok or args.verbose:
            print(f"  {'ok  ' if ok else 'FAIL'} {label:34s} frame {frame:5d}: buttons {got_b:04X} stick "
                  f"{got_st[0]:4d} {got_st[1]:4d} (want {buttons:04X} {stick[0]:4d} {stick[1]:4d})")
    for what in ("controls: gamepad connected", "controls: gamepad disconnected"):
        if what not in log:
            print(f"  FAIL no '{what}' line")
            bad += 1
    return bad + (rc != 0)


def check_newgame(args):
    bad = 0
    # the presses of playthrough.py's NEW_GAME (START@4400 A@4500 A@4700 A@4800), 4 frames each
    for name, kind, start, a in (("newgame_pad", "btn", "START", "SOUTH"), ("newgame_kb", "key", "ENTER", "ENTER")):
        lines = []
        for f, k in ((4400, start), (4500, a), (4700, a), (4800, a)):
            lines += [f"{f} {kind} {k} down", f"{f + 4} {kind} {k} up"]
        rc, log = run(args, name, 6000, lines)
        modes = [int(m) for m in re.findall(r"engine mode -?\d+ -> (-?\d+)", log)]
        ok = rc == 0 and modes and modes[-1] == 11
        bad += not ok
        print(f"{name}: exit {rc}, engine modes {' '.join(map(str, modes))}: {'ok' if ok else 'FAIL'}")
    return bad


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bin", required=True)
    ap.add_argument("--archive", required=True)
    ap.add_argument("--rom")
    ap.add_argument("--work", default="padtest_out")
    ap.add_argument("--only", choices=("map", "newgame"))
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()
    args.bin, args.archive = os.path.abspath(args.bin), os.path.abspath(args.archive)
    args.rom = os.path.abspath(args.rom) if args.rom else None
    args.work = os.path.abspath(args.work)
    bad = 0
    if args.only in (None, "map"):
        bad += check_map(args)
    if args.only in (None, "newgame"):
        bad += check_newgame(args)
    print("padtest:", "OK" if bad == 0 else f"{bad} FAILED")
    return 0 if bad == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
