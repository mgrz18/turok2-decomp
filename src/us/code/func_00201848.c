#include "common.h"
#include "m2c_macros.h"

s32 func_00201848(s32 arg0, s32 arg1, s32 arg2) {
    return arg1 + M2C_FIELD((arg0 + ((arg2 << 2))), s32 *, 4);
}
