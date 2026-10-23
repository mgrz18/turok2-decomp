#include "common.h"
#include "m2c_macros.h"

void func_0021F1D0(void *arg0, s32 arg1) {
    if (arg1 != 0) {
        M2C_FIELD(arg0, s32 *, 0x27C) = (s32) (M2C_FIELD(arg0, s32 *, 0x27C) | 2);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0x27C) = (s32) (M2C_FIELD(arg0, s32 *, 0x27C) & ~2);
}
