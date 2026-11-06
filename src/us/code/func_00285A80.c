#include "common.h"
#include "m2c_macros.h"

extern u8 gPakWatched[];

void Controller_UnwatchPak(s32 arg0) {
    *(((u8 *)(gPakWatched + arg0))) = 0;
}
