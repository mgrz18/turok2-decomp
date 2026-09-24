#include "common.h"
#include "m2c_macros.h"

s32 func_0025ED4C(s32, s32);

void func_00260060(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF4) = func_0025ED4C;
    M2C_FIELD(arg1, f32 *, 0xD0) = 75.0f;
}
