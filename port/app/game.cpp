// game.cpp - `turok2 --game`: run the linked game on the port runtime (docs/port/LINK.md §4).
//
// main.cpp has built the LUS context, mounted the archives and (unless --no-window) opened the
// Fast3D window. This file boots the game (T2Port_Boot: arena, ROM, t2os, the mainproc thread)
// and pumps frames: per host frame one VI retrace, the game threads run until all are blocked,
// then the display list the scheduler started is (with --draw) handed to Fast3D. Controllers
// come from LUS's ControlDeck through the runtime's hooks.
#include <libultraship/libultraship.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <memory>

#include "fast/Fast3dWindow.h"
#include "t2controls.h"
#include "fast/interpreter.h"
#include "fast/backends/gfx_rendering_api.h"

#include <cstdlib>
#include <cmath>
#include <vector>
#include <thread>
#include <pthread.h>

extern "C" {
#include "t2port.h"
#include "t2audio.h"
// port/game/t2debug.c
int T2Port_EngineMode(void);
void T2Port_LogState(int frame);
void T2Port_LogDet(int frame);
void T2Port_RawDump(const char *spec);
void T2Port_GiveWeapons(int frame); // DEBUG-ONLY: T2_GIVE_WEAPONS=1 (t2debug.c)
void T2Port_DebugWarp(int frame);
void T2Port_ProgressLog(int frame);
void T2Port_SetTotems(void);
void T2Port_Teleport(int frame, const char *spec);
int T2Port_ScheduleFrame(int frame);
extern int gT2HostFrame;
void T2Port_WakeEnemies(int frame, const char *spec);
void T2Port_FaceNearest(int frame, const char *spec);
void T2Port_ScribbleStack(unsigned value);
int T2Port_AimNode(int frame, const char *spec);
void T2Port_BossFront(int frame, const char *spec);
void T2Port_LairPeek(int frame, const char *spec);
void T2Port_PrimagenStage(int frame, const char *spec);
void T2Port_StartWarp(int frame, const char *spec);
void T2Port_RngSet(int frame, const char *spec);
void T2Port_LegacyShakeTick(int frame);
int T2Port_LegacyNoSound(void);
void T2Port_TraceBoss(int frame);
void T2Port_AimAt(int frame, const char *spec);
void T2Port_AimEnemy(int frame, const char *spec);
int T2Port_FireGate(int frame);
int T2Port_AimPose(int frame, char *buf, int n);
int T2Port_MovePose(int frame, char *buf, int n);
void T2Port_Cheats(int frame, const char *spec, const char *fill);
void T2Port_SavePortal(int frame, const char *spec);
void T2Port_DoorOpen(int frame, const char *spec);
void T2Port_BitsLog(int frame, const char *spec);
void T2Port_DumpActor(int frame, const char *spec);
void T2Port_GiveItems(int frame, const char *spec);
void T2Port_HurtLog(int frame, int sched, const char *spec);
void T2Port_DebugSwitches(int skipPak, int startMode);
}

#include "game.h"
#include "offscreen_gl.h"
#include "t2display.h"
#include "t2audiodev.h"

// the game's screen size (render.h; Video_SetMode writes it)
extern "C" int32_t gScreenWidth, gScreenHeight;
extern "C" const void* T2Game_TopMenu(int* cursor); // port/game/t2modelog.c

namespace {

// T2_DLDUMP=1: print the commands of the last frame's display list (following G_DL), for
// bring-up. Words are host-sized (gfx.h GfxWord).
// T2_DLDUMP_VTX=1 also prints the vertices of each G_VTX (the first 4; =2: all), segments from
// G_MW_SEGMENT, the 16 words of each G_MTX (s15.16, Fast3D's host-order int32[16]) and a hash
// of the first 256 bytes of each G_SETTIMG image (docs/port/RENDER.md, MACOS.md section 9).
uintptr_t sDumpSegments[16];
void DumpVtxMtx(uint8_t op, uint32_t w0, uintptr_t w1, int depth) {
    static const bool on = getenv("T2_DLDUMP_VTX") != nullptr;
    if (!on) {
        return;
    }
    if (op == 0xDB && ((w0 >> 16) & 0xFF) == 0x06) {
        sDumpSegments[((w0 & 0xFFFF) / 4) & 15] = w1;
        return;
    }
    uintptr_t addr = w1;
    if (addr & 1) {
        addr = sDumpSegments[(addr >> 24) & 15] + (addr & 0x00FFFFFE);
    }
    if (op == 0x01) {
        int n = (w0 >> 12) & 0xFF;
        const int16_t* v = (const int16_t*)addr;
        static const int maxV = getenv("T2_DLDUMP_VTX") != nullptr && atoi(getenv("T2_DLDUMP_VTX")) > 1 ? 64 : 4;
        for (int i = 0; i < n && i < maxV; i++, v += 8) {
            const uint8_t* c = (const uint8_t*)(v + 6);
            printf("%*s   v%d ob %d %d %d flag %d tc %d %d cn %02X%02X%02X%02X\n", depth * 2, "", i, v[0], v[1], v[2],
                   v[3], v[4], v[5], c[0], c[1], c[2], c[3]);
        }
    } else if (op == 0xFD && addr >= 0x10000 && !(addr & 1)) {
        // G_SETTIMG: fnv1a64 of the image's first 256 bytes (texture data differences between hosts)
        uint64_t h = 1469598103934665603ull;
        for (int i = 0; i < 256; i++) {
            h = (h ^ ((const uint8_t*)addr)[i]) * 1099511628211ull;
        }
        printf("%*s   timg %016llx hash\n", depth * 2, "", (unsigned long long)h);
        if (atoi(getenv("T2_DLDUMP_VTX")) > 2) {
            printf("%*s   timg bytes", depth * 2, "");
            for (int i = 0; i < 32; i++) {
                printf(" %02x", ((const uint8_t*)addr)[i]);
            }
            printf("\n");
        }
    } else if (op == 0xDC && addr >= 0x10000 && !(addr & 1)) {
        // G_MOVEMEM (viewport, lights): the first 16 bytes (=2 and up)
        if (atoi(getenv("T2_DLDUMP_VTX")) > 1) {
            printf("%*s   mem", depth * 2, "");
            for (int i = 0; i < 16; i++) {
                printf(" %02x", ((const uint8_t*)addr)[i]);
            }
            printf("\n");
        }
    } else if (op == 0xDA) {
        const int32_t* m = (const int32_t*)addr;
        printf("%*s   mtx", depth * 2, "");
        for (int i = 0; i < 16; i++) {
            // the integer halves (m[0..7], pairs) and the fractions (m[8..15]): element i of row r
            int r = i / 4, c = i % 4;
            int32_t ip = (int16_t)(c & 1 ? m[r * 2 + c / 2] & 0xFFFF : (uint32_t)m[r * 2 + c / 2] >> 16);
            uint32_t fp = c & 1 ? m[8 + r * 2 + c / 2] & 0xFFFF : (uint32_t)m[8 + r * 2 + c / 2] >> 16;
            printf(" %.3f", ip + fp / 65536.0);
        }
        printf("\n");
    }
}

void DumpDL(const uintptr_t* dl, int depth, int* budget) {
    for (; *budget > 0; dl += 2) {
        (*budget)--;
        uint32_t w0 = (uint32_t)dl[0];
        uint8_t op = w0 >> 24;
        printf("%*s%02X %08X %016llX\n", depth * 2, "", op, w0, (unsigned long long)dl[1]);
        DumpVtxMtx(op, w0, dl[1], depth);
        if (op == 0xDE) {
            if (dl[1] & 1) {
                continue; // segmented: not followed
            }
            DumpDL((const uintptr_t*)dl[1], depth + 1, budget);
            if ((w0 >> 16 & 0xFF) == 1) {
                return; // branch
            }
        } else if (op == 0xDF) {
            return;
        }
    }
}

// --offscreen-gl / T2_DLCHECK=1: a display list whose address words cannot be host pointers
// (64-bit hosts only: a value below 4 GB that is not a segmented address is a truncated pointer,
// harmless on Linux low memory, fatal in Fast3D on macOS). Returns the number of bad words and
// prints the first few of each run with their position (MACOS.md section 8).
bool AddrOk(uintptr_t w1) {
    if (sizeof(uintptr_t) < 8 || w1 == 0) {
        return true;
    }
    if ((w1 & 1) && w1 < 0x10000000u) {
        return true; // GBI_SEGADDR: segment-relative
    }
    return w1 >= 0x100000000ull;
}
int CheckDL(const uintptr_t* dl, int depth, int* budget, int frame) {
    int bad = 0;
    static int reported = 0;
    for (; *budget > 0; dl += 2) {
        (*budget)--;
        uint32_t w0 = (uint32_t)dl[0];
        uint8_t op = w0 >> 24;
        bool addr = op == 0x01 || op == 0xDA || op == 0xDC || op == 0xDE || op == 0xFD || op == 0xFE || op == 0xFF ||
                    (op == 0xDB && ((w0 >> 16) & 0xFF) == 0x06);
        if (addr && !AddrOk(dl[1])) {
            bad++;
            // one report per (command, the three commands before it), up to 60
            static std::vector<uint64_t> seen;
            uint64_t sig = (uint64_t)w0 << 32 ^ (uint32_t)dl[-2] * 31u ^ (uint32_t)dl[-4] * 17u ^ (uint32_t)dl[-6];
            bool fresh = std::find(seen.begin(), seen.end(), sig) == seen.end();
            if (fresh && reported < 60) {
                seen.push_back(sig);
                reported++;
                printf("dlcheck: frame %d: %02X %08X %016llX at %p (depth %d); before:", frame, op, w0,
                       (unsigned long long)dl[1], (const void*)dl, depth);
                for (int k = 3; k >= 1; k--) {
                    printf(" %08X:%llX", (uint32_t)dl[-2 * k], (unsigned long long)dl[-2 * k + 1]);
                }
                printf("\n");
            }
            if (op == 0xDE) {
                if ((w0 >> 16 & 0xFF) == 1) {
                    return bad;
                }
                continue;
            }
        }
        if (op == 0xDE) {
            if (dl[1] & 1) {
                continue;
            }
            bad += CheckDL((const uintptr_t*)dl[1], depth + 1, budget, frame);
            if ((w0 >> 16 & 0xFF) == 1) {
                return bad;
            }
        } else if (op == 0xDF) {
            return bad;
        }
    }
    return bad;
}

OSMesgQueue sContQueue;
OSMesg sContMsg[1];

// LUS's osContInit/osContGetReadData (this file does not see port/include/t2_rename.h).
int ContInit(uint8_t* bits) {
    OSContStatus status[MAXCONTROLLERS] = {};
    osCreateMesgQueue(&sContQueue, sContMsg, 1);
    const int rc = osContInit(&sContQueue, bits, status);
    T2Controls_AfterContInit(); // the pads present at start, --pad-test's virtual pad
    return rc;
}

// --input: scripted input on port 1 for headless runs (debug aid, docs/port/INPUT.md section 3).
// Entries are separated by commas or whitespace; "@FILE" reads them from a file (# comments).
//   BUTTON@F        press BUTTON for kInputHold host frames from frame F on
//   BUTTON@F1..F2   hold BUTTON from frame F1 to F2 (inclusive)
//   SX=V@F1..F2     stick X = V (-128..127, + = right) over the range (SY: + = up);
//   SX=V@F          ... for kInputHold frames. Overlapping stick entries: the last one wins.
// Buttons: A B Z START DU DD DL DR L R CU CD CL CR.
struct ScriptedPress {
    int frame;
    int last; // inclusive
    uint16_t button;
    int axis; // 0 = button, 1 = stick X, 2 = stick Y
    int value;
};
std::vector<ScriptedPress> sScript;
int sFrame;
constexpr int kInputHold = 4;

bool ParseInputItem(const std::string& item) {
    static const struct {
        const char* name;
        uint16_t bit;
    } kButtons[] = { { "A", 0x8000 },  { "B", 0x4000 },  { "Z", 0x2000 },  { "START", 0x1000 }, { "DU", 0x0800 },
                     { "DD", 0x0400 }, { "DL", 0x0200 }, { "DR", 0x0100 }, { "L", 0x0020 },     { "R", 0x0010 },
                     { "CU", 0x0008 }, { "CD", 0x0004 }, { "CL", 0x0002 }, { "CR", 0x0001 } };
    size_t at = item.find('@');
    if (at == std::string::npos) {
        return false;
    }
    std::string name = item.substr(0, at);
    std::string range = item.substr(at + 1);
    ScriptedPress p = {};
    size_t dots = range.find("..");
    char* endp = nullptr;
    p.frame = (int)strtol(range.c_str(), &endp, 10);
    if (endp == range.c_str()) {
        return false;
    }
    if (dots != std::string::npos) {
        const char* l = range.c_str() + dots + 2;
        p.last = (int)strtol(l, &endp, 10);
        if (endp == l || p.last < p.frame) {
            return false;
        }
    } else {
        p.last = p.frame + kInputHold - 1;
    }
    size_t eq = name.find('=');
    if (eq != std::string::npos) {
        std::string axis = name.substr(0, eq);
        if (axis == "SX") {
            p.axis = 1;
        } else if (axis == "SY") {
            p.axis = 2;
        } else {
            return false;
        }
        const char* v = name.c_str() + eq + 1;
        p.value = (int)strtol(v, &endp, 10);
        if (endp == v || p.value < -128 || p.value > 127) {
            return false;
        }
        sScript.push_back(p);
        return true;
    }
    for (auto& b : kButtons) {
        if (name == b.name) {
            p.button = b.bit;
        }
    }
    if (p.button == 0) {
        return false;
    }
    sScript.push_back(p);
    return true;
}

bool ParseInputScript(const std::string& spec) {
    std::string text = spec;
    if (!spec.empty() && spec[0] == '@') {
        FILE* fp = fopen(spec.c_str() + 1, "r");
        if (!fp) {
            fprintf(stderr, "--input: cannot open %s\n", spec.c_str() + 1);
            return false;
        }
        text.clear();
        char line[512];
        while (fgets(line, sizeof line, fp)) {
            char* hash = strchr(line, '#');
            if (hash) {
                *hash = '\0';
            }
            text += line;
            text += ' ';
        }
        fclose(fp);
    }
    std::string item;
    for (size_t i = 0; i <= text.size(); i++) {
        char c = i < text.size() ? text[i] : ',';
        if (c == ',' || c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (!item.empty() && !ParseInputItem(item)) {
                fprintf(stderr, "--input: bad entry '%s'\n", item.c_str());
                return false;
            }
            item.clear();
        } else {
            item += c;
        }
    }
    return true;
}

uint16_t ScriptedButtons() {
    uint16_t b = 0;
    for (auto& p : sScript) {
        if (p.axis == 0 && sFrame >= p.frame && sFrame <= p.last) {
            b |= p.button;
        }
    }
    return b;
}

// The scripted stick: true (and x/y set) when an SX/SY entry covers this frame.
bool ScriptedStick(int8_t* x, int8_t* y) {
    bool any = false;
    for (auto& p : sScript) {
        if (p.axis != 0 && sFrame >= p.frame && sFrame <= p.last) {
            *(p.axis == 1 ? x : y) = (int8_t)p.value;
            any = true;
        }
    }
    return any;
}

// T2_AIM_FIFO=DIR (debug, round 15 d3): the stick driver of T2_AIM_STICK (tools/port/aimdrive.py).
// After each frame in which an aim assist reported a target (t2debug.c, T2Port_AimPose), one line
// goes to the fifo DIR/out ("aim F px py pz rotY ex ey ez pitch tx ty tz") and one line comes back
// from DIR/in: the next frame's input on port 1, over the script's.
// Input only: the driver sees what the game drew and answers with a stick, as a player would.
// T2_MOVE_DRIVE=F1-F2[;...] (debug, round 15 C30; also T2_WALK_PLAN's windows, round 16 e3): inside
// these windows a second line goes out every frame, "move F px py pz rotY bx by bz" (t2bosslog.c,
// T2Port_MovePose: the player and the level boss). Round 16 e2: the aim line also carries the
// candidate targets after the chosen one and the player's health (t2debug.c, T2Port_AimPose).
//
// The answer, one protocol for every line kind and every driver (round 16 merge, PLAYTHROUGH.md 31):
//   "SX SY [TOKEN ...]"
// SX SY: the stick (-128..127), or "- -" to keep the script's stick. Each optional token sets one
// button group over the script's for the next frame; a group no token names stays the script's:
//   cH       the C buttons, hex mask U 8, D 4, L 2, R 1 (c0 releases all four): lair and walk drivers
//   z0 / z1  the Z trigger (fire) released / held: the aim driver's fire (T2_FIRE_GATE defers to it)
//   r0 / r1  the R trigger (jump) released / held
// When an aim and a move line both go out in one frame, their tokens add up (the move answer last).
struct AimStick {
    bool valid;
    int8_t x, y;
    uint16_t mask;    // the N64 button bits the driver sets this frame (C 0x000F, R 0x0010, Z 0x2000)
    uint16_t buttons; // their values
};
AimStick sAimStick;

// The tokens after "SX SY" of a driver answer (above).
void AimFifoButtons(const char* reply, int frame) {
    char tok[3][16];
    int n = sscanf(reply, "%*s %*s %15s %15s %15s", tok[0], tok[1], tok[2]);
    for (int k = 0; k < n; k++) {
        unsigned v;
        uint16_t bits = tok[k][0] == 'c' ? 0x000F : tok[k][0] == 'z' ? 0x2000 : tok[k][0] == 'r' ? 0x0010 : 0;
        if (bits == 0 || sscanf(tok[k] + 1, "%x", &v) != 1) {
            static bool warned = false;
            if (!warned) {
                fprintf(stderr, "T2_AIM_FIFO: frame %d: unknown answer token \"%s\" (cH, z0/z1, r0/r1)\n", frame,
                        tok[k]);
                warned = true;
            }
            continue;
        }
        sAimStick.mask |= bits;
        sAimStick.buttons =
            (uint16_t)((sAimStick.buttons & ~bits) | (bits == 0x000F ? (v & 0xF) : (v != 0 ? bits : 0)));
    }
}

// The driver's buttons over the pad's (T2_AIM_FIFO).
uint16_t AimFifoApply(uint16_t b) {
    return (uint16_t)((b & ~sAimStick.mask) | (sAimStick.buttons & sAimStick.mask));
}

void AimFifoExchange(int frame, const char* dir) {
    static FILE* out = nullptr;
    static FILE* in = nullptr;
    static bool broken = false;
    sAimStick.valid = false;
    sAimStick.mask = 0;
    sAimStick.buttons = 0;
    if (broken) {
        return;
    }
    if (out == nullptr) {
        std::string o = std::string(dir) + "/out", i = std::string(dir) + "/in";
        out = fopen(o.c_str(), "w"); // blocks until the driver opens it (a fifo)
        in = out != nullptr ? fopen(i.c_str(), "r") : nullptr;
        if (out == nullptr || in == nullptr) {
            fprintf(stderr, "T2_AIM_FIFO: cannot open %s/out and %s/in\n", dir, dir);
            broken = true;
            return;
        }
    }
    char line[2048]; // round 16 e2: the aim line carries up to 8 candidates
    for (int kind = 0; kind < 2; kind++) {
        if (!(kind == 0 ? T2Port_AimPose(frame, line, sizeof line) : T2Port_MovePose(frame, line, sizeof line))) {
            continue;
        }
        char reply[64];
        if (fputs(line, out) < 0 || fflush(out) != 0 || fgets(reply, sizeof reply, in) == nullptr) {
            fprintf(stderr, "T2_AIM_FIFO: the driver went away at frame %d\n", frame);
            broken = true;
            return;
        }
        int sx, sy;
        if (sscanf(reply, "%d %d", &sx, &sy) == 2 && sx >= -128 && sx <= 127 && sy >= -128 && sy <= 127) {
            sAimStick.valid = true;
            sAimStick.x = (int8_t)sx;
            sAimStick.y = (int8_t)sy;
        }
        AimFifoButtons(reply, frame);
    }
}

// The script's buttons. T2_FIRE_GATE=... (DEBUG-ONLY, round 16 e5): the script's Z is held back
// while the aimed actor is asleep or far (t2bosslog.c, T2Port_FireGate), unless the stick driver
// sets Z itself this frame (a "z" token: the driver's fire wins, round 16 merge).
uint16_t ScriptedButtonsGated() {
    uint16_t sb = ScriptedButtons();
    static const bool fireGate = getenv("T2_FIRE_GATE") != nullptr;
    if (fireGate && (sb & 0x2000) && !(sAimStick.mask & 0x2000) && !T2Port_FireGate(sFrame + 1)) {
        sb &= (uint16_t)~0x2000u;
    }
    return sb;
}

// T2_PAD_LOG=1 (debug, round 14 c4): port 1's pad as the game receives it, one line per change
// ("pad: frame F buttons XXXX stick X Y"); =2: every read.
void PadLog(uint16_t b, int8_t sx, int8_t sy) {
    static const int level = getenv("T2_PAD_LOG") != nullptr ? atoi(getenv("T2_PAD_LOG")) : 0;
    static uint32_t last = 0xFFFFFFFFu;
    const uint32_t cur = (uint32_t)b << 16 | (uint32_t)(uint8_t)sx << 8 | (uint8_t)sy;
    if (level > 0 && (cur != last || level > 1)) {
        printf("pad: frame %d buttons %04X stick %d %d\n", sFrame + 1, b, sx, sy);
        last = cur;
    }
}

// LUS's OSContPad carries gyro and right-stick fields (0x24 bytes); the game's is the N64's
// 6-byte record.
void ContRead(void* pads) {
    OSContPad lus[MAXCONTROLLERS] = {};
    T2PadTest_Apply(sFrame + 1); // --pad-test (debug): this frame's scripted device events
    T2Controls_PollHotplug();     // gamepads plugged in or out
    osContStartReadData(&sContQueue);
    osContGetReadData(lus);
    // round 19 (h4): no input while the window does not have the focus (the pads stay connected)
    const bool focus = T2Controls_InputFocus();
    if (!focus) {
        for (int i = 0; i < MAXCONTROLLERS; i++) {
            lus[i].button = 0;
            lus[i].stick_x = 0;
            lus[i].stick_y = 0;
        }
    }
    auto* out = static_cast<uint8_t*>(pads);
    for (int i = 0; i < 4 && i < MAXCONTROLLERS; i++) {
        uint16_t b = (uint16_t)lus[i].button;
        if (i == 0) {
            b |= ScriptedButtonsGated();
        }
        // N64 byte order is not an issue here: OSContPad.button is read as a u16 by the game
        memcpy(out + i * 6, &b, 2);
        int8_t sx = (int8_t)lus[i].stick_x, sy = (int8_t)lus[i].stick_y;
        if (i == 0) {
            T2Controls_Hotkeys();                             // F1: the controls
            T2Controls_Look(&sy, T2Port_EngineMode() == 11); // gT2StickInvertY
            if (focus) {
                T2Controls_Mouse(&b, &sx, &sy); // captured mouse: look, fire, jump
            }
            memcpy(out, &b, 2);
            ScriptedStick(&sx, &sy);
            if (sAimStick.valid) { // T2_AIM_FIFO
                sx = sAimStick.x;
                sy = sAimStick.y;
            }
            if (sAimStick.mask != 0) { // T2_AIM_FIFO: the driver's C / Z / R buttons
                b = AimFifoApply(b);
                memcpy(out, &b, 2);
            }
            PadLog(b, sx, sy);
        }
        out[i * 6 + 2] = (uint8_t)sx;
        out[i * 6 + 3] = (uint8_t)sy;
        out[i * 6 + 4] = lus[i].err_no;
    }
}

// Without a window (--no-window: macOS has no headless GL/Metal, MACOS.md) LUS's ControlDeck
// has no device, so the game would see no controller and no pak. Headless runs get one virtual
// pad on port 1 instead: connected, its pak per --pak-*, buttons from --input only.
int ContInitHeadless(uint8_t* bits) {
    *bits = 1;
    return 0;
}

void ContReadHeadless(void* pads) {
    auto* out = static_cast<uint8_t*>(pads);
    uint16_t b = ScriptedButtonsGated();
    int8_t sx = 0, sy = 0;
    ScriptedStick(&sx, &sy);
    if (sAimStick.valid) { // T2_AIM_FIFO
        sx = sAimStick.x;
        sy = sAimStick.y;
    }
    b = AimFifoApply(b); // T2_AIM_FIFO: the driver's C / Z / R buttons
    PadLog(b, sx, sy);
    memcpy(out, &b, 2);
    out[2] = (uint8_t)sx;
    out[3] = (uint8_t)sy;
    out[4] = 0;
}

// The interpreter that draws: the window's, or --offscreen-gl's (macOS, no window).
std::shared_ptr<Fast::Interpreter> DrawInterpreter(const std::shared_ptr<Fast::Fast3dWindow>& window) {
    return window ? window->GetInterpreterWeak().lock() : T2Offscreen_Interpreter();
}

// Reads back the last frame Fast3D drew and writes it as a binary PPM (T2_SHOT): the window's
// framebuffer 0, or with --offscreen-gl the interpreter's game framebuffer.
void WriteShot(const std::shared_ptr<Fast::Fast3dWindow>& window, const char* shot) {
    auto interp = DrawInterpreter(window);
    if (!interp || !interp->mRapi) {
        return;
    }
    const uint32_t w = interp->mCurDimensions.width, h = interp->mCurDimensions.height;
    // 8 bits per channel (round 17 f4: T2Gl_ReadRGB8); T2_SHOT_RGB16=1 (debug) keeps the old
    // RGBA5551 readback (the low 3 bits of each channel dropped, as in shots before round 17)
    static const bool rgb16 = getenv("T2_SHOT_RGB16") != nullptr;
    std::vector<uint8_t> rgb8((size_t)w * h * 3);
    if (!rgb16 && !window && T2Offscreen_MetalRGB8(w, h, rgb8.data())) { // --offscreen-metal: top row first
        if (FILE* fp = fopen(shot, "wb")) {
            fprintf(fp, "P6\n%u %u\n255\n", w, h);
            fwrite(rgb8.data(), 1, rgb8.size(), fp);
            fclose(fp);
            printf("game: wrote %s (%ux%u, Metal)\n", shot, w, h);
        }
        return;
    }
    if (!rgb16 && T2Gl_ReadRGB8(interp, window ? 0 : T2Offscreen_ShotFramebuffer(), w, h, rgb8.data())) {
        if (FILE* fp = fopen(shot, "wb")) {
            fprintf(fp, "P6\n%u %u\n255\n", w, h);
            for (uint32_t y = 0; y < h; y++) {
                // the same row order as below: framebuffer 0 bottom-up, the game framebuffer flipped
                fwrite(&rgb8[(size_t)(window ? h - 1 - y : y) * w * 3], 1, (size_t)w * 3, fp);
            }
            fclose(fp);
            printf("game: wrote %s (%ux%u)\n", shot, w, h);
        }
        return;
    }
    std::vector<uint16_t> fb((size_t)w * h);
    interp->mRapi->ReadFramebufferToCPU(window ? 0 : T2Offscreen_ShotFramebuffer(), w, h, fb.data());
    if (FILE* fp = fopen(shot, "wb")) {
        fprintf(fp, "P6\n%u %u\n255\n", w, h);
        for (uint32_t y = 0; y < h; y++) {
            for (uint32_t x = 0; x < w; x++) {
                // framebuffer 0 is bottom-up; the game framebuffer is stored flipped (invertY)
                uint16_t c = fb[(size_t)(window ? h - 1 - y : y) * w + x];
                uint8_t px[3] = { (uint8_t)((c >> 11 & 31) << 3), (uint8_t)((c >> 6 & 31) << 3),
                                  (uint8_t)((c >> 1 & 31) << 3) };
                fwrite(px, 1, 3, fp);
            }
        }
        fclose(fp);
        printf("game: wrote %s (%ux%u)\n", shot, w, h);
    }
}

// ---- --bench / --pace (DEBUG-ONLY measurement, docs/port/PERF.md) ----

using BenchClock = std::chrono::steady_clock;

static double Ms(BenchClock::duration d) {
    return std::chrono::duration<double, std::milli>(d).count();
}

// Prints count, mean, p50/p95/p99 and max of one series (milliseconds).
static void BenchLine(const char* name, std::vector<double> v, const char* unit = "ms") {
    if (v.empty()) {
        return;
    }
    double sum = 0.0;
    for (double x : v) {
        sum += x;
    }
    std::sort(v.begin(), v.end());
    auto pct = [&](double p) { return v[std::min(v.size() - 1, (size_t)(p * (double)(v.size() - 1) + 0.5))]; };
    printf("bench:   %-22s n %6zu  mean %8.3f  p50 %8.3f  p95 %8.3f  p99 %8.3f  max %8.3f %s\n", name, v.size(),
           sum / (double)v.size(), pct(0.50), pct(0.95), pct(0.99), v.back(), unit);
}

// The cost of one t2os thread switch, measured with the same primitive t2os uses (one mutex,
// a condition variable per thread, the baton handed over with pthread_cond_signal and awaited
// with pthread_cond_wait): two host threads pass a baton n times. Returns ns per handoff.
static double BenchSwitchNs(int n) {
    struct Shared {
        pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t cv[2] = { PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER };
        int owner = 0;
        int left = 0;
    } sh;
    sh.left = n;
    auto run = [&sh](int me) {
        pthread_mutex_lock(&sh.lock);
        for (;;) {
            while (sh.owner != me && sh.left > 0) {
                pthread_cond_wait(&sh.cv[me], &sh.lock);
            }
            if (sh.left <= 0) {
                break;
            }
            sh.left--;
            sh.owner = 1 - me;
            pthread_cond_signal(&sh.cv[1 - me]);
        }
        pthread_cond_signal(&sh.cv[1 - me]);
        pthread_mutex_unlock(&sh.lock);
    };
    auto t0 = BenchClock::now();
    std::thread a(run, 0), b(run, 1);
    a.join();
    b.join();
    return std::chrono::duration<double, std::nano>(BenchClock::now() - t0).count() / (double)n;
}

} // namespace

extern "C" {
int T2Sched_TaskEndsFrame(const void* ostask); // port/game/t2sched.c
int Sound__Play2D(int id);
void Music_SetTrack(int track);
void Sound__PlaySpeech(int group);
void T2Audio_TestVolumes(void);
}

int T2Game_Run(const std::shared_ptr<Fast::Fast3dWindow>& window, const T2GameArgs& args) {
    // Debug only (audio bring-up, docs/port/AUDIO.md section 9): T2_TEST_SFX=<id> plays the
    // game's sound <id> through Sound__Play2D at frame 30, T2_TEST_SONG=<n> starts music
    // track <n> through Music_SetTrack at frame 60 (both between frames, when every game thread
    // is idle). Needs --audio.
    const char* testSfx = getenv("T2_TEST_SFX");
    const char* testSong = getenv("T2_TEST_SONG");
    const char* testSpeech = getenv("T2_TEST_SPEECH"); // Sound__PlaySpeech(<group>) at frame 90
    if (window || T2PadTest_Active()) {
        static const T2PortContHooks hooks = { ContInit, ContRead };
        T2Port_SetContHooks(&hooks);
    } else {
        static const T2PortContHooks headless = { ContInitHeadless, ContReadHeadless };
        T2Port_SetContHooks(&headless);
    }
    T2PortConfig cfg = {};
    cfg.romPath = args.rom.empty() ? nullptr : args.rom.c_str();
    cfg.lowMemory = args.lowMemory;
    cfg.verbose = args.verbose;
    // round 17 merge: the sound world runs in every game run with the ROM (its requests draw random
    // numbers as on the N64; the samples are dropped without --audio/--wav), unless T2_LEGACY_NOSOUND=1
    // (DEBUG-ONLY: the silent stream of rounds 1-16, port/game/t2debug.c)
    cfg.audio = args.audio || (!args.rom.empty() && !T2Port_LegacyNoSound());
    cfg.pakMode = args.pakMode;
    cfg.pakDir = args.pakDir.empty() ? nullptr : args.pakDir.c_str();
    if (!args.input.empty() && !ParseInputScript(args.input)) {
        fprintf(stderr, "--input: expected BUTTON@FRAME[..LAST], SX=V@FRAME[..LAST], SY=V@..., or @FILE "
                        "(buttons: A B Z START DU DD DL DR L R CU CD CL CR)\n");
        return 1;
    }
    if (T2Port_Boot(&cfg) != 0) {
        return 1;
    }
    printf("game: booted, %d frames\n", args.frames);
    fflush(stdout);

    // --wav: the mixed output as a 16-bit stereo WAV (header patched at the end), with a summary
    // (peak, RMS, non-silent frames) so headless runs can check the sound is there and sane
    FILE* wav = nullptr;
    uint64_t wavFrames = 0, wavNonZero = 0;
    double wavSumSq = 0.0;
    int wavPeak = 0;
    if (!args.wav.empty()) {
        wav = fopen(args.wav.c_str(), "wb");
        if (wav == nullptr) {
            fprintf(stderr, "game: cannot write %s\n", args.wav.c_str());
        } else {
            static const uint8_t zero[44] = {};
            fwrite(zero, 1, sizeof zero, wav);
        }
    }

    int drawn = 0, skippedBad = 0;
    const void* lastDL = nullptr;
    int mode = -1;
    // T2_SHOT_EVERY=<n> (with T2_SHOT=<file.ppm>): also write <file>.<frame>.ppm every n frames
    const char* shotEnv = getenv("T2_SHOT_EVERY");
    const int shotEvery = shotEnv != nullptr ? atoi(shotEnv) : 0;
    int nextShot = shotEvery;
    // T2_SHOT_FROM=<frame> (debug, round 7 v2): the periodic shots start at that frame
    if (const char* from = getenv("T2_SHOT_FROM"); from != nullptr && shotEvery > 0 && atoi(from) > nextShot) {
        nextShot = atoi(from);
    }
    const char* dumpAtEnv = getenv("T2_DLDUMP_AT");
    const int dumpAt = dumpAtEnv != nullptr ? atoi(dumpAtEnv) : 0;
    bool dumped = false;
    // --bench: per VI, the host time of the game (T2Port_Frame: the game threads and the t2os
    // switches, minus the audio manager), the audio manager (synthesizer + ASP), the draw (dlcheck,
    // Fast3D's interpreter and, offscreen, glFinish), the whole iteration; per drawn game frame,
    // the time since the previous drawn frame (the cost of one displayed frame).
    const bool bench = args.benchFrom >= 0;
    std::vector<double> bGame, bAudio, bDraw, bTotal, bGameFrame, bSwitches;
    double bSumSynth = 0.0, bSumAsp = 0.0;
    auto bLastDrawn = BenchClock::time_point{};
    // round 17 f4: the game's cadence (VIs from one drawn frame to the next: 2 = 30 fps), the
    // worst VIs by host time, and T2_BENCH_CSV=<file> (per VI: VI, game, audio, draw, total ms,
    // drawn) for offline histograms
    int bLastDrawnVI = -1;
    std::vector<int> bCadence(8, 0); // [1..6] VIs, [7] 7 or more
    std::vector<std::pair<double, int>> bWorst; // (total ms, VI)
    FILE* bCsv = nullptr;
    if (const char* csv = getenv("T2_BENCH_CSV"); bench && csv != nullptr) {
        bCsv = fopen(csv, "w");
        if (bCsv) {
            fprintf(bCsv, "vi,game_ms,audio_ms,draw_ms,total_ms,drawn\n");
        }
    }
    // --pace: one VI every 1/60 s of real time (absolute deadlines, no catch-up: a late VI moves
    // the schedule)
    const auto viPeriod = std::chrono::duration_cast<BenchClock::duration>(std::chrono::duration<double>(1.0 / 60.0));
    auto paceNext = BenchClock::now() + viPeriod;
    int paceLate = 0;
    std::vector<std::pair<int, double>> paceLateList; // (VI, ms late), the worst printed at the end
    double paceWorstLateMs = 0.0, paceSleptMs = 0.0;
    // --pace --audio (round 10 y4): a simulated 22,050 Hz sound device fed by the audio manager,
    // to count underruns without one (headless runs have no audio backend): it starts with
    // kSinkPrefill frames queued at the first audio frame, then drains in real time; a VI that
    // finds it empty is an underrun (the sink then restarts with the prefill).
    constexpr double kSinkRate = 22050.0, kSinkPrefill = 2048.0;
    bool sinkOn = false;
    auto sinkStart = BenchClock::time_point{};
    double sinkQueued = 0.0, sinkMin = 1e9, sinkMax = 0.0, sinkProduced = 0.0;
    int sinkUnder = 0;
    int devMin = 1 << 30, devMax = 0, devSlow = 0, devFast = 0; // the real device's buffer (--pace)
    std::vector<int> sinkUnderVIs;
    auto t0 = std::chrono::steady_clock::now();
    for (int f = 0; f < args.frames && (!window || window->IsRunning()); f++) {
        const auto bt0 = BenchClock::now();
        uint64_t bSynth0 = 0, bAsp0 = 0;
        T2PortStats bS0 = {};
        if (bench) {
            T2Audio_GetTimes(&bSynth0, &bAsp0);
            T2Port_GetStats(&bS0);
        }
        BenchClock::time_point bt1 = bt0, bt2 = bt0, bt3 = bt0;
        if (window) {
            window->HandleEvents();
        }
        // T2_ANCHOR (round 12 a3): the scripted input and the frame-scheduled debug switches run
        // on the schedule clock (the host frame, re-anchored when a listed level is reached)
        sFrame = T2Port_ScheduleFrame(f + 1) - 1;
        if (args.audio && (testSfx != nullptr || testSong != nullptr || testSpeech != nullptr) && f == 29) {
            T2Audio_TestVolumes();
        }
        if (args.audio && testSfx != nullptr && f == 30) {
            int h = Sound__Play2D((int)strtol(testSfx, nullptr, 0));
            printf("game: T2_TEST_SFX: Sound__Play2D(%s) -> handle %d\n", testSfx, h);
        }
        if (args.audio && testSpeech != nullptr && f == 90) {
            Sound__PlaySpeech((int)strtol(testSpeech, nullptr, 0));
            printf("game: T2_TEST_SPEECH: Sound__PlaySpeech(%s)\n", testSpeech);
        }
        if (args.audio && testSong != nullptr && f == 60) {
            Music_SetTrack((int)strtol(testSong, nullptr, 0));
            printf("game: T2_TEST_SONG: Music_SetTrack(%s)\n", testSong);
        }
        // DEBUG-ONLY: T2_SCRIBBLE=HEX (round 14 c5): fill the stack the game frame will use with
        // HEX, so a local read before it is written shows up (STABILITY.md section 10)
        static const char* scribble = getenv("T2_SCRIBBLE");
        if (scribble != nullptr) {
            T2Port_ScribbleStack((unsigned)strtoul(scribble, nullptr, 0));
        }
        T2Port_Frame();
        bt1 = BenchClock::now();
        static const bool menuLog = getenv("T2_MENU_LOG") != nullptr;
        static const void* lastMenu = nullptr;
        static int lastCursor = -1;
        int cursor;
        if (const void* menu = T2Game_TopMenu(&cursor); menuLog && (menu != lastMenu || cursor != lastCursor)) {
            printf("game: frame %d: menu desc %p cursor %d\n", f, menu, cursor);
            lastMenu = menu;
            lastCursor = cursor;
        }
        // T2_STATE_LOG=1: every 60 frames; =N (N > 1): every N frames
        static const int statePeriod = getenv("T2_STATE_LOG") == nullptr ? 0
                                       : (atoi(getenv("T2_STATE_LOG")) > 1 ? atoi(getenv("T2_STATE_LOG")) : 60);
        gT2HostFrame = f + 1;
        T2Port_GiveWeapons(sFrame + 1); // DEBUG-ONLY: T2_GIVE_WEAPONS=1
        if (statePeriod > 0 && (f + 1) % statePeriod == 0) {
            T2Port_LogState(f + 1); // DEBUG-ONLY: T2_STATE_LOG=1
        }
        // T2_DET_LOG=N: the determinism trace every N frames (exact bits; T2_DET_FROM, T2_DET_ACTORS)
        static const int detPeriod = getenv("T2_DET_LOG") == nullptr ? 0
                                     : (atoi(getenv("T2_DET_LOG")) > 0 ? atoi(getenv("T2_DET_LOG")) : 1);
        if (detPeriod > 0 && (f + 1) % detPeriod == 0) {
            T2Port_LogDet(f + 1); // DEBUG-ONLY: T2_DET_LOG=N
        }
        static const char* teleport = getenv("T2_TELEPORT");
        if (teleport != nullptr) {
            T2Port_Teleport(sFrame + 1, teleport); // DEBUG-ONLY: T2_TELEPORT=F:x,y,z[;F:x,y,z...]
        }
        static const char* wake = getenv("T2_WAKE_ENEMIES");
        if (wake != nullptr) {
            T2Port_WakeEnemies(sFrame + 1, wake); // DEBUG-ONLY: T2_WAKE_ENEMIES=R
        }
        static const char* faceNearest = getenv("T2_FACE_NEAREST");
        if (faceNearest != nullptr) {
            T2Port_FaceNearest(sFrame + 1, faceNearest); // DEBUG-ONLY: T2_FACE_NEAREST=R
        }
        static const bool bossTrace = getenv("T2_BOSS_LOG") != nullptr;
        if (bossTrace) {
            T2Port_TraceBoss(f + 1); // DEBUG-ONLY: T2_BOSS_LOG=1, the boss mode/phase changes
        }
        static const char* aimNode = getenv("T2_AIM_NODE");
        if (aimNode != nullptr) {
            T2Port_AimNode(sFrame + 1, aimNode); // DEBUG-ONLY: T2_AIM_NODE=MASK[,R]
        }
        static const char* bossFront = getenv("T2_BOSS_FRONT");
        if (bossFront != nullptr) {
            T2Port_BossFront(sFrame + 1, bossFront); // DEBUG-ONLY: T2_BOSS_FRONT=D[,EVERY]
        }
        static const char* lairPeek = getenv("T2_LAIR_PEEK");
        if (lairPeek != nullptr) {
            T2Port_LairPeek(sFrame + 1, lairPeek); // DEBUG-ONLY: T2_LAIR_PEEK=W (round 15 C30)
        }
        static const char* startWarp = getenv("T2_START_WARP");
        if (startWarp != nullptr) {
            T2Port_StartWarp(sFrame + 1, startWarp); // DEBUG-ONLY: T2_START_WARP=ID@F (round 16 e1)
        }
        T2Port_LegacyShakeTick(sFrame + 1); // DEBUG-ONLY: T2_LEGACY_SHAKE=1,F (round 17 f2)
        static const char* rngSet = getenv("T2_RNG_SET");
        if (rngSet != nullptr) {
            T2Port_RngSet(sFrame + 1, rngSet); // DEBUG-ONLY: T2_RNG_SET=LEVEL:GF:HEX (round 16 e1)
        }
        // after T2_RNG_SET: the dump shows the seed the next frame starts from, as n64trace.py --raw does
        static const char* rawDump = getenv("T2_RAW_DUMP");
        if (rawDump != nullptr) {
            T2Port_RawDump(rawDump); // DEBUG-ONLY: T2_RAW_DUMP=FILE:LEVEL:GF[-GF2][,...] (round 17 f3)
        }
        static const char* primagenStage = getenv("T2_PRIMAGEN_STAGE");
        if (primagenStage != nullptr) {
            T2Port_PrimagenStage(sFrame + 1, primagenStage); // DEBUG-ONLY: T2_PRIMAGEN_STAGE=F[,F] (round 16 e1)
        }
        static const char* aimAt = getenv("T2_AIM_AT");
        if (aimAt != nullptr) {
            T2Port_AimAt(sFrame + 1, aimAt); // DEBUG-ONLY: T2_AIM_AT=F1-F2:x,y,z[;...]
        }
        static const char* aimEnemy = getenv("T2_AIM_ENEMY");
        if (aimEnemy != nullptr) {
            T2Port_AimEnemy(sFrame + 1, aimEnemy); // DEBUG-ONLY: T2_AIM_ENEMY=F1-F2[,R][;...]
        }
        static const char* aimFifo = getenv("T2_AIM_FIFO");
        if (aimFifo != nullptr) {
            AimFifoExchange(sFrame + 1, aimFifo); // DEBUG-ONLY: T2_AIM_STICK's stick driver
        }
        static const char* cheats = getenv("T2_CHEATS");
        static const char* fillAmmo = getenv("T2_FILL_AMMO");
        if (cheats != nullptr || fillAmmo != nullptr) {
            T2Port_Cheats(sFrame + 1, cheats, fillAmmo); // DEBUG-ONLY: T2_CHEATS=MASK, T2_FILL_AMMO=N
        }
        static const char* hurtLog = getenv("T2_HURT_LOG");
        if (hurtLog != nullptr) {
            T2Port_HurtLog(f + 1, sFrame + 1, hurtLog); // DEBUG-ONLY: T2_HURT_LOG=1|2[,R]
        }
        static const char* giveItems = getenv("T2_GIVE_ITEMS");
        if (giveItems != nullptr) {
            T2Port_GiveItems(sFrame + 1, giveItems); // DEBUG-ONLY: T2_GIVE_ITEMS=ID[:N][,...]
        }
        static const char* doorOpen = getenv("T2_DOOR_OPEN");
        if (doorOpen != nullptr) {
            T2Port_DoorOpen(sFrame + 1, doorOpen); // DEBUG-ONLY: T2_DOOR_OPEN=F
        }
        static const char* bitsLog = getenv("T2_BITS_LOG");
        if (bitsLog != nullptr) {
            T2Port_BitsLog(f + 1, bitsLog); // DEBUG-ONLY: T2_BITS_LOG=L1,L2,...
        }
        static const char* dumpActor = getenv("T2_DUMP_ACTOR");
        if (dumpActor != nullptr) {
            T2Port_DumpActor(f + 1, dumpActor); // DEBUG-ONLY: T2_DUMP_ACTOR=F:I[;...]
        }
        static const char* savePortal = getenv("T2_SAVE_PORTAL");
        if (savePortal != nullptr) {
            T2Port_SavePortal(sFrame + 1, savePortal); // DEBUG-ONLY: T2_SAVE_PORTAL=F
        }
        if (int m = T2Port_EngineMode(); m != mode) {
            printf("game: frame %d: engine mode %d -> %d\n", f + 1, mode, m);
            fflush(stdout);
            mode = m;
        }
        T2Port_DebugWarp(sFrame + 1); // DEBUG-ONLY: T2_WARP_TO=<id>[@frame][,...]
        T2Port_ProgressLog(f + 1);    // DEBUG-ONLY: T2_PROGRESS_LOG=1
        T2Port_SetTotems();           // DEBUG-ONLY: T2_TOTEMS=abcde
        if (args.skipPakCheck || args.startMode >= 0) {
            T2Port_DebugSwitches(args.skipPakCheck, args.startMode); // DEBUG-ONLY
        }
        if (args.audio) {
            // the frames the audio manager produced this retrace (22050 Hz stereo s16) to LUS's
            // AudioPlayer (a no-op when it has no audio backend, e.g. headless)
            static int16_t pcm[4096 * 2];
            if (args.audioDeviceChangeTest > 0 && f + 1 == args.audioDeviceChangeTest) {
                T2AudioDev_SimulateChange(); // --audio-device-change-test (round 19 h4)
            }
            T2AudioDev_Poll(); // the default output device changed: a new player on it (macOS)
            size_t n = T2Audio_TakeSamples(pcm, 4096);
            if (n > 0) {
                // T2_AUDIO_SILENT=1 (debug, round 14 c4): the device gets silence (tests of the
                // device path that should not be heard); --wav still gets the sound
                static const bool silent = getenv("T2_AUDIO_SILENT") != nullptr;
                static int16_t zeros[4096 * 2];
                AudioPlayerPlayFrame(reinterpret_cast<const uint8_t*>(silent ? zeros : pcm), n * 4);
            }
            if (args.pace && (n > 0 || sinkOn)) {
                const auto now = BenchClock::now();
                if (!sinkOn) {
                    sinkOn = true;
                    sinkStart = now;
                    sinkQueued = kSinkPrefill;
                }
                const double drained = std::chrono::duration<double>(now - sinkStart).count() * kSinkRate;
                double level = sinkQueued - drained;
                if (level < 0.0) {
                    sinkUnder++;
                    if (sinkUnderVIs.size() < 12) {
                        sinkUnderVIs.push_back(f + 1);
                    }
                    sinkStart = now;
                    sinkQueued = kSinkPrefill;
                    level = kSinkPrefill;
                }
                sinkMin = std::min(sinkMin, level);
                sinkQueued += (double)n;
                sinkProduced += (double)n;
                sinkMax = std::max(sinkMax, level + (double)n);
            }
            if (wav != nullptr && n > 0) {
                fwrite(pcm, 4, n, wav);
                for (size_t k = 0; k < n * 2; k++) {
                    int v = pcm[k] < 0 ? -pcm[k] : pcm[k];
                    wavPeak = v > wavPeak ? v : wavPeak;
                    wavSumSq += (double)pcm[k] * pcm[k];
                    wavNonZero += pcm[k] != 0;
                }
                wavFrames += n;
            }
        }
        T2PortTask task;
        bt2 = BenchClock::now();
        bool drewThis = false;
        const bool hadTask = T2Port_TakeGfxTask(&task);
        const bool got = hadTask && args.draw && task.dataPtr != nullptr;
        if (got) {
            lastDL = task.dataPtr;
            // T2_DLDUMP_AT=<frame>: dump the first display list from that frame on, now, before
            // the game runs again (no window needed: macOS has no headless GL, MACOS.md)
            if (dumpAt > 0 && f + 1 >= dumpAt && !dumped) {
                dumped = true;
                int budget = getenv("T2_DLDUMP_MAX") != nullptr ? atoi(getenv("T2_DLDUMP_MAX")) : 4000;
                printf("dl: frame %d\n", f + 1);
                DumpDL((const uintptr_t*)lastDL, 0, &budget);
            }
        }
        // Round 20 i1: a gfx task that is not the last of its frame (the automap is on: its line
        // list follows as a second task into the same framebuffer, CEngineApp__SubmitGfxTask) is
        // kept and drawn with the next one as a single frame. Drawn alone, each was a frame of its
        // own: the map's frame black with nothing but the map, every other frame (flicker).
        static const void* sHeldDL = nullptr;
        static uintptr_t sJoinDL[6];
        const void* drawDL = got ? task.dataPtr : nullptr;
        if (got && task.type == 1 && !T2Sched_TaskEndsFrame(task.task)) {
            sHeldDL = task.dataPtr;
            drawDL = nullptr;
        } else if (got && sHeldDL != nullptr) {
            sJoinDL[0] = 0xDE000000u; // gSPDisplayList: the frame's main list
            sJoinDL[1] = (uintptr_t)sHeldDL;
            sJoinDL[2] = 0xDE000000u; // then the line list (it sets its own modes and matrices)
            sJoinDL[3] = (uintptr_t)task.dataPtr;
            sJoinDL[4] = 0xDF000000u; // gSPEndDisplayList
            sJoinDL[5] = 0;
            drawDL = sJoinDL;
            sHeldDL = nullptr;
        }
        // T2_DRAW_FROM=F or A-B[;A-B...] (DEBUG-ONLY, round 12 C27): with --draw, draw (and shoot) from
        // frame F on, or in those frame ranges, only: pictures of a long scripted run without drawing
        // all of it (offscreen drawing is ~40 frames a second on the box; the game runs the same)
        static const char* drawFrom = getenv("T2_DRAW_FROM");
        bool drawNow = drawFrom == nullptr;
        for (const char* d = drawFrom; d != nullptr && *d != 0 && !drawNow;) {
            const int a = atoi(d);
            const char* dash = strchr(d, '-');
            const char* semi = strchr(d, ';');
            const bool range = dash != nullptr && (semi == nullptr || dash < semi);
            drawNow = range ? (f + 1 >= a && f + 1 <= atoi(dash + 1)) : f + 1 >= a;
            d = semi != nullptr ? semi + 1 : nullptr;
        }
        if (drawDL != nullptr && drawNow && (window || T2Offscreen_Interpreter())) {
            // Only with --draw: the game's display lists are not host-correct yet (GBI words
            // hold 32-bit addresses; docs/port/BUILD.md "GBI").
            // Fast3D maps RDP coordinates from its native size (320x240 by default) to the
            // window: the game draws at the size Video_SetMode chose (gScreenWidth x
            // gScreenHeight, 480x360 in mode 0), so tell it before every frame.
            if (auto interp = DrawInterpreter(window);
                interp && gScreenWidth > 0 && gScreenHeight > 0 &&
                (interp->mNativeDimensions.width != (uint32_t)gScreenWidth ||
                 interp->mNativeDimensions.height != (uint32_t)gScreenHeight)) {
                interp->SetNativeDimensions((float)gScreenWidth, (float)gScreenHeight);
            }
            static const bool dlCheck = !window || getenv("T2_DLCHECK") != nullptr;
            int badWords = 0;
            if (dlCheck) {
                int budget = 200000;
                badWords = CheckDL((const uintptr_t*)drawDL, 0, &budget, f + 1);
                if (badWords > 0) {
                    skippedBad++; // Fast3D would dereference a truncated pointer: not drawn
                }
            }
            if (badWords == 0 && window) {
                T2Display_UpdateWindow(window, args.renderScale); // retina: the frame at pixel resolution
            }
            if (badWords == 0 &&
                (window ? window->DrawAndRunGraphicsCommands((Gfx*)drawDL, {}) : T2Offscreen_Draw(const_cast<void*>(drawDL)))) {
                drawn++;
                drewThis = true;
                if (bench && !window) {
                    T2Offscreen_Finish(); // the GPU's part of the frame too
                }
                if (const char* shot = getenv("T2_SHOT"); shot != nullptr && shotEvery > 0 && f + 1 >= nextShot) {
                    nextShot = (f + 1) / shotEvery * shotEvery + shotEvery; // the first drawn frame of each period
                    std::string name = std::string(shot) + "." + std::to_string(f + 1) + ".ppm";
                    WriteShot(window, name.c_str());
                }
            }
        }
        bt3 = BenchClock::now();
        if (bench && f >= args.benchFrom) {
            uint64_t synth1, asp1;
            T2Audio_GetTimes(&synth1, &asp1);
            T2PortStats bS1;
            T2Port_GetStats(&bS1);
            const double audioMs = (double)((synth1 - bSynth0) + (asp1 - bAsp0)) / 1e6;
            bSumSynth += (double)(synth1 - bSynth0) / 1e6;
            bSumAsp += (double)(asp1 - bAsp0) / 1e6;
            bGame.push_back(Ms(bt1 - bt0) - audioMs);
            bAudio.push_back(audioMs);
            bDraw.push_back(Ms(bt3 - bt2));
            bTotal.push_back(Ms(bt3 - bt0));
            bSwitches.push_back((double)(bS1.switches - bS0.switches));
            const bool drawnVI = drewThis || (hadTask && !(got && (window || T2Offscreen_Interpreter())));
            if (drawnVI) {
                if (bLastDrawn != BenchClock::time_point{}) {
                    bGameFrame.push_back(Ms(bt3 - bLastDrawn));
                }
                bLastDrawn = bt3;
                if (bLastDrawnVI >= 0) {
                    bCadence[std::min(f - bLastDrawnVI, 7)]++;
                }
                bLastDrawnVI = f;
            }
            bWorst.emplace_back(bTotal.back(), f + 1);
            if (bWorst.size() >= 4096) { // keep the 16 worst
                std::partial_sort(bWorst.begin(), bWorst.begin() + 16, bWorst.end(), std::greater<>());
                bWorst.resize(16);
            }
            if (bCsv) {
                fprintf(bCsv, "%d,%.4f,%.4f,%.4f,%.4f,%d\n", f + 1, bGame.back(), bAudio.back(), bDraw.back(),
                        bTotal.back(), drawnVI ? 1 : 0);
            }
        }
        if (args.verbose || (f + 1) % 60 == 0) {
            T2PortStats s;
            T2Port_GetStats(&s);
            printf("  frame %d: switches %llu, gfx tasks %llu, audio tasks %llu, PI DMAs %llu (%llu bytes), "
                   "VI swaps %llu, missing calls %llu\n",
                   f + 1, (unsigned long long)s.switches, (unsigned long long)s.gfxTasks,
                   (unsigned long long)s.audioTasks, (unsigned long long)s.piDmas, (unsigned long long)s.piBytes,
                   (unsigned long long)s.viSwaps, (unsigned long long)s.missingCalls);
            fflush(stdout);
        }
        if (args.pace) {
            auto now = BenchClock::now();
            if (now < paceNext) {
                paceSleptMs += Ms(paceNext - now);
                std::this_thread::sleep_until(paceNext);
                paceNext += viPeriod;
                if (args.audioDevice && sinkOn) {
                    // a real sound device (round 14 c4): the VI clock follows its buffer, so the
                    // host's 60 Hz and the device's 22,050 Hz cannot drift apart (a long session
                    // would overflow or starve LUS's 6,000-frame ring). At most +-1 ms per VI.
                    const int buffered = AudioPlayerBuffered();
                    const int desired = std::max(AudioPlayerGetDesiredBuffered(), 1470);
                    if (f >= 600) { // after the start-up (the boot's loads)
                        devMin = std::min(devMin, buffered);
                        devMax = std::max(devMax, buffered);
                    }
                    if (buffered > desired + 735) {
                        paceNext += std::chrono::milliseconds(1);
                        devSlow++;
                    } else if (buffered < desired - 735) {
                        paceNext -= std::chrono::milliseconds(1);
                        devFast++;
                    }
                }
            } else {
                paceLate++;
                paceWorstLateMs = std::max(paceWorstLateMs, Ms(now - paceNext));
                paceLateList.emplace_back(f + 1, Ms(now - paceNext));
                paceNext = now + viPeriod;
            }
        }
    }
    if (getenv("T2_DLDUMP") != nullptr && lastDL != nullptr) {
        int budget = 4000;
        DumpDL((const uintptr_t*)lastDL, 0, &budget);
    }
    // T2_SHOT=<file.ppm>: the last frame Fast3D drew, read back (bring-up screenshots).
    if (const char* shot = getenv("T2_SHOT"); shot != nullptr && drawn > 0) {
        WriteShot(window, shot);
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    if (wav != nullptr) {
        uint32_t rate = T2Audio_Rate() != 0 ? T2Audio_Rate() : 22050;
        uint32_t data = (uint32_t)(wavFrames * 4);
        uint8_t h[44];
        auto put32 = [&](int o, uint32_t v) { h[o] = v; h[o + 1] = v >> 8; h[o + 2] = v >> 16; h[o + 3] = v >> 24; };
        auto put16 = [&](int o, uint32_t v) { h[o] = v; h[o + 1] = v >> 8; };
        memcpy(h, "RIFF", 4); put32(4, 36 + data); memcpy(h + 8, "WAVEfmt ", 8); put32(16, 16); put16(20, 1);
        put16(22, 2); put32(24, rate); put32(28, rate * 4); put16(32, 4); put16(34, 16); memcpy(h + 36, "data", 4);
        put32(40, data);
        fseek(wav, 0, SEEK_SET);
        fwrite(h, 1, sizeof h, wav);
        fclose(wav);
        double rms = wavFrames ? sqrt(wavSumSq / (double)(wavFrames * 2)) : 0.0;
        printf("game: wav %s: %llu stereo frames at %u Hz (%.2f s), peak %d, rms %.1f, %llu non-zero samples\n",
               args.wav.c_str(), (unsigned long long)wavFrames, rate, wavFrames / (double)rate, wavPeak, rms,
               (unsigned long long)wavNonZero);
    }
    T2PortStats s;
    T2Port_GetStats(&s);
    printf("game: %llu frames in %.2f s, %llu thread switches, %llu gfx tasks (%d drawn), %llu audio tasks, "
           "%llu PI DMAs, %llu missing-function calls\n",
           (unsigned long long)s.frames, secs, (unsigned long long)s.switches, (unsigned long long)s.gfxTasks, drawn,
           (unsigned long long)s.audioTasks, (unsigned long long)s.piDmas, (unsigned long long)s.missingCalls);
    if (skippedBad > 0) {
        printf("game: %d display lists not drawn: truncated host addresses (dlcheck)\n", skippedBad);
    }
    if (bench) {
        // with --pace the totals still exclude the sleep (it comes after the iteration)
        printf("bench: VIs %d..%d (%zu measured), %s, %s\n", args.benchFrom, args.frames, bTotal.size(),
               args.draw ? (window ? "drawn in a window" : (T2Offscreen_Interpreter() ? "drawn offscreen (glFinish per frame)" : "display lists not drawn")) : "--draw off",
               args.audio ? "audio on" : "audio off");
        BenchLine("game (CPU, per VI)", bGame);
        BenchLine("audio (per VI)", bAudio);
        BenchLine("draw (per VI)", bDraw);
        BenchLine("total (per VI)", bTotal);
        BenchLine("game frame (drawn)", bGameFrame);
        BenchLine("t2os switches/VI", bSwitches, "");
        int cadN = 0;
        for (int c : bCadence) {
            cadN += c;
        }
        if (cadN > 0) {
            printf("bench: cadence (VIs between drawn frames, %d frames):", cadN);
            for (int k = 1; k < 8; k++) {
                printf(" %s%d: %d (%.2f%%)", k == 7 ? ">=" : "", k, bCadence[k], 100.0 * bCadence[k] / cadN);
            }
            printf("\n");
        }
        {
            static const double edges[] = { 0.5, 1.0, 2.0, 4.0, 8.0, 16.667, 33.333 };
            int hist[8] = {};
            for (double x : bTotal) {
                int k = 0;
                while (k < 7 && x >= edges[k]) {
                    k++;
                }
                hist[k]++;
            }
            printf("bench: host ms per VI histogram: <0.5 %d, 0.5-1 %d, 1-2 %d, 2-4 %d, 4-8 %d, 8-16.7 %d, 16.7-33.3 %d, >=33.3 %d\n",
                   hist[0], hist[1], hist[2], hist[3], hist[4], hist[5], hist[6], hist[7]);
            std::sort(bWorst.begin(), bWorst.end(), std::greater<>());
            printf("bench: the worst VIs (VI:ms):");
            for (size_t i = 0; i < bWorst.size() && i < 12; i++) {
                printf(" %d:%.2f", bWorst[i].second, bWorst[i].first);
            }
            printf("\n");
        }
        if (bCsv) {
            fclose(bCsv);
        }
        double sum = 0.0, sumF = 0.0;
        for (double x : bTotal) {
            sum += x;
        }
        for (double x : bGameFrame) {
            sumF += x;
        }
        if (!bTotal.empty()) {
            const double viMs = sum / (double)bTotal.size();
            printf("bench: mean %.3f ms per VI -> %.0f VI/s unpaced (%.1fx the N64's 60); synth %.3f + ASP %.3f ms per VI\n",
                   viMs, 1000.0 / viMs, 1000.0 / viMs / 60.0, bSumSynth / (double)bTotal.size(), bSumAsp / (double)bTotal.size());
        }
        if (!bGameFrame.empty()) {
            const double frMs = sumF / (double)bGameFrame.size();
            printf("bench: mean %.3f ms per drawn game frame -> equivalent %.0f fps (the N64 game: 30 fps, 33.3 ms)\n",
                   frMs, 1000.0 / frMs);
        }
        const double swNs = BenchSwitchNs(200000);
        double swSum = 0.0;
        for (double x : bSwitches) {
            swSum += x;
        }
        printf("bench: t2os switch (pthread cond handoff, 200000 x): %.0f ns -> %.3f ms per VI at %.1f switches/VI\n", swNs,
               bSwitches.empty() ? 0.0 : swNs * swSum / (double)bSwitches.size() / 1e6,
               bSwitches.empty() ? 0.0 : swSum / (double)bSwitches.size());
    }
    if (args.pace) {
        printf("pace: %d VIs at 60 Hz in %.2f s (ideal %.2f s), %d late (worst %.3f ms), slept %.1f%% of the time\n",
               args.frames, secs, args.frames / 60.0, paceLate, paceWorstLateMs, secs > 0 ? 100.0 * paceSleptMs / 1000.0 / secs : 0.0);
        std::sort(paceLateList.begin(), paceLateList.end(), [](auto& a, auto& b) { return a.second > b.second; });
        int lateMs = 0, lateBench = 0;
        for (auto& l : paceLateList) {
            lateMs += l.second >= 1.0;
            lateBench += bench && l.first > args.benchFrom;
        }
        printf("pace: %d late by 1 ms or more; %d late from VI %d on; the worst:", lateMs, lateBench, std::max(args.benchFrom, 0));
        for (size_t i = 0; i < paceLateList.size() && i < 12; i++) {
            printf(" %d:%.1f", paceLateList[i].first, paceLateList[i].second);
        }
        printf(" (VI:ms)\n");
        if (args.audio) {
            printf("pace: audio %.0f frames produced (%.1f Hz over the run), simulated %.0f Hz device with %.0f "
                   "prefilled: %d underruns, buffer min %.0f max %.0f frames",
                   sinkProduced, secs > 0 ? sinkProduced / secs : 0.0, kSinkRate, kSinkPrefill, sinkUnder,
                   sinkOn ? sinkMin : 0.0, sinkMax);
            for (int v : sinkUnderVIs) {
                printf(" %d", v);
            }
            printf("\n");
        }
        if (args.audioDevice) {
            printf("pace: sound device buffer from VI 600: min %d max %d frames; VI clock slowed %d, sped up %d times\n",
                   devMin == (1 << 30) ? 0 : devMin, devMax, devSlow, devFast);
        }
    }
    T2PifStats pif;
    T2Port_GetPifStats(&pif);
    printf("game: PIF %llu SI DMAs, %llu status, %llu pak reads, %llu pak writes\n", (unsigned long long)pif.dmas,
           (unsigned long long)pif.status, (unsigned long long)pif.reads, (unsigned long long)pif.writes);
    fflush(stdout);
    return 0;
}
