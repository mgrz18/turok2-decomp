#include "common.h"
#include "m2c_macros.h"

extern u8 D_800F7078[];
extern s32 D_800B6D54;
extern s32 func_00224DF4(void *, s32, s32, s32);

s32 func_0027AE00(s32 arg0) {
    s32 r;

    if (D_800B6D54 != 0) {
        r = func_00224DF4(D_800F7078, arg0, 3, 1);
    } else {
        r = 0;
    }
    return r;
}
