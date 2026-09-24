#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32 arg0, s32 arg1) {
    return arg0 + M2C_FIELD((arg0 + ((arg1 << 2))), s32 *, 4);
}
