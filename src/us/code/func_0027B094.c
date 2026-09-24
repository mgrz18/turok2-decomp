#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_80110190;

void func_0027B094(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    M2C_FIELD(&D_80110190, s32 *, 0) = arg0;
    M2C_FIELD(&D_80110190, s32 *, 4) = arg1;
    M2C_FIELD(&D_80110190, s32 *, 8) = arg2;
    M2C_FIELD(&D_80110190, s32 *, 0xC) = arg3;
    M2C_FIELD(&D_80110190, s32 *, 0x10) = arg4;
    M2C_FIELD(&D_80110190, s32 *, 0x14) = arg5;
}
