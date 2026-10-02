#include "common.h"
#include "m2c_macros.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern M2C_UNK D_8011AAD8;

s32 func_004557E0(s32 arg0, s32 arg1) {
    M2C_UNK *var_s0;

    var_s0 = &D_8011AAD8;
    var_s0 = &D_8011AAD8;
    M2C_FIELD(&D_8011AAD8, s32 *, 0xC) = func_0041648C(arg1, M2C_FIELD(&D_8011AAD8, s32 *, 0xC), 0x10, 0, 0x80, 0);
    return 0;
}
