#include "common.h"
#include "m2c_macros.h"

extern s32 gCache_frameDelay;

void CCache__SetFrameDelay(s32 arg0) {
    gCache_frameDelay = arg0;
}
