#!/usr/bin/env python3
"""Per-frame game state of the N64 (the fidelity oracle) in the port's T2_DET_LOG format, to find
the first field where the port leaves the N64 (docs/port/FIDELITY.md section 5, round 10 y3).

Runs the ROM on mupen64plus (the same headless setup as refshots.py: Rice under xvfb-run, the
SDL input plugin as a keyboard controller with a blank Controller Pak) and, from the core's frame
callback, reads RDRAM in place (no dump): the RNG seeds, gFrameCountGameplay, gNextTicks, the
first player and every level actor and pooled instance, at their N64 offsets. The offsets are
computed from include/ with clang for mips (the N64 layout, no TARGET_PORT), the addresses come
from versions/symbols_names.us.txt.

    xvfb-run -a -s "-screen 0 1024x768x24" python3 tools/port/n64trace.py \
        --rom baserom.us.z64 --from 1990 --to 3200 [--actors] [--input "..."] > n64.det

One line set per game frame (a new gFrameCountGameplay), the same fields as the port's
`T2_DET_LOG=1 T2_DET_GF=1 [T2_DET_ACTORS=1]` lines, so tools/port/detdiff.py can align both by
gFrameCountGameplay and print the first differing field. Never commit the traces.

    python3 tools/port/n64trace.py --offsets      # print the N64 offsets used and exit
"""
import argparse
import ctypes as C
import json
import os
import re
import shlex
import struct
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
import refshots as R  # noqa: E402

# name -> (type, member): N64 offset and size from clang --target=mips-linux-gnu
FIELDS = [
    ("app_mode", "CEngineApp", "m_Settings.mode"),
    ("sc_level", "CScene", "m_nLevel"),
    ("sc_actors", "CScene", "m_pActors"),
    ("sc_first", "CScene", "m_nFirstActor"),
    ("sc_nactors", "CScene", "m_nActors"),
    ("sc_pool", "CScene", "m_InstancePool.m_ActiveList.m_pHead"),
    ("game_players", "CGame", "m_PlayerList.m_ActiveList.m_pHead"),
    ("pl_actor", "CPlayer", "m_CharacterActor"),
    ("pl_health", "CPlayer", "m_Data.m_Health"),
    ("pl_weapon", "CPlayer", "m_WeaponActor"),
    ("pl_buttons", "CPlayer", "m_ContState.m_dwButtons"),
    ("pl_movex", "CPlayer", "m_ContState.m_MoveX"),
    ("pl_lookx", "CPlayer", "m_ContState.m_LookX"),
    ("o_pos", "CGameObjectInstance", "header.m_vPos"),
    ("o_intel", "CGameObjectInstance", "header.m_pIntelligence"),
    ("o_roty", "CGameObjectInstance", "m_RotY"),
    ("o_vel", "CGameObjectInstance", "m_vVelocity"),
    ("o_hp", "CGameObjectInstance", "m_AI.m_Health"),
    ("o_mode", "CGameObjectInstance", "m_AI.m_Mode"),
    ("o_act", "CGameObjectInstance", "m_AI.m_Action"),
    ("o_anim", "CGameObjectInstance", "m_CurrentAnimation.m_nAnimIndex"),
    ("o_aframe", "CGameObjectInstance", "m_CurrentAnimation.m_AnimationFrame"),
    ("o_flags", "CGameObjectInstance", "m_dwFlags"),
    ("li_next", "CListedInstance", "m_pNext"),
    ("sc_prt", "CScene", "m_ParticleSystem.m_ActiveList"),
    ("prt_type", "CParticle", "header.m_nObjType"),
    ("prt_pos", "CParticle", "header.m_vPos"),
    ("prt_vel", "CParticle", "m_vVelocity"),
    ("prt_age", "CParticle", "m_cFrame"),
    ("prt_next", "CParticle", "m_pNext"),
    ("li_inst", "CListedInstance", "m_Instance"),
]
SIZEOF = ["CGameObjectInstance"]
HEADERS = ["common.h", "structs/particle.h", "structs/world.h", "structs/player.h", "structs/game.h",
           "structs/game_object.h", "structs/intelligence.h", "structs/collision.h"]
SYMS = ["gRandomValue", "gRandomValueB", "gFrameCountGameplay", "gNextTicks", "gEngineApp",
        "gEngineApp_m_Game", "gScene", "gDemoPlayer"]


def n64_offsets(extra=()):
    """{name: (offset, size)} at the N64 layout; extra: more (name, type, member) triples."""
    fields = FIELDS + list(extra)
    src = "".join('#include "%s"\n' % h for h in HEADERS)
    src += "#define F(T, m) __builtin_offsetof(T, m), sizeof(((T *) 0)->m)\n"
    src += "const int t2_offs[] = {\n"
    src += "".join("  F(%s, %s),\n" % (t, m) for (_, t, m) in fields)
    src += "".join("  sizeof(%s), 0,\n" % t for t in SIZEOF)
    src += "};\n"
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as f:
        f.write(src)
        path = f.name
    try:
        asm = subprocess.run(
            ["clang", "--target=mips-linux-gnu", "-S", "-o", "-", "-std=gnu89", "-fsigned-char",
             "-DF3DEX_GBI_2", "-D_LANGUAGE_C", '-DGAME_VERSION="us"', "-w",
             "-I" + os.path.join(ROOT, "include"), path],
            capture_output=True, text=True, check=True).stdout
    finally:
        os.unlink(path)
    vals = [int(v, 0) for v in re.findall(r"\.4byte\s+(\S+)", asm)]
    out = {}
    for i, (name, _, _) in enumerate(fields):
        out[name] = (vals[2 * i], vals[2 * i + 1])
    for k, t in enumerate(SIZEOF):
        out["sizeof_" + t] = (vals[2 * len(fields) + 2 * k], 0)
    return out


def symbols(extra=()):
    out = {}
    want = set(SYMS) | set(extra)
    for line in open(os.path.join(ROOT, "versions", "symbols_names.us.txt")):
        m = re.match(r"\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)", line)
        if m and m.group(1) in want and m.group(1) not in out:
            out[m.group(1)] = int(m.group(2), 16)
    missing = want - set(out)
    assert not missing, missing
    return out


class Ram:
    """RDRAM as the core keeps it: host-order (little-endian) 32-bit words."""

    def __init__(self, ptr):
        self.m = (C.c_uint8 * (8 << 20)).from_address(ptr)

    def u32(self, a):
        return struct.unpack_from("<I", self.m, a & 0x7FFFFF)[0]

    def s16(self, a):
        return struct.unpack_from("<h", self.m, (a & 0x7FFFFF) ^ 2)[0]

    def s8(self, a):
        return struct.unpack_from("<b", self.m, (a & 0x7FFFFF) ^ 3)[0]

    def sval(self, a, size):
        if size == 4:
            v = self.u32(a)
            return v - (1 << 32) if v & 0x80000000 else v
        if size == 2:
            return self.s16(a)
        return self.s8(a)

    def f(self, a):
        return struct.unpack("<f", struct.pack("<I", self.u32(a)))[0]


def valid(p):
    return 0x80000000 <= p < 0x80800000


class Tracer:
    def __init__(self, ram, offs, syms, actors, out):
        self.r, self.o, self.s, self.actors, self.out = ram, offs, syms, actors, out
        self.last_gf = None

    def actor(self, h, a, kind, i, verbose):
        r, o = self.r, self.o
        pos = o["o_pos"][0]
        v = [r.u32(a + pos), r.u32(a + pos + 4), r.u32(a + pos + 8), r.u32(a + o["o_roty"][0]),
             r.sval(a + o["o_hp"][0], o["o_hp"][1]) & 0xFFFFFFFF,
             r.sval(a + o["o_mode"][0], o["o_mode"][1]) & 0xFFFFFFFF,
             r.sval(a + o["o_act"][0], o["o_act"][1]) & 0xFFFFFFFF,
             r.sval(a + o["o_anim"][0], o["o_anim"][1]) & 0xFFFFFFFF,
             r.u32(a + o["o_aframe"][0]), r.u32(a + o["o_flags"][0])]
        for x in v:
            for k in range(4):
                h ^= (x >> (k * 8)) & 0xFF
                h = (h * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
        if verbose:
            fl = [r.f(a + pos), r.f(a + pos + 4), r.f(a + pos + 8)]
            sv = [x - (1 << 32) if x & 0x80000000 else x for x in v[4:8]]
            self.out.write("det:  %s %d pos %08x %08x %08x (%.3f %.3f %.3f) rot %08x hp %d mode %d act %d "
                           "anim %d %08x fl %x\n" % (kind, i, v[0], v[1], v[2], fl[0], fl[1], fl[2], v[3],
                                                     sv[0], sv[1], sv[2], sv[3], v[8], v[9]))
        return h

    def frame(self, n):
        r, o, s = self.r, self.o, self.s
        gf = r.u32(s["gFrameCountGameplay"])
        if gf == self.last_gf:
            return
        self.last_gf = gf
        sc = s["gScene"]
        self.out.write("det: frame %d gf %d ticks %d mode %d level %d rng %08x %08x" % (
            n, gf, r.sval(s["gNextTicks"], 4), r.sval(s["gEngineApp"] + o["app_mode"][0], o["app_mode"][1]),
            r.sval(sc + o["sc_level"][0], o["sc_level"][1]), r.u32(s["gRandomValue"]), r.u32(s["gRandomValueB"])))
        p = r.u32(s["gEngineApp_m_Game"] + o["game_players"][0])
        if valid(p):
            a = p + o["pl_actor"][0]
            pos, vel = a + o["o_pos"][0], a + o["o_vel"][0]
            self.out.write(" player %08x %08x %08x rot %08x vel %08x %08x %08x hp %d mode %d btn %x stick %08x %08x" % (
                r.u32(pos), r.u32(pos + 4), r.u32(pos + 8), r.u32(a + o["o_roty"][0]),
                r.u32(vel), r.u32(vel + 4), r.u32(vel + 8),
                r.sval(p + o["pl_health"][0], o["pl_health"][1]), r.sval(a + o["o_mode"][0], o["o_mode"][1]),
                r.u32(p + o["pl_buttons"][0]), r.u32(p + o["pl_movex"][0]), r.u32(p + o["pl_lookx"][0])))
        self.out.write("\n")
        if valid(p) and self.actors:
            self.actor(0, p + o["pl_weapon"][0], "wpn", 0, True)
        h, cnt = 0xCBF29CE484222325, 0
        base, first, na = (r.u32(sc + o["sc_actors"][0]), r.sval(sc + o["sc_first"][0], 4),
                           r.sval(sc + o["sc_nactors"][0], 4))
        stride = o["sizeof_CGameObjectInstance"][0]
        if valid(base):
            for i in range(max(first, 0), min(na, 4096)):
                a = base + i * stride
                h = self.actor(h, a, "lvl", i, self.actors and r.u32(a + o["o_intel"][0]) != 0)
                cnt += 1
        li = r.u32(sc + o["sc_pool"][0])
        k = 0
        while valid(li) and k < 256:
            h = self.actor(h, li + o["li_inst"][0], "dyn", k, self.actors)
            li = r.u32(li + o["li_next"][0])
            k += 1
            cnt += 1
        if self.actors:
            for j in range(3):
                q = r.u32(sc + o["sc_prt"][0] + j * 0x14)
                k = 0
                while valid(q) and k < 128:
                    pos, vel = q + o["prt_pos"][0], q + o["prt_vel"][0]
                    self.out.write("det:  prt %d %d type %d pos %08x %08x %08x vel %08x %08x %08x age %08x\n" % (
                        j, k, r.sval(q + o["prt_type"][0], o["prt_type"][1]), r.u32(pos), r.u32(pos + 4),
                        r.u32(pos + 8), r.u32(vel), r.u32(vel + 4), r.u32(vel + 8), r.u32(q + o["prt_age"][0])))
                    q = r.u32(q + o["prt_next"][0])
                    k += 1
        self.out.write("det: frame %d actors %d hash %016x\n" % (n, cnt, h))
        self.out.flush()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--rom")
    ap.add_argument("--out", default=None, help="the emulator's scratch dir (cfg, pak); default a temp dir")
    ap.add_argument("--from", dest="start", type=int, default=0, help="first emulator frame traced")
    ap.add_argument("--to", type=int, default=0, help="last emulator frame (stops there)")
    ap.add_argument("--actors", action="store_true", help="one line per live actor (T2_DET_ACTORS)")
    ap.add_argument("--input", default="")
    ap.add_argument("--gfx", default="rice")
    ap.add_argument("--offsets", action="store_true")
    ap.add_argument("--args", default="",
                    help="with --calls: REG:TYPE:MEMBER[:f|x|d],... printed at each call, read through the "
                         "argument register REG (a0-a3) as a TYPE * (N64 offsets), or at a global "
                         "symbol REG (e.g. gCollisionResults:CCollisionResults:m_nInstanceNode:d), e.g. "
                         "a2:CAITargets:m_Sight.m_Dist:f,a1:CAI:m_Enemy.m_AttackDelay:f")
    ap.add_argument("--calls", default="",
                    help="SYM[,SYM...]: also print 'call: gf G SYM ra RA' each time the CPU enters these "
                         "functions in the traced range (e.g. Random_Next,Random_NextB: who draws a number)")
    ap.add_argument("--at", default="CEngineApp__Update",
                    help="read the state when the CPU reaches this function (the core's debugger, a "
                         "consistent point of the game loop); 'vi' reads it from the frame callback instead "
                         "(fast, but at a varying point of the frame: noisy)")
    a = ap.parse_args()

    calls = [c for c in a.calls.split(",") if c]
    use_bp = a.at != "vi" or bool(calls)
    argspec = []
    for k, spec in enumerate(x for x in a.args.split(",") if x):
        reg, typ, mem = spec.split(":")[:3]
        fmt = (spec.split(":") + ["x"])[3] if len(spec.split(":")) > 3 else "x"
        where = 4 + "0123".index(reg[1]) if re.match(r"a[0-3]$", reg) else reg  # a register or a global
        argspec.append(("arg%d" % k, typ, mem, where, fmt, spec))
    offs = n64_offsets([(n, t, m) for (n, t, m, _, _, _) in argspec])
    syms = symbols(([a.at] if a.at != "vi" and not a.at.startswith("0x") else []) + calls
                   + [r for (_, _, _, r, _, _) in argspec if isinstance(r, str)])
    bp_addr = (int(a.at, 16) if a.at.startswith("0x") else syms[a.at]) if a.at != "vi" else None
    call_at = {syms[c]: c for c in calls}
    if a.offsets:
        for k, (o, s) in offs.items():
            print("%-28s 0x%05x %d" % (k, o, s))
        for k, v in syms.items():
            print("%-28s 0x%08x" % (k, v))
        return 0
    assert a.rom and a.to, "--rom and --to are needed"

    out = os.path.abspath(a.out or tempfile.mkdtemp(prefix="n64trace"))
    cfg = os.path.join(out, "cfg")
    save = os.path.join(out, "save")
    for d in (cfg, save):
        os.makedirs(d, exist_ok=True)
    for f in os.listdir(save):
        os.unlink(os.path.join(save, f))  # a blank pak per run (the port's --pak-blank)
    with open(os.path.join(cfg, "mupen64plus.cfg"), "w") as f:
        f.write(R.INPUT_CFG)
        f.write('\n[Core]\nVersion = 1.010000\nOnScreenDisplay = False\nScreenshotPath = "%s"\n'
                'SaveStatePath = "%s"\nSaveSRAMPath = "%s"\n' % (out, out, save))
        if use_bp:  # breakpoints need the debugger, which needs an interpreter (1: cached)
            f.write('EnableDebugger = True\nR4300Emulator = 1\n')
        f.write('\n[Video-General]\nFullscreen = False\nScreenWidth = 320\nScreenHeight = 240\nVerticalSync = False\n')
    events = R.parse_input(a.input)

    core = C.CDLL(R.CORE, mode=C.RTLD_GLOBAL)
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

    assert core.CoreStartup(0x020001, cfg.encode(), R.DATADIR.encode(), None, dbg, None, st) == 0
    rom = open(a.rom, "rb").read()
    assert core.CoreDoCommand(R.M64CMD_ROM_OPEN, len(rom), C.c_char_p(rom)) == 0
    plugins = []
    for ptype, name in ((R.M64PLUGIN_GFX, "mupen64plus-video-%s.so" % a.gfx),
                        (R.M64PLUGIN_INPUT, "mupen64plus-input-sdl.so"),
                        (R.M64PLUGIN_RSP, "mupen64plus-rsp-hle.so")):
        lib = C.CDLL(os.path.join(R.PLUGDIR, name))
        assert lib.PluginStartup(C.c_void_p(core._handle), None, dbg) == 0, name
        assert core.CoreAttachPlugin(ptype, C.c_void_p(lib._handle)) == 0, name
        plugins.append(lib)
    core.DebugMemGetPointer.restype = C.c_void_p

    state = {"n": 0}

    def trace():
        if state["n"] >= a.start:
            if "t" not in state:
                state["t"] = Tracer(Ram(core.DebugMemGetPointer(R.M64P_DBG_PTR_RDRAM)), offs, syms,
                                    a.actors, sys.stdout)
            state["t"].frame(state["n"])

    @FRAMECB
    def on_frame(n):
        state["n"] = n
        if n % 250 == 0:
            sys.stderr.write("n64trace: emulator frame %d\n" % n)
        for down, k in events.get(n, ()):
            core.CoreDoCommand(R.M64CMD_SEND_SDL_KEYDOWN if down else R.M64CMD_SEND_SDL_KEYUP, k, None)
        if not use_bp:
            trace()
        if n >= a.to:
            core.CoreDoCommand(R.M64CMD_STOP, 0, None)

    # the debugger: the core pauses on the breakpoint and calls dbg_update(pc) from its own thread;
    # read the state there and let it run on (run state RUNNING + one step posts its semaphore)
    VOIDCB = C.CFUNCTYPE(None)
    PCCB = C.CFUNCTYPE(None, C.c_uint)

    @VOIDCB
    def dbg_init():
        core.DebugSetRunState(2)
        core.DebugStep()

    @PCCB
    def dbg_update(pc):
        if pc == bp_addr:
            trace()
        elif pc in call_at and state["n"] >= a.start:
            if "regs" not in state:
                core.DebugGetCPUDataPtr.restype = C.c_void_p
                state["regs"] = (C.c_int64 * 32).from_address(core.DebugGetCPUDataPtr(2))  # M64P_CPU_REG_REG
            ram = Ram(core.DebugMemGetPointer(R.M64P_DBG_PTR_RDRAM))
            regs = state["regs"]
            sys.stdout.write("call: frame %d gf %d %s ra %08x a0-a3 %08x %08x %08x %08x" % (
                state["n"], ram.u32(syms["gFrameCountGameplay"]), call_at[pc], regs[31] & 0xFFFFFFFF,
                regs[4] & 0xFFFFFFFF, regs[5] & 0xFFFFFFFF, regs[6] & 0xFFFFFFFF, regs[7] & 0xFFFFFFFF))
            for (n, t, m, r, fmt, spec) in argspec:
                base = syms[r] if isinstance(r, str) else regs[r] & 0xFFFFFFFF
                if not valid(base):
                    continue
                off, size = offs[n]
                if fmt == "f":
                    v = "%g" % ram.f(base + off)
                elif fmt == "d":
                    v = "%d" % ram.sval(base + off, size)
                else:
                    v = "%x" % (ram.sval(base + off, size) & ((1 << (8 * size)) - 1))
                sys.stdout.write(" %s=%s" % (m, v))
            sys.stdout.write("\n")
        core.DebugSetRunState(2)
        core.DebugStep()

    @VOIDCB
    def dbg_vi():
        pass

    if use_bp:
        class Bkpt(C.Structure):
            _fields_ = [("address", C.c_uint32), ("endaddr", C.c_uint32), ("flags", C.c_uint)]
        assert core.DebugSetCallbacks(dbg_init, dbg_update, dbg_vi) == 0
        state["cbs"] = (dbg_init, dbg_update, dbg_vi)
        for addr in ([bp_addr] if bp_addr is not None else []) + list(call_at):
            bk = Bkpt(addr, addr, 0x01 | 0x08)  # ENABLED | EXEC
            r = core.DebugBreakpointCommand(2, 0, C.byref(bk))  # ADD_STRUCT
            assert r >= 0, r

    core.CoreDoCommand(R.M64CMD_SET_FRAME_CALLBACK, 0, C.cast(on_frame, C.c_void_p))
    core.CoreDoCommand(R.M64CMD_EXECUTE, 0, None)
    core.CoreDoCommand(R.M64CMD_ROM_CLOSE, 0, None)
    for lib in plugins:
        lib.PluginShutdown()
    core.CoreShutdown()
    return 0


if __name__ == "__main__":
    sys.exit(main())
