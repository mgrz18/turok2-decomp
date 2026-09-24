#include "common.h"
#include "m2c_macros.h"

s32 func_002655C4(s32, s32);

extern M2C_UNK D_800B2290;

void func_00265544(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, M2C_UNK **, 0x2C) = &D_800B2290;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF0) = func_002655C4;
}
