#include "common.h"
#include "m2c_macros.h"

void func_00210284(void *arg0, f32 arg1, f32 arg2, f32 arg3) {
    M2C_FIELD(arg0, f32 *, 0x30) = (f32) (M2C_FIELD(arg0, f32 *, 0x30) + ((arg1 * M2C_FIELD(arg0, f32 *, 0)) + (arg2 * M2C_FIELD(arg0, f32 *, 0x10)) + (arg3 * M2C_FIELD(arg0, f32 *, 0x20))));
    M2C_FIELD(arg0, f32 *, 0x34) = (f32) (M2C_FIELD(arg0, f32 *, 0x34) + ((arg1 * M2C_FIELD(arg0, f32 *, 4)) + (arg2 * M2C_FIELD(arg0, f32 *, 0x14)) + (arg3 * M2C_FIELD(arg0, f32 *, 0x24))));
    M2C_FIELD(arg0, f32 *, 0x38) = (f32) (M2C_FIELD(arg0, f32 *, 0x38) + ((arg1 * M2C_FIELD(arg0, f32 *, 8)) + (arg2 * M2C_FIELD(arg0, f32 *, 0x18)) + (arg3 * M2C_FIELD(arg0, f32 *, 0x28))));
}
