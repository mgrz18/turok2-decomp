#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);
s32 func_0026DFB0(s32);

f32 func_0026D210(u16);                             /* extern */

void func_00225354(void *arg0, s32 arg1) {
    f32 temp_f0;
    s32 var_s0;

    var_s0 = M2C_FIELD(arg0, s32 *, 0x1194);
    if (var_s0 != 0) {
        func_00200738((s32) (arg0 + 0x1194), var_s0);
        func_00200518((s32) (arg0 + 0x1180), var_s0);
        M2C_FIELD(var_s0, s32 *, 8) = arg1;
    } else {
        var_s0 = M2C_FIELD(arg0, s32 *, 0x1184);
        func_0026DFB0(M2C_FIELD(var_s0, s32 *, 8));
        M2C_FIELD(var_s0, s32 *, 8) = arg1;
    }
    temp_f0 = func_0026D210(M2C_FIELD(arg1, u16 *, 0xC));
    M2C_FIELD(var_s0, f32 *, 0xC) = temp_f0;
    if (M2C_FIELD(arg1, u8 *, 0xE) & 2) {
        M2C_FIELD(var_s0, f32 *, 0xC) = (f32) (temp_f0 * 15.0f);
    }
}
