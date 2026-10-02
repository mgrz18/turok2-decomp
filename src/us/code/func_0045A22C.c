#include "common.h"
#include "m2c_macros.h"

extern s32 D_800F70A8;

void func_0045A22C(s32 arg0, s32 arg1) {
    M2C_FIELD((D_800F70A8 + arg0), s32 *, 0x684) = arg1;
}
