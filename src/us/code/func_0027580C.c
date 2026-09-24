#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00271AC4(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_800F2A00;
extern s32 D_8011ACA0;

void func_0027580C(s32 arg0) {
    if (D_8011ACA0 == 0) {
        func_00271AC4(&D_800F2A00, arg0);
    }
}
