#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011AAD8;

void func_002536E0(s32 arg0, s32 arg1) {
    void *temp_v1;

    if (!(D_8011AAD8 & 0x10)) {
        if (arg1 < 0x1121) {
            if (arg1 < 0x111C) {
                goto block_3;
            }
        } else {
block_3:
            { s32 i = arg1 - 0xFA0; temp_v1 = (s8 *)arg0 + i; }
            M2C_FIELD(temp_v1, u8 *, 0x52C) = (u8) (M2C_FIELD(temp_v1, u8 *, 0x52C) - 1);
        }
    }
}
