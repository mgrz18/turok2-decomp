#include "common.h"
#include "m2c_macros.h"

s32 BitAddr_GetShift(s32 arg0) {
    return arg0 & 0x1F;
}
