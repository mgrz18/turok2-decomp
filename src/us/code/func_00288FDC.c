#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A9DF4;

extern f32 D_800B6D28;
extern M2C_UNK D_8011ACB0;

void func_00288FDC(void *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f1;

    if (M2C_FIELD(arg0, s32 *, 0x23FF4) == 0) {
        temp_f1 = M2C_FIELD(&D_8011ACB0, f32 *, 0) + D_800B6D28;
        M2C_FIELD(&D_8011ACB0, f32 *, 0) = temp_f1;
        if (temp_f1 >= 15.0f) {
            temp_f0 = M2C_FIELD(&D_8011ACB0, f32 *, 4) + 1.0f;
            M2C_FIELD(&D_8011ACB0, f32 *, 0) = (f32) (temp_f1 - 15.0f);
            M2C_FIELD(&D_8011ACB0, f32 *, 4) = temp_f0;
            if (D_800A9DF4 <= temp_f0) {
                temp_f0_2 = M2C_FIELD(&D_8011ACB0, f32 *, 8) + 1.0f;
                M2C_FIELD(&D_8011ACB0, f32 *, 4) = (f32) (temp_f0 - D_800A9DF4);
                M2C_FIELD(&D_8011ACB0, f32 *, 8) = temp_f0_2;
                if (D_800A9DF4 <= temp_f0_2) {
                    M2C_FIELD(&D_8011ACB0, f32 *, 8) = (f32) (temp_f0_2 - D_800A9DF4);
                    M2C_FIELD(&D_8011ACB0, f32 *, 0xC) = (f32) (M2C_FIELD(&D_8011ACB0, f32 *, 0xC) + 1.0f);
                }
            }
        }
    }
}
