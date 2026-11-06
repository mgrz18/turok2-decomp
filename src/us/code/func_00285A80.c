#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"


void Controller_UnwatchPak(s32 arg0) {
    *(((u8 *)(gPakWatched + arg0))) = 0;
}
