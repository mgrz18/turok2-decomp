#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A7B3C;
extern f32 func_002119FC(f32, f32);

void func_00261E10(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0x64) = func_002119FC(0.0f, D_800A7B3C);
}
