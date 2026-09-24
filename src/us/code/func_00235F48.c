#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002366C8();                            /* extern */
M2C_UNK func_002367A0(s32);                         /* extern */
extern s32 D_800ACFC0;
extern s32 D_8011ACA4;

void func_00235F48(s32 arg0) {
    D_800ACFC0 = 0;
    if (D_8011ACA4 == 0) {
        func_002366C8();
        func_002367A0(arg0);
    }
}
