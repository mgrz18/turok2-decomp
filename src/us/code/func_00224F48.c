#include "common.h"
#include "m2c_macros.h"

s32 func_0020185C(s32, s32, s32, s32);

void func_00224F48(void *arg0, s32 arg1, s32 *arg2, s32 arg3) {
    *arg2 = func_0020185C(M2C_FIELD(arg0, s32 *, 0x3C), M2C_FIELD(arg0, s32 *, 0x10), arg1, arg3);
}
