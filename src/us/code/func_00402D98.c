#include "common.h"
#include "m2c_macros.h"

f32 func_0021170C(f32, f32);
s32 func_00243414(s32, s32, s32);

extern f32 D_800C0354;

void func_00402D98(s32 arg0, void *arg1) {
    f32 temp_f20;

    temp_f20 = D_800C0354;
    M2C_FIELD(arg1, f32 *, 0x54) = func_0021170C(M2C_FIELD(arg1, f32 *, 0x54), temp_f20);
    M2C_FIELD(arg1, f32 *, 0x5C) = func_0021170C(M2C_FIELD(arg1, f32 *, 0x5C), temp_f20);
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        func_00243414(arg0, (s32) arg1, 5);
    }
}
