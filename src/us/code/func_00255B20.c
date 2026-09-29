#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800A71A8;

void func_00255B20(void *arg0, void *arg1) {
    M2C_FIELD(arg0, f32 *, 0xA78) = (f32) (M2C_FIELD(arg0, f32 *, 0xA78) * M2C_FIELD(&D_800A71A8, f32 *, 4));
    M2C_FIELD(arg0, f32 *, 0xA7C) = (f32) (M2C_FIELD(arg0, f32 *, 0xA7C) * M2C_FIELD(&D_800A71A8, f32 *, 4));
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) (M2C_FIELD(arg1, f32 *, 0x1C) * M2C_FIELD(&D_800A71A8, f32 *, 4));
}
