#include "common.h"
#include "m2c_macros.h"

s32 func_002017E8(s32 *t, s32 i, s32 *size) {
    *size = *(s32 *)((s8 *)t + ((i + 1) << 2) + 4) - *(s32 *)((s8 *)t + (i << 2) + 4);
    return (s32)t + *(s32 *)((s8 *)t + (i << 2) + 4);
}
