#include "common.h"
#include "m2c_macros.h"

void func_00228BF8(void *arg0, s32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4) {
    *arg1 = M2C_FIELD(arg0, s32 *, 0x20);
    *arg4 = M2C_FIELD(arg0, f32 *, 0x10);
    *arg2 = M2C_FIELD(arg0, f32 *, 8);
    *arg3 = M2C_FIELD(arg0, f32 *, 0xC);
}
