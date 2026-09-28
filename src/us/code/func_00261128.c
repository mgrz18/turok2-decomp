#include "common.h"
#include "m2c_macros.h"

extern f32 func_0025EB50(s32, void *, f32, f32);

void func_00261128(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, u8 *, 0x37) = 0;
    M2C_FIELD(arg1, f32 *, 0x44) = func_0025EB50(arg0, arg1, 90.0f, 30.0f);
}
