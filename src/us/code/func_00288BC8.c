#include "common.h"
#include "m2c_macros.h"

s32 func_0028D0E0();                                /* extern */
extern f32 D_800A9DE0;

void func_00288BC8(void *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x23FD8);
    M2C_FIELD(arg0, s8 *, 0x23FE1) = 1;
    M2C_FIELD(arg0, f32 *, 0x23FE4) = 0.0f;
    M2C_FIELD(arg0, s32 *, 0x23FD8) = 0x11;
    M2C_FIELD(arg0, s32 *, 0x23FDC) = arg1;
    M2C_FIELD(arg0, s32 *, 0x23FD4) = temp_v1;
    if (func_0028D0E0() != 0) {
        M2C_FIELD(arg0, f32 *, 0x23FE4) = D_800A9DE0;
    }
}
