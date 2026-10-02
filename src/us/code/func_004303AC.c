#include "common.h"
#include "m2c_macros.h"

s32 func_0042C194(s32, s32, s32, s32, s32);

extern M2C_UNK D_800F7078;
extern M2C_UNK D_8011ACB0;

void func_004303AC(void *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (func_0042C194((s32) &D_800F7078, -1, -1, (s32) sp18, 1) == 0) {
        var_s1 = 0;
    }
    M2C_FIELD(&D_8011ACB0, s32 *, 0) = 0;
    M2C_FIELD(&D_8011ACB0, s32 *, 4) = 0;
    M2C_FIELD(&D_8011ACB0, s32 *, 8) = 0;
    M2C_FIELD(&D_8011ACB0, s32 *, 0xC) = 0;
    M2C_FIELD(arg0, s32 *, 0x23FFC) = 0;
    M2C_FIELD(&D_8011ACB0, s32 *, 0x44) = 0;
    M2C_FIELD(arg0, s8 *, 0x23FE1) = 2;
    M2C_FIELD(arg0, s32 *, 0x23FF8) = var_s1;
    M2C_FIELD(arg0, s32 *, 0x23FDC) = 0xB;
}
