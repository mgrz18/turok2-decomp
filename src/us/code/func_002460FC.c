#include "common.h"
#include "m2c_macros.h"

void func_002460FC(s32 arg0, void *arg1, s32 arg2, s32 arg3) {
    M2C_FIELD(arg1, s32 *, 0x78) = arg2;
    M2C_FIELD(arg1, s32 *, 0x7C) = arg3;
}
