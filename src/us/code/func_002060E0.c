#include "common.h"
#include "m2c_macros.h"

s32 func_002031E8(s32, s32);
s32 func_002062F8(s32, s32);
s32 func_002063B4(s32, s32, s32, s32);
s32 func_00206498(s32, s32);

s32 func_002060E0(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s0;

    var_s0 = func_002031E8(0, arg2);
    if (var_s0 != 0) {
        M2C_FIELD(var_s0, s32 *, 8) = (s32) (M2C_FIELD(var_s0, s32 *, 8) + 1);
        M2C_FIELD(var_s0, s32 *, 0xC) = (s32) (M2C_FIELD(var_s0, s32 *, 0xC) | 0x100);
        temp_v0 = func_002063B4(0, arg1, ((u32) arg2 >> 5) & 1, arg2);
        M2C_FIELD(var_s0, s32 *, 0) = temp_v0;
        if (temp_v0 != 0) {
            M2C_FIELD(var_s0, s32 *, 4) = arg1;
            M2C_FIELD(var_s0, s32 *, 0xC) = (s32) (M2C_FIELD(var_s0, s32 *, 0xC) | arg2);
            func_00206498(0, var_s0);
            return var_s0;
        }
        temp_v0_2 = M2C_FIELD(var_s0, s32 *, 8) - 1;
        M2C_FIELD(var_s0, s32 *, 8) = temp_v0_2;
        if (temp_v0_2 == 0) {
            M2C_FIELD(var_s0, s32 *, 0xC) = (s32) (M2C_FIELD(var_s0, s32 *, 0xC) & ~0x100);
        }
        func_002062F8(0, var_s0);
        var_s0 = 0;
        goto block_6;
    }
block_6:
    return var_s0;
}
