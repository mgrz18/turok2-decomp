#include "common.h"
#include "m2c_macros.h"

extern f32 D_800C054C;

void func_00440894(void *arg0) {
    M2C_FIELD(arg0, f32 *, 0x1FC) = (f32) D_800C054C;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2000);
}
