#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;

void func_00239130(u8 *arg0, s32 arg1, s32 arg2, V3 arg3, s32 arg4) {
    if (arg0[0] == 1) {
        M2C_FIELD(arg0, s32 *, 0x27C) |= 1 << arg4;
    }
}
