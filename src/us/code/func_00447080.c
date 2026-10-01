#include "common.h"
#include "m2c_macros.h"

void func_00447080(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 0x50C) = (s32) (M2C_FIELD(arg1, s32 *, 0x50C) + 1);
}
