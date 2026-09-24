#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A7188;
extern f32 D_800A718C;
extern f32 D_800A7190;

void func_00255840(void *arg0, void *arg1) {
    if (M2C_FIELD(arg0, f32 *, 0xA5C) < 0.0f) {
        M2C_FIELD(arg0, f32 *, 0xA7C) = (f32) D_800A7188;
    } else {
        M2C_FIELD(arg0, f32 *, 0xA7C) = (f32) D_800A718C;
    }
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) D_800A7190;
}
