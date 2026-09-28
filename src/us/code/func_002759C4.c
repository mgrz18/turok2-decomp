#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00275A08();                            /* extern */
extern s32 D_800F478C[];
extern s32 D_8011ACA0[];

void func_002759C4(s32 arg0) {
    if ((D_8011ACA0[0] == 0) && (D_800F478C[0] != arg0)) {
        D_800F478C[0] = arg0;
        if (arg0 == 0) {
            func_00275A08();
        }
    }
}
