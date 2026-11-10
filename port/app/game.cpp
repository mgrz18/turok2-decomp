// game.cpp - `turok2 --game`: run the linked game on the port runtime (docs/port/LINK.md §4).
//
// main.cpp has built the LUS context, mounted the archives and (unless --no-window) opened the
// Fast3D window. This file boots the game (T2Port_Boot: arena, ROM, t2os, the mainproc thread)
// and pumps frames: per host frame one VI retrace, the game threads run until all are blocked,
// then the display list the scheduler started is (with --draw) handed to Fast3D. Controllers
// come from LUS's ControlDeck through the runtime's hooks.
#include <libultraship/libultraship.h>

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

extern "C" {
#include "t2port.h"
#include "t2audio.h"
// port/game/t2debug.c
int T2Port_EngineMode(void);
void T2Port_DebugSwitches(int skipPak, int startMode);
}

#include "game.h"

// the game's screen size (render.h; Video_SetMode writes it)
extern "C" int32_t gScreenWidth, gScreenHeight;
extern "C" const void* T2Game_TopMenu(int* cursor); // port/game/t2modelog.c

namespace {

// T2_DLDUMP=1: print the commands of the last frame's display list (following G_DL), for
// bring-up. Words are host-sized (gfx.h GfxWord).
// T2_DLDUMP_VTX=1 also prints the vertices of each G_VTX (segments from G_MW_SEGMENT) and the
// 16 words of each G_MTX (s15.16, Fast3D's host-order int32[16]) (docs/port/RENDER.md).
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
        for (int i = 0; i < n && i < 4; i++, v += 8) {
            const uint8_t* c = (const uint8_t*)(v + 6);
            printf("%*s   v%d ob %d %d %d flag %d tc %d %d cn %02X%02X%02X%02X\n", depth * 2, "", i, v[0], v[1], v[2],
                   v[3], v[4], v[5], c[0], c[1], c[2], c[3]);
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

OSMesgQueue sContQueue;
OSMesg sContMsg[1];

// LUS's osContInit/osContGetReadData (this file does not see port/include/t2_rename.h).
int ContInit(uint8_t* bits) {
    OSContStatus status[MAXCONTROLLERS] = {};
    osCreateMesgQueue(&sContQueue, sContMsg, 1);
    return osContInit(&sContQueue, bits, status);
}

// --input "START@200,A@320,DD@400": scripted presses on port 1 for headless runs (debug aid,
// docs/port/INPUT.md). Each press holds the button for kInputHold host frames from FRAME on.
struct ScriptedPress {
    int frame;
    uint16_t button;
};
std::vector<ScriptedPress> sScript;
int sFrame;
constexpr int kInputHold = 4;

bool ParseInputScript(const std::string& spec) {
    static const struct {
        const char* name;
        uint16_t bit;
    } kButtons[] = { { "A", 0x8000 },  { "B", 0x4000 },  { "Z", 0x2000 },  { "START", 0x1000 }, { "DU", 0x0800 },
                     { "DD", 0x0400 }, { "DL", 0x0200 }, { "DR", 0x0100 }, { "L", 0x0020 },     { "R", 0x0010 },
                     { "CU", 0x0008 }, { "CD", 0x0004 }, { "CL", 0x0002 }, { "CR", 0x0001 } };
    size_t pos = 0;
    while (pos < spec.size()) {
        size_t end = spec.find(',', pos);
        std::string item = spec.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
        pos = end == std::string::npos ? spec.size() : end + 1;
        size_t at = item.find('@');
        if (at == std::string::npos) {
            return false;
        }
        std::string name = item.substr(0, at);
        uint16_t bit = 0;
        for (auto& b : kButtons) {
            if (name == b.name) {
                bit = b.bit;
            }
        }
        if (bit == 0) {
            return false;
        }
        sScript.push_back({ atoi(item.c_str() + at + 1), bit });
    }
    return true;
}

uint16_t ScriptedButtons() {
    uint16_t b = 0;
    for (auto& p : sScript) {
        if (sFrame >= p.frame && sFrame < p.frame + kInputHold) {
            b |= p.button;
        }
    }
    return b;
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
        out[i * 6 + 2] = (uint8_t)lus[i].stick_x;
        out[i * 6 + 3] = (uint8_t)lus[i].stick_y;
        out[i * 6 + 4] = lus[i].err_no;
    }
}

// Reads back the last frame Fast3D drew and writes it as a binary PPM (T2_SHOT).
void WriteShot(const std::shared_ptr<Fast::Fast3dWindow>& window, const char* shot) {
    if (!window) {
        return;
    }
    auto interp = window->GetInterpreterWeak().lock();
    if (!interp || !interp->mRapi) {
        return;
    }
    const uint32_t w = interp->mCurDimensions.width, h = interp->mCurDimensions.height;
    std::vector<uint16_t> fb((size_t)w * h);
    interp->mRapi->ReadFramebufferToCPU(0, w, h, fb.data());
    if (FILE* fp = fopen(shot, "wb")) {
        fprintf(fp, "P6\n%u %u\n255\n", w, h);
        for (uint32_t y = 0; y < h; y++) {
            for (uint32_t x = 0; x < w; x++) {
                uint16_t c = fb[(size_t)(h - 1 - y) * w + x];
                uint8_t px[3] = { (uint8_t)((c >> 11 & 31) << 3), (uint8_t)((c >> 6 & 31) << 3),
                                  (uint8_t)((c >> 1 & 31) << 3) };
                fwrite(px, 1, 3, fp);
            }
        }
        fclose(fp);
        printf("game: wrote %s (%ux%u)\n", shot, w, h);
    }
}

} // namespace

extern "C" {
int Sound__Play2D(int id);
void Music_SetTrack(int track);
void T2Audio_TestVolumes(void);
}

int T2Game_Run(const std::shared_ptr<Fast::Fast3dWindow>& window, const T2GameArgs& args) {
    // Debug only (audio bring-up, docs/port/AUDIO.md section 9): T2_TEST_SFX=<id> plays the
    // game's sound <id> through Sound__Play2D at frame 30, T2_TEST_SONG=<n> starts music
    // track <n> through Music_SetTrack at frame 60 (both between frames, when every game thread
    // is idle). Needs --audio.
    const char* testSfx = getenv("T2_TEST_SFX");
    const char* testSong = getenv("T2_TEST_SONG");
    if (window) {
        static const T2PortContHooks hooks = { ContInit, ContRead };
        T2Port_SetContHooks(&hooks);
    }
    T2PortConfig cfg = {};
    cfg.romPath = args.rom.empty() ? nullptr : args.rom.c_str();
    cfg.lowMemory = args.lowMemory;
    cfg.verbose = args.verbose;
    cfg.audio = args.audio;
    cfg.pakMode = args.pakMode;
    cfg.pakDir = args.pakDir.empty() ? nullptr : args.pakDir.c_str();
    if (!args.input.empty() && !ParseInputScript(args.input)) {
        fprintf(stderr, "--input: expected BUTTON@FRAME[,...] (A B Z START DU DD DL DR L R CU CD CL CR)\n");
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

    int drawn = 0;
    const void* lastDL = nullptr;
    int mode = -1;
    // T2_SHOT_EVERY=<n> (with T2_SHOT=<file.ppm>): also write <file>.<frame>.ppm every n frames
    const char* shotEnv = getenv("T2_SHOT_EVERY");
    const int shotEvery = shotEnv != nullptr ? atoi(shotEnv) : 0;
    int nextShot = shotEvery;
    const char* dumpAtEnv = getenv("T2_DLDUMP_AT");
    const int dumpAt = dumpAtEnv != nullptr ? atoi(dumpAtEnv) : 0;
    bool dumped = false;
    auto t0 = std::chrono::steady_clock::now();
    for (int f = 0; f < args.frames && (!window || window->IsRunning()); f++) {
        if (window) {
            window->HandleEvents();
        }
        sFrame = f;
        if (args.audio && (testSfx != nullptr || testSong != nullptr) && f == 29) {
            T2Audio_TestVolumes();
        }
        if (args.audio && testSfx != nullptr && f == 30) {
            int h = Sound__Play2D((int)strtol(testSfx, nullptr, 0));
            printf("game: T2_TEST_SFX: Sound__Play2D(%s) -> handle %d\n", testSfx, h);
        }
        if (args.audio && testSong != nullptr && f == 60) {
            Music_SetTrack((int)strtol(testSong, nullptr, 0));
            printf("game: T2_TEST_SONG: Music_SetTrack(%s)\n", testSong);
        }
        T2Port_Frame();
        static const bool menuLog = getenv("T2_MENU_LOG") != nullptr;
        static const void* lastMenu = nullptr;
        static int lastCursor = -1;
        int cursor;
        if (const void* menu = T2Game_TopMenu(&cursor); menuLog && (menu != lastMenu || cursor != lastCursor)) {
            printf("game: frame %d: menu desc %p cursor %d\n", f, menu, cursor);
            lastMenu = menu;
            lastCursor = cursor;
        }
        if (int m = T2Port_EngineMode(); m != mode) {
            printf("game: frame %d: engine mode %d -> %d\n", f + 1, mode, m);
            fflush(stdout);
            mode = m;
        }
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
        const bool got = T2Port_TakeGfxTask(&task) && args.draw && task.dataPtr != nullptr;
        if (got) {
            lastDL = task.dataPtr;
            // T2_DLDUMP_AT=<frame>: dump the first display list from that frame on, now, before
            // the game runs again (no window needed: macOS has no headless GL, MACOS.md)
            if (dumpAt > 0 && f + 1 >= dumpAt && !dumped) {
                dumped = true;
                int budget = 4000;
                printf("dl: frame %d\n", f + 1);
                DumpDL((const uintptr_t*)lastDL, 0, &budget);
            }
        }
        if (got && window) {
            // Only with --draw: the game's display lists are not host-correct yet (GBI words
            // hold 32-bit addresses; docs/port/BUILD.md "GBI").
            // Fast3D maps RDP coordinates from its native size (320x240 by default) to the
            // window: the game draws at the size Video_SetMode chose (gScreenWidth x
            // gScreenHeight, 480x360 in mode 0), so tell it before every frame.
            if (auto interp = window->GetInterpreterWeak().lock();
                interp && gScreenWidth > 0 && gScreenHeight > 0 &&
                (interp->mNativeDimensions.width != (uint32_t)gScreenWidth ||
                 interp->mNativeDimensions.height != (uint32_t)gScreenHeight)) {
                interp->SetNativeDimensions((float)gScreenWidth, (float)gScreenHeight);
            }
            if (window->DrawAndRunGraphicsCommands((Gfx*)task.dataPtr, {})) {
                drawn++;
                if (const char* shot = getenv("T2_SHOT"); shot != nullptr && shotEvery > 0 && f + 1 >= nextShot) {
                    nextShot = (f + 1) / shotEvery * shotEvery + shotEvery; // the first drawn frame of each period
                    std::string name = std::string(shot) + "." + std::to_string(f + 1) + ".ppm";
                    WriteShot(window, name.c_str());
                }
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
    T2PifStats pif;
    T2Port_GetPifStats(&pif);
    printf("game: PIF %llu SI DMAs, %llu status, %llu pak reads, %llu pak writes\n", (unsigned long long)pif.dmas,
           (unsigned long long)pif.status, (unsigned long long)pif.reads, (unsigned long long)pif.writes);
    fflush(stdout);
    return 0;
}
