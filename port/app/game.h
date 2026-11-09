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
    int lowMemory = 0;  // game memory below 2 GB (Linux bring-up aid)
    int verbose = 0;
};

// Boots the game and runs args.frames frames; window may be null (--no-window).
int T2Game_Run(const std::shared_ptr<Fast::Fast3dWindow>& window, const T2GameArgs& args);
