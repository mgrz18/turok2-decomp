#!/usr/bin/env python3
"""Reference frames and RDRAM dumps from the user's ROM on a headless N64 emulator (the fidelity
oracle, docs/port/FIDELITY.md).

Drives libmupen64plus (Debian: mupen64plus-ui-console's core, 2.6) through ctypes, under a virtual
X display (xvfb-run), with a video plugin (Rice by default: Glide64mk2 gives black frames under
Xvfb/llvmpipe), the HLE RSP, no audio and the SDL input plugin as a keyboard controller with a
Controller Pak (a blank pak, as the port's --pak-blank). At the given emulator frames it saves a
screenshot (PNG) and/or the 8 MB RDRAM (raw, N64 byte order), and it can press controller buttons
by frame.

The emulator's frame counter is the core's (one per VI with a new frame buffer: a game frame),
not the port's VI count: map scenes by event (STATUS.md's mode switches) or by eye.

    xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/refshots.py \
        --rom baserom.us.z64 --out REFDIR --shots 1100,2000:2400:10 --rdram 1150 \
        [--input "START@1500 A@1600 Z@2000..2050"] [--gfx rice|glide64mk2] [--frames N]

Writes REFDIR/shot_<frame>.png and REFDIR/rdram_<frame>.bin. Do not commit captured images or
dumps (they are derived from the ROM).
"""
import argparse
import ctypes as C
import os
import shutil
import sys
import glob

LIBDIR = "/usr/lib/x86_64-linux-gnu"
CORE = LIBDIR + "/libmupen64plus.so.2"
PLUGDIR = LIBDIR + "/mupen64plus"
DATADIR = "/usr/share/games/mupen64plus"

M64CMD_ROM_OPEN, M64CMD_ROM_CLOSE, M64CMD_EXECUTE, M64CMD_STOP = 1, 2, 5, 6
M64CMD_SEND_SDL_KEYDOWN, M64CMD_SEND_SDL_KEYUP, M64CMD_SET_FRAME_CALLBACK = 13, 14, 15
M64CMD_TAKE_NEXT_SCREENSHOT = 16
M64PLUGIN_RSP, M64PLUGIN_GFX, M64PLUGIN_AUDIO, M64PLUGIN_INPUT = 1, 2, 3, 4
M64P_DBG_PTR_RDRAM = 1

# controller -> keyboard keys of the Input-SDL-Control1 section written below (SDL1 key numbers)
KEYS = {
    "A": 304, "B": 306, "Z": 122, "START": 13, "R": 99, "L": 120,
    "CU": 105, "CD": 107, "CL": 106, "CR": 108,
    "DU": 119, "DD": 115, "DL": 97, "DR": 100,
    "SL": 276, "SR": 275, "SU": 273, "SD": 274,   # the stick, full deflection
}

INPUT_CFG = """[Input-SDL-Control1]
version = 2.000000
mode = 0
device = -1
name = "Keyboard"
plugged = True
plugin = 2
mouse = False
MouseSensitivity = "2.00,2.00"
AnalogDeadzone = "4096,4096"
AnalogPeak = "32768,32768"
DPad R = "key(100)"
DPad L = "key(97)"
DPad D = "key(115)"
DPad U = "key(119)"
Start = "key(13)"
Z Trig = "key(122)"
B Button = "key(306)"
A Button = "key(304)"
C Button R = "key(108)"
C Button L = "key(106)"
C Button D = "key(107)"
C Button U = "key(105)"
R Trig = "key(99)"
L Trig = "key(120)"
Mempak switch = "key(44)"
Rumblepak switch = "key(46)"
X Axis = "key(276,275)"
Y Axis = "key(273,274)"
"""


def frames(spec):
    out = set()
    for part in filter(None, spec.split(",")):
        if ":" in part:
            a, b, s = (part.split(":") + ["1"])[:3]
            out.update(range(int(a), int(b) + 1, int(s)))
        else:
            out.add(int(part))
    return out


def parse_input(spec):
    """'A@100 Z@200..260' -> {frame: [(down?, key)]}"""
    ev = {}
    for tok in spec.split():
        name, at = tok.split("@")
        if ".." in at:
            a, b = (int(x) for x in at.split(".."))
        else:
            a = b = int(at)
            b = a + 3  # a tap: held for a few frames
        k = KEYS[name.upper()]
        ev.setdefault(a, []).append((True, k))
        ev.setdefault(b, []).append((False, k))
    return ev


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--rom", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--shots", default="")
    ap.add_argument("--rdram", default="")
    ap.add_argument("--input", default="")
    ap.add_argument("--gfx", default="rice")
    ap.add_argument("--frames", type=int, default=0, help="stop at this frame (default: the last shot/dump)")
    ap.add_argument("--keep-pak", action="store_true", help="keep the emulator's pak between runs")
    a = ap.parse_args()

    out = os.path.abspath(a.out)
    os.makedirs(out, exist_ok=True)
    cfg = os.path.join(out, "cfg")
    os.makedirs(cfg, exist_ok=True)
    with open(os.path.join(cfg, "mupen64plus.cfg"), "w") as f:
        f.write(INPUT_CFG)
        f.write('\n[Core]\nVersion = 1.010000\nOnScreenDisplay = False\nScreenshotPath = "%s"\n'
                'SaveStatePath = "%s"\nSaveSRAMPath = "%s"\n' % (out + "/sshot", out, out + "/save"))
        f.write('\n[Video-General]\nFullscreen = False\nScreenWidth = 640\nScreenHeight = 480\nVerticalSync = False\n')
    save = os.path.join(out, "save")
    if not a.keep_pak:
        shutil.rmtree(save, ignore_errors=True)
    os.makedirs(save, exist_ok=True)
    shotdir = os.path.join(out, "sshot")
    shutil.rmtree(shotdir, ignore_errors=True)
    os.makedirs(shotdir, exist_ok=True)

    shots, dumps, events = frames(a.shots), frames(a.rdram), parse_input(a.input)
    last = a.frames or max(list(shots) + list(dumps) + [0]) + 2

    core = C.CDLL(CORE, mode=C.RTLD_GLOBAL)
    DEBUGCB = C.CFUNCTYPE(None, C.c_void_p, C.c_int, C.c_char_p)
    STATECB = C.CFUNCTYPE(None, C.c_void_p, C.c_int, C.c_int)
    FRAMECB = C.CFUNCTYPE(None, C.c_uint)

    @DEBUGCB
    def dbg(ctx, level, msg):
        if level <= 2:
            sys.stderr.write("core: %s\n" % msg.decode(errors="replace"))

    @STATECB
    def st(ctx, param, value):
        pass

    rc = core.CoreStartup(0x020001, cfg.encode(), DATADIR.encode(), None, dbg, None, st)
    assert rc == 0, rc
    rom = open(a.rom, "rb").read()
    rc = core.CoreDoCommand(M64CMD_ROM_OPEN, len(rom), C.c_char_p(rom))
    assert rc == 0, rc

    plugins = []
    for ptype, name in ((M64PLUGIN_GFX, "mupen64plus-video-%s.so" % a.gfx),
                        (M64PLUGIN_INPUT, "mupen64plus-input-sdl.so"),
                        (M64PLUGIN_RSP, "mupen64plus-rsp-hle.so")):
        lib = C.CDLL(os.path.join(PLUGDIR, name))
        rc = lib.PluginStartup(C.c_void_p(core._handle), None, dbg)
        assert rc == 0, (name, rc)
        rc = core.CoreAttachPlugin(ptype, C.c_void_p(lib._handle))
        assert rc == 0, (name, rc)
        plugins.append(lib)

    core.DebugMemGetPointer.restype = C.c_void_p
    pending = {}

    @FRAMECB
    def on_frame(n):
        # the screenshot asked for at frame n-1 lands as the core's next file: rename it
        for f in glob.glob(shotdir + "/*.png"):
            os.replace(f, os.path.join(out, "shot_%d.png" % pending.pop("shot", n)))
        for down, k in events.get(n, ()):
            core.CoreDoCommand(M64CMD_SEND_SDL_KEYDOWN if down else M64CMD_SEND_SDL_KEYUP, k, None)
        if n in dumps:
            p = core.DebugMemGetPointer(M64P_DBG_PTR_RDRAM)
            with open(os.path.join(out, "rdram_%d.bin" % n), "wb") as f:
                # RDRAM is kept as host-order 32-bit words: swap back to N64 byte order
                raw = bytearray(C.string_at(p, 8 << 20))
                raw[0::4], raw[1::4], raw[2::4], raw[3::4] = raw[3::4], raw[2::4], raw[1::4], raw[0::4]
                f.write(raw)
        if n in shots:
            pending["shot"] = n
            core.CoreDoCommand(M64CMD_TAKE_NEXT_SCREENSHOT, 0, None)
        if n >= last:
            core.CoreDoCommand(M64CMD_STOP, 0, None)

    core.CoreDoCommand(M64CMD_SET_FRAME_CALLBACK, 0, C.cast(on_frame, C.c_void_p))
    core.CoreDoCommand(M64CMD_EXECUTE, 0, None)
    for f in glob.glob(shotdir + "/*.png"):
        os.replace(f, os.path.join(out, "shot_%d.png" % pending.pop("shot", last)))
    core.CoreDoCommand(M64CMD_ROM_CLOSE, 0, None)
    for lib in plugins:
        lib.PluginShutdown()
    core.CoreShutdown()


if __name__ == "__main__":
    main()
