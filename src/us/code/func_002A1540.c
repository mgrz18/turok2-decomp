#include "common.h"
#include "m2c_macros.h"

extern u32 func_002A1384(s32);

s32 func_002A1540(s32 arg0) {
    s32 shift;

    shift = ~arg0;
    shift &= 3;
    shift *= 8;
    return (func_002A1384(arg0 & ~3) >> shift) & 0xFF;
}
