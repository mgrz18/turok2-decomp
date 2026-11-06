#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"


void Controller_WatchPak(s32 arg0) {
    gPakWatched[arg0] = 1;
}
