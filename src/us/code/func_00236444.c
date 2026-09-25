#include "common.h"
#include "m2c_macros.h"

s32 func_0026D3BC(s32);

void func_00236444(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 temp_v0;

    temp_v0 = func_0026D3BC(arg0 + 0x7580);
    if (temp_v0 != 0) {
        M2C_FIELD(temp_v0, s32 *, 0x18) = arg1;
        M2C_FIELD(temp_v0, s32 *, 0x14) = arg2;
        M2C_FIELD(temp_v0, s32 *, 8) = 0;
        M2C_FIELD(temp_v0, f32 *, 0xC) = arg4;
        M2C_FIELD(temp_v0, s32 *, 0x10) = arg3;
    }
}
