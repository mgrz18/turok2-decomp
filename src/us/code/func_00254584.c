#include "common.h"
#include "m2c_macros.h"

s32 func_00249358(s32, s32, s32, s32, s32);
s32 func_00254430(s32, s32);

extern s32 D_8011AAD8;

void func_00254584(void *arg0, void *arg1) {
    s32 var_v0;

    if (D_8011AAD8 & 0x10) {
        var_v0 = 1;
    } else {
        var_v0 = M2C_FIELD(arg0, u8 *, 0x6A8) & 0xF;
    }
    if (var_v0 != 0) {
        if (M2C_FIELD(arg1, s32 *, 0x140) & 0x80) {
            func_00254430(arg0, arg1);
        }
    } else {
        func_00249358(arg0, arg1, 0xA00, 0x02000000, 0x1C);
    }
}
