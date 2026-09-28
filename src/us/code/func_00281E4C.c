#include "common.h"
#include "m2c_macros.h"

s32 func_00220A1C(s32, s32);

f32 func_0026BE60(s32, f32, f32);                   /* extern */
s32 func_00284188();                                /* extern */
extern s32 D_800B6D00;
extern M2C_UNK D_800F7078;

void func_00281E4C(void *arg0) {
    f32 temp_f1;
    f32 temp_f20;
    f32 temp_f21;
    f32 temp_f22;
    f32 temp_f23;
    s32 temp_s1;

    if (func_00284188() != 0) {
        M2C_FIELD(arg0, s32 *, 0x58) = func_00220A1C((s32) &D_800F7078, arg0 + 0x38);
    }
    temp_s1 = M2C_FIELD(arg0, s32 *, 0x58);
    temp_f21 = M2C_FIELD(arg0, f32 *, 0x38);
    temp_f23 = M2C_FIELD(arg0, f32 *, 0x3C);
    temp_f20 = M2C_FIELD(arg0, f32 *, 0x40);
    temp_f22 = M2C_FIELD(arg0, f32 *, 0x44);
    M2C_FIELD(arg0, s32 *, 0x64) = (s32) D_800B6D00;
    if ((temp_s1 != 0) && (func_00284188() == 0)) {
        temp_f1 = ((temp_f23 + temp_f22) - func_0026BE60(temp_s1, temp_f21, temp_f20)) * 0.09765625f;
        if ((temp_f1 < 11.0f) && (temp_f1 > 5.0f)) {
            M2C_FIELD(arg0, s32 *, 0x64) = (s32) M2C_FIELD(temp_s1, s32 *, 0x1C);
        }
    }
    if (func_00284188() != 0) {
        M2C_FIELD(arg0, s32 *, 0x64) = (s32) D_800B6D00;
    }
    if (M2C_FIELD(arg0, f32 *, 0x5C) > 0.0f) {
        M2C_FIELD(arg0, s32 *, 0x64) = (s32) D_800B6D00;
    }
}
