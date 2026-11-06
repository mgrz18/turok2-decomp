#include "common.h"
#include "m2c_macros.h"

extern s32 gCache_frameDelay;

s32 CCache__GetFrameDelay(void) {
    return gCache_frameDelay;
}
