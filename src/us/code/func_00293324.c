#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;

s16 func_00293324(u8 **arg0) {
    s32 hi = *arg0[2]++;
    s32 lo = *arg0[2]++;

    return (hi << 8) | lo;
}
