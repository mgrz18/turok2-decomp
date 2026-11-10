// game.h - `turok2 --game` (game.cpp, docs/port/LINK.md).
#pragma once

#include <memory>
#include <string>

namespace Fast {
class Fast3dWindow;
}

struct T2GameArgs {
    std::string rom;    // baserom.us.z64 for the PI DMA model (optional)
    int frames = 60;    // host frames (VI retraces) to run
    bool draw = false;  // hand the game's display lists to Fast3D
    int audio = 0;      // --audio: the game's sound (docs/port/AUDIO.md) through LUS's AudioPlayer
    std::string wav;    // --wav <file>: also write the mixed output (stereo s16, the synthesizer's rate) as a WAV
    int lowMemory = 0;  // game memory below 2 GB (Linux bring-up aid)
    int verbose = 0;
    int pakMode = 0;        // T2_PAK_FILE / T2_PAK_BLANK / T2_PAK_NONE (t2port.h, docs/port/INPUT.md)
    std::string pakDir;     // --pak-dir (empty: the user data dir)
    std::string input;      // --input: scripted button presses for headless runs (debug aid)
    // DEBUG-ONLY bring-up switches (port/game/t2debug.c) until the Controller Pak is modelled:
    int skipPakCheck = 0; // --skip-pak-check: mode 0 ends as "continue without saving"
    int startMode = -1;   // --start-mode N: switch to engine mode N once mode 0 has started
    // DEBUG-ONLY measurement (docs/port/PERF.md):
    int benchFrom = -1;   // --bench [FROM]: per-frame host times from VI FROM on (0: all), printed at the end
    int pace = 0;         // --pace: real time, one VI per 1/60 s (the game draws every second VI: 30 fps)
};

// Boots the game and runs args.frames frames; window may be null (--no-window).
int T2Game_Run(const std::shared_ptr<Fast::Fast3dWindow>& window, const T2GameArgs& args);
