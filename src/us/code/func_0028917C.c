#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00285ABC();                            /* extern */
M2C_UNK func_00288C5C(s32, M2C_UNK);                /* extern */
M2C_UNK func_00416644(s32, M2C_UNK *, M2C_UNK, M2C_UNK *, s32); /* extern */
extern M2C_UNK D_0043B738;
extern f32 D_800B6D28;
extern s32 D_800B6D64;
extern M2C_UNK D_800F5C78;
extern f32 D_8011AD40;
extern s32 D_8011B118;
extern s32 D_8011F148;

void func_0028917C(s32 arg0) {
    if (D_800B6D64 != 0) {
        if (D_8011AD40 > 0.0f) {
            D_8011AD40 -= D_800B6D28;
        }
        if ((D_8011AD40 <= 0.0f) && (D_8011B118 == 0)) {
            D_8011AD40 = 0.0f;
            D_8011B118 = 1;
            func_00416644(arg0 + 0x2317C, &D_0043B738, 0, &D_800F5C78, 0);
        }
        if (D_8011F148 != 0) {
            func_00288C5C(arg0, 1);
            func_00285ABC();
        }
    }
}
