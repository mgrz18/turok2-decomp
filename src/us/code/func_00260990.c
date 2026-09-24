#include "common.h"
#include "m2c_macros.h"

f32 func_0021170C(f32, f32);
s32 func_0025E428(s32, s32);

extern f32 D_800A79D0;
extern f32 D_800B6D28;

void func_00260990(s32 arg0, s32 arg1) {
    f32 temp_f0;
    f32 temp_f20;

    if ((func_0025E428(arg0, arg1) != 1) || (M2C_FIELD(arg1, s8 *, 0x34) != 4)) {
        temp_f20 = D_800A79D0;
        M2C_FIELD(arg1, f32 *, 0x110) = func_0021170C(M2C_FIELD(arg1, f32 *, 0x110), temp_f20);
        M2C_FIELD(arg1, f32 *, 0x10C) = func_0021170C(M2C_FIELD(arg1, f32 *, 0x10C), temp_f20);
    }
    if (M2C_FIELD(arg1, s8 *, 0x37) == 0) {
        temp_f0 = M2C_FIELD(arg1, f32 *, 0x44) - D_800B6D28;
        M2C_FIELD(arg1, f32 *, 0x44) = temp_f0;
        if (temp_f0 < 0.0f) {
            M2C_FIELD(arg1, s8 *, 0x37) = 1;
        }
    }
}
