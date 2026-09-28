#include "common.h"
#include "m2c_macros.h"

void func_00265908(s32 arg0, void *arg1, f32 arg2) {
    if ((arg2 == 0.0f) || (M2C_FIELD(arg1, f32 *, 0x64) < arg2)) {
        if (M2C_FIELD(arg1, f32 *, 0x64) == 0.0f) {
            M2C_FIELD(arg1, f32 *, 0x40) = 0.0f;
        }
        M2C_FIELD(arg1, f32 *, 0x64) = arg2;
    }
}
