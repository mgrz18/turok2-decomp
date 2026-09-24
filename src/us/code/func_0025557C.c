#include "common.h"
#include "m2c_macros.h"

s32 func_0024E5F0(s32, s32, s32);

void func_0025557C(void *arg0, s32 arg1) {
    if ((M2C_FIELD(arg0, f32 *, 0xA78) != 0.0f) || (M2C_FIELD(arg0, f32 *, 0xA7C) != 0.0f)) {
        func_0024E5F0((s32) arg0, arg1, 3);
    }
}
