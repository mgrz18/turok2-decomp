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
void T2Port_GiveWeapons(void); // DEBUG-ONLY: T2_GIVE_WEAPONS=1 (t2debug.c)
void T2Port_DebugWarp(int frame);
void T2Port_Teleport(int frame, const char *spec);
void T2Port_WakeEnemies(int frame, const char *spec);
void T2Port_SavePortal(int frame, const char *spec);
void T2Port_DebugSwitches(int skipPak, int startMode);
}

#include "game.h"
#include "offscreen_gl.h"

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
    return osContInit(&sContQueue, bits, status);
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

// LUS's OSContPad carries gyro and right-stick fields (0x24 bytes); the game's is the N64's
// 6-byte record.
void ContRead(void* pads) {
    OSContPad lus[MAXCONTROLLERS] = {};
    osContStartReadData(&sContQueue);
    osContGetReadData(lus);
    auto* out = static_cast<uint8_t*>(pads);
    for (int i = 0; i < 4 && i < MAXCONTROLLERS; i++) {
        uint16_t b = (uint16_t)lus[i].button;
        if (i == 0) {
            b |= ScriptedButtons();
        }
        // N64 byte order is not an issue here: OSContPad.button is read as a u16 by the game
        memcpy(out + i * 6, &b, 2);
        int8_t sx = (int8_t)lus[i].stick_x, sy = (int8_t)lus[i].stick_y;
        if (i == 0) {
            ScriptedStick(&sx, &sy);
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
    uint16_t b = ScriptedButtons();
    int8_t sx = 0, sy = 0;
    ScriptedStick(&sx, &sy);
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
    if (window) {
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
    cfg.audio = args.audio;
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
    // --pace: one VI every 1/60 s of real time (absolute deadlines, no catch-up: a late VI moves
    // the schedule)
    const auto viPeriod = std::chrono::duration_cast<BenchClock::duration>(std::chrono::duration<double>(1.0 / 60.0));
    auto paceNext = BenchClock::now() + viPeriod;
    int paceLate = 0;
    std::vector<std::pair<int, double>> paceLateList; // (VI, ms late), the worst printed at the end
    double paceWorstLateMs = 0.0, paceSleptMs = 0.0;
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
        sFrame = f;
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
        T2Port_GiveWeapons(); // DEBUG-ONLY: T2_GIVE_WEAPONS=1
        if (statePeriod > 0 && (f + 1) % statePeriod == 0) {
            T2Port_LogState(f + 1); // DEBUG-ONLY: T2_STATE_LOG=1
        }
        static const char* teleport = getenv("T2_TELEPORT");
        if (teleport != nullptr) {
            T2Port_Teleport(f + 1, teleport); // DEBUG-ONLY: T2_TELEPORT=F:x,y,z[;F:x,y,z...]
        }
        static const char* wake = getenv("T2_WAKE_ENEMIES");
        if (wake != nullptr) {
            T2Port_WakeEnemies(f + 1, wake); // DEBUG-ONLY: T2_WAKE_ENEMIES=R
        }
        static const char* savePortal = getenv("T2_SAVE_PORTAL");
        if (savePortal != nullptr) {
            T2Port_SavePortal(f + 1, savePortal); // DEBUG-ONLY: T2_SAVE_PORTAL=F
        }
        if (int m = T2Port_EngineMode(); m != mode) {
            printf("game: frame %d: engine mode %d -> %d\n", f + 1, mode, m);
            fflush(stdout);
            mode = m;
        }
        T2Port_DebugWarp(f + 1); // DEBUG-ONLY: T2_WARP_TO=<id>[@frame][,...]
        if (args.skipPakCheck || args.startMode >= 0) {
            T2Port_DebugSwitches(args.skipPakCheck, args.startMode); // DEBUG-ONLY
        }
        if (args.audio) {
            // the frames the audio manager produced this retrace (22050 Hz stereo s16) to LUS's
            // AudioPlayer (a no-op when it has no audio backend, e.g. headless)
            static int16_t pcm[4096 * 2];
            size_t n = T2Audio_TakeSamples(pcm, 4096);
            if (n > 0) {
                AudioPlayerPlayFrame(reinterpret_cast<const uint8_t*>(pcm), n * 4);
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
        if (got && (window || T2Offscreen_Interpreter())) {
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
                badWords = CheckDL((const uintptr_t*)task.dataPtr, 0, &budget, f + 1);
                if (badWords > 0) {
                    skippedBad++; // Fast3D would dereference a truncated pointer: not drawn
                }
            }
            if (badWords == 0 &&
                (window ? window->DrawAndRunGraphicsCommands((Gfx*)task.dataPtr, {}) : T2Offscreen_Draw(const_cast<void*>(task.dataPtr)))) {
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
            if (drewThis || (hadTask && !(got && (window || T2Offscreen_Interpreter())))) {
                if (bLastDrawn != BenchClock::time_point{}) {
                    bGameFrame.push_back(Ms(bt3 - bLastDrawn));
                }
                bLastDrawn = bt3;
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
    }
    T2PifStats pif;
    T2Port_GetPifStats(&pif);
    printf("game: PIF %llu SI DMAs, %llu status, %llu pak reads, %llu pak writes\n", (unsigned long long)pif.dmas,
           (unsigned long long)pif.status, (unsigned long long)pif.reads, (unsigned long long)pif.writes);
    fflush(stdout);
    return 0;
}
