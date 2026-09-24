#include "common.h"
#include "m2c_macros.h"

f32 func_0021170C(f32, f32);
s32 func_0025E428(s32, s32);

extern M2C_UNK D_800A7A38;
extern f32 D_800B6D28;

void func_00261074(s32 arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f20;
    s8 temp_v1;

    M2C_FIELD(arg1, f32 *, 0xB0) = (f32) M2C_FIELD(arg1, f32 *, 0xA0);
    if ((func_0025E428(arg0, (s32) arg1) != 1) || ((temp_v1 = M2C_FIELD(arg1, s8 *, 0x34), (temp_v1 != 4)) && (temp_v1 != 0xE))) {
        temp_f20 = M2C_FIELD(&D_800A7A38, f32 *, 4);
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
