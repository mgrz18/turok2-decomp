#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;
typedef struct { f32 x, y, z; } V3f;

extern f32 D_800A77FC;

f32 func_0025EB50(s32 arg0, u8 *arg1, f32 arg2, f32 arg3) {
    return arg2 + arg1[0xC8] * D_800A77FC * (arg3 - arg2);
}
