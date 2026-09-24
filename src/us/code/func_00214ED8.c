#include "common.h"
#include "m2c_macros.h"

extern s32 D_800ACAF0;

s32 func_00214ED8(void) {
    s32 temp_v0;

    temp_v0 = D_800ACAF0 + 1;
    D_800ACAF0 = temp_v0;
    if (temp_v0 == 0x3FFFFF) {
        D_800ACAF0 = 0x380000;
    }
    return D_800ACAF0;
}
