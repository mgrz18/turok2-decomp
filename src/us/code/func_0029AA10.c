#include "common.h"
#include "m2c_macros.h"

extern s32 D_800B7AC0;

s32 __osViGetCurrentContext(void) {
    return D_800B7AC0;
}
