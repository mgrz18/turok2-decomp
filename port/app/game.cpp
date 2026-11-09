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
#include <vector>

extern "C" {
#include "t2port.h"
}

#include "game.h"

namespace {

// T2_DLDUMP=1: print the commands of the last frame's display list (following G_DL), for
// bring-up. Words are host-sized (gfx.h GfxWord).
void DumpDL(const uintptr_t* dl, int depth, int* budget) {
    for (; *budget > 0; dl += 2) {
        (*budget)--;
        uint32_t w0 = (uint32_t)dl[0];
        uint8_t op = w0 >> 24;
        printf("%*s%02X %08X %016llX\n", depth * 2, "", op, w0, (unsigned long long)dl[1]);
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

// LUS's OSContPad carries gyro and right-stick fields (0x24 bytes); the game's is the N64's
// 6-byte record.
void ContRead(void* pads) {
    OSContPad lus[MAXCONTROLLERS] = {};
    osContStartReadData(&sContQueue);
    osContGetReadData(lus);
    auto* out = static_cast<uint8_t*>(pads);
    for (int i = 0; i < 4 && i < MAXCONTROLLERS; i++) {
        uint16_t b = (uint16_t)lus[i].button;
        // N64 byte order is not an issue here: OSContPad.button is read as a u16 by the game
        memcpy(out + i * 6, &b, 2);
        out[i * 6 + 2] = (uint8_t)lus[i].stick_x;
        out[i * 6 + 3] = (uint8_t)lus[i].stick_y;
        out[i * 6 + 4] = lus[i].err_no;
    }
}

} // namespace

int T2Game_Run(const std::shared_ptr<Fast::Fast3dWindow>& window, const T2GameArgs& args) {
    if (window) {
        static const T2PortContHooks hooks = { ContInit, ContRead };
        T2Port_SetContHooks(&hooks);
    }
    T2PortConfig cfg = {};
    cfg.romPath = args.rom.empty() ? nullptr : args.rom.c_str();
    cfg.lowMemory = args.lowMemory;
    cfg.verbose = args.verbose;
    if (T2Port_Boot(&cfg) != 0) {
        return 1;
    }
    printf("game: booted, %d frames\n", args.frames);
    fflush(stdout);

    int drawn = 0;
    const void* lastDL = nullptr;
    auto t0 = std::chrono::steady_clock::now();
    for (int f = 0; f < args.frames && (!window || window->IsRunning()); f++) {
        if (window) {
            window->HandleEvents();
        }
        T2Port_Frame();
        T2PortTask task;
        if (T2Port_TakeGfxTask(&task) && args.draw && window && task.dataPtr != nullptr) {
            // Only with --draw: the game's display lists are not host-correct yet (GBI words
            // hold 32-bit addresses; docs/port/BUILD.md "GBI").
            lastDL = task.dataPtr;
            if (window->DrawAndRunGraphicsCommands((Gfx*)task.dataPtr, {})) {
                drawn++;
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
    if (const char* shot = getenv("T2_SHOT"); shot != nullptr && window && drawn > 0) {
        auto interp = window->GetInterpreterWeak().lock();
        if (interp && interp->mRapi) {
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
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    T2PortStats s;
    T2Port_GetStats(&s);
    printf("game: %llu frames in %.2f s, %llu thread switches, %llu gfx tasks (%d drawn), %llu audio tasks, "
           "%llu PI DMAs, %llu missing-function calls\n",
           (unsigned long long)s.frames, secs, (unsigned long long)s.switches, (unsigned long long)s.gfxTasks, drawn,
           (unsigned long long)s.audioTasks, (unsigned long long)s.piDmas, (unsigned long long)s.missingCalls);
    fflush(stdout);
    return 0;
}
