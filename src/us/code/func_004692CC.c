#include "common.h"
#include "m2c_macros.h"

s32 func_0042C0B4(M2C_UNK *, M2C_UNK, M2C_UNK, M2C_UNK *, s32); /* extern */
extern M2C_UNK D_800F7078;
extern M2C_UNK D_8011ACB0;

void func_004692CC(void *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (func_0042C0B4(&D_800F7078, -1, -1, sp18, 1) == 0) {
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
