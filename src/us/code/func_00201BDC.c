#include "common.h"
#include "m2c_macros.h"

s32 func_0029B820(s32, s32, s32);

s32 func_00201BDC(s32 arg0, void *arg1, s32 arg2) {
    M2C_FIELD(arg1, s32 *, 0x20) = arg2;
    return ~func_0029B820(arg0 + 0x230, (s32) arg1, 0) != 0;
}
