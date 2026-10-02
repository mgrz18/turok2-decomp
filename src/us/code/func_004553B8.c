#include "common.h"
#include "m2c_macros.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern M2C_UNK D_8011AAE8;

s32 func_004553B8(s32 arg0, s32 arg1) {
    u16 temp_v0;
    u8 var_v1;

    temp_v0 = func_0041648C(arg1, (s32) M2C_FIELD(&D_8011AAE8, u16 *, 0), 0x10, 0, 0x100, 0);
    M2C_FIELD(&D_8011AAE8, u16 *, 0) = temp_v0;
    var_v1 = 0xFF;
    if ((u32) (temp_v0 & 0xFFFF) < 0xFFU) {
        var_v1 = M2C_FIELD(&D_8011AAE8, u8 *, 1);
    }
    M2C_FIELD(&D_8011AAE8, u8 *, 2) = var_v1;
    return 0;
}
