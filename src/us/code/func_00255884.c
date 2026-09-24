#include "common.h"
#include "m2c_macros.h"

s32 func_0024E5F0(s32, s32, s32);
f32 func_00255940(s32, s32);

void func_00255884(void *arg0, void *arg1) {
    f32 temp_f2;

    temp_f2 = M2C_FIELD(arg0, f32 *, 0xA7C);
    if (((temp_f2 < 0.0f) && (M2C_FIELD(arg0, f32 *, 0xA5C) >= 0.0f)) || ((temp_f2 > 0.0f) && (M2C_FIELD(arg0, f32 *, 0xA5C) <= 0.0f)) || (M2C_FIELD(arg0, f32 *, 0xA10) >= 1.5f)) {
        func_0024E5F0((s32) arg0, (s32) arg1, 7);
    } else {
        M2C_FIELD(arg1, f32 *, 0x1C) = 307.19998f;
    }
    func_00255940((s32) arg0, (s32) arg1);
}
