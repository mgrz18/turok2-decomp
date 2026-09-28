#include "common.h"
#include "m2c_macros.h"

s32 func_002946D0(s32, s32);

extern s32 D_800B7760[];

void func_00293400(s32 arg0, s32 arg1) {
    if (D_800B7760[0] == 0) {
        D_800B7760[0] = arg0;
        func_002946D0(arg0, arg1);
    }
}
