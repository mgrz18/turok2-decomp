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

extern "C" {
#include "t2port.h"
#include "t2audio.h"
}

#include "game.h"

namespace {

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
    cfg.audio = args.audio;
    if (T2Port_Boot(&cfg) != 0) {
        return 1;
    }
    printf("game: booted, %d frames\n", args.frames);
    fflush(stdout);

    int drawn = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int f = 0; f < args.frames && (!window || window->IsRunning()); f++) {
        if (window) {
            window->HandleEvents();
        }
        T2Port_Frame();
        if (args.audio) {
            // the frames the audio manager produced this retrace (22050 Hz stereo s16) to LUS's
            // AudioPlayer (a no-op when it has no audio backend, e.g. headless)
            static int16_t pcm[4096 * 2];
            size_t n = T2Audio_TakeSamples(pcm, 4096);
            if (n > 0) {
                AudioPlayerPlayFrame(reinterpret_cast<const uint8_t*>(pcm), n * 4);
            }
        }
        T2PortTask task;
        if (T2Port_TakeGfxTask(&task) && args.draw && window && task.dataPtr != nullptr) {
            // Only with --draw: the game's display lists are not host-correct yet (GBI words
            // hold 32-bit addresses; docs/port/BUILD.md "GBI").
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
