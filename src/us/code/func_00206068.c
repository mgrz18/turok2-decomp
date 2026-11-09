#include "common.h"
#include "m2c_macros.h"
#include "globals/cache.h"
#include "protos/cache.h"


void CCache__SetFrameDelay(s32 arg0) {
    gCache_frameDelay = arg0;
}
