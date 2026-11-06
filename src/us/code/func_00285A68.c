#include "common.h"
#include "m2c_macros.h"

extern u8 gPakWatched[];

void Controller_WatchPak(s32 arg0) {
    gPakWatched[arg0] = 1;
}
