#include "common.h"
#include "m2c_macros.h"

void func_004470A0(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 0x514) = (s32) (M2C_FIELD(arg1, s32 *, 0x514) + 1);
}
