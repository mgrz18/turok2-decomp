#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800A7364;

void func_00259E30(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0x130) = (f32) D_800A7364;
    func_00243414(arg0, (s32) arg1, 4);
}
