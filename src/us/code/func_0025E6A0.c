#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800B6D28;

void func_0025E6A0(s32 arg0, void *arg1) {
    f32 temp_f0;

    temp_f0 = M2C_FIELD(arg1, f32 *, 0x64) - D_800B6D28;
    M2C_FIELD(arg1, f32 *, 0x64) = temp_f0;
    if (temp_f0 <= 0.0f) {
        func_00243414(arg0, (s32) arg1, 2);
    }
}
