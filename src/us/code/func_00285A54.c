#include "common.h"
#include "m2c_macros.h"

extern u8 gPakWatched[];

u8 Controller_IsPakWatched(s32 arg0) {
    return *(((u8 *)(gPakWatched + arg0)));
}
