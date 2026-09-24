#include "common.h"
#include "m2c_macros.h"

s32 func_0026CDAC(s32, s32, s32);

void func_0026CD70(void *arg0, s32 arg1, void *arg2) {
    f32 temp_f0;
    f32 temp_f0_2;

    if (arg1 != 0) {
        temp_f0 = M2C_FIELD(arg2, f32 *, 0);
        M2C_FIELD(arg0, f32 *, 8) = temp_f0;
        M2C_FIELD(arg0, f32 *, 0) = temp_f0;
        temp_f0_2 = M2C_FIELD(arg2, f32 *, 8);
        M2C_FIELD(arg0, f32 *, 0xC) = temp_f0_2;
        M2C_FIELD(arg0, f32 *, 4) = temp_f0_2;
        func_0026CDAC((s32) arg0, arg1 - 1, (s32) (arg2 + 0xC));
    }
}
