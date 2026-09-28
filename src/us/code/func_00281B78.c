#include "common.h"
#include "m2c_macros.h"

void func_00281B78(void *arg0, f32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0) = arg2;
    M2C_FIELD(arg0, f32 *, 4) = (f32) (arg1 * 0.1f);
}
