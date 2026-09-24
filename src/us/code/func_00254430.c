#include "common.h"
#include "m2c_macros.h"

s32 func_00249358(s32, s32, s32, s32, s32);

M2C_UNK func_0027580C(M2C_UNK);                     /* extern */
extern f32 D_800B6D28;
extern u8 D_8011AAF1;

void func_00254430(void *arg0, s32 arg1) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s1;

    temp_f1 = M2C_FIELD(arg0, f32 *, 0xBE4);
    temp_f0 = temp_f1 + D_800B6D28;
    M2C_FIELD(arg0, f32 *, 0xBE4) = temp_f0;
    if ((temp_f1 < 300.0f) && (temp_f0 > 300.0f) && (D_8011AAF1 == 0)) {
        func_0027580C(0x163);
    }
    if (M2C_FIELD(arg0, f32 *, 0xBE4) > 600.0f) {
        temp_v0 = (s32) M2C_FIELD(arg0, s32 *, 0x524) >> 8;
        var_s1 = 1;
        if ((0x96 - temp_v0) > 0) {
            temp_v1 = (0x96 - temp_v0) / 25;
            if (temp_v1 > 0) {
                var_s1 = temp_v1;
            }
        }
        if (M2C_FIELD(arg0, s32 *, 0xBEC) == 0) {
            if (D_8011AAF1 == 0) {
                func_0027580C(0x164);
            }
        }
        M2C_FIELD(arg0, s32 *, 0xBEC) = 1;
        func_00249358((s32) arg0, arg1, var_s1 << 8, 0, 0x1C);
        M2C_FIELD(arg0, f32 *, 0xBE4) = (f32) (M2C_FIELD(arg0, f32 *, 0xBE4) - 37.5f);
    }
}
