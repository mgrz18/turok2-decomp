#include "common.h"
#include "m2c_macros.h"

extern s32 func_00288C5C(s32, s32);
extern f32 D_800A9E14[];
extern s32 D_800F5D28[];

void func_00289700(void *arg0) {
    if (D_800F5D28[0] != 0) {
        M2C_FIELD(arg0, f32 *, 0x23FD0) = 0.0f;
    }
    if (M2C_FIELD(arg0, f32 *, 0x23FD0) > D_800A9E14[0]) {
        func_00288C5C((s32) arg0, 7);
    }
}
