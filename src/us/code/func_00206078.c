#include "common.h"
#include "m2c_macros.h"
#include "globals/cache.h"
#include "protos/cache.h"


s32 CCache__GetFrameDelay(void) {
    return gCache_frameDelay;
}
