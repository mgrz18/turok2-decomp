#include "common.h"
#include "m2c_macros.h"

/* Offset table: returns base + the i-th offset and stores the i-th entry's size. */
s32 func_0020185C(s32 *t, s32 base, s32 i, s32 *size) {
    *size = *(s32 *)((s8 *)t + ((i + 1) << 2) + 4) - *(s32 *)((s8 *)t + (i << 2) + 4);
    return base + *(s32 *)((s8 *)t + (i << 2) + 4);
}
