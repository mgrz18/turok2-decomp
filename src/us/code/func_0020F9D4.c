#include "common.h"
#include "m2c_macros.h"

void func_0020F9D4(void *arg0, void *arg1, void *arg2) {
    f32 temp_f1;
    f32 temp_f2;

    M2C_FIELD(arg2, f32 *, 0) = (f32) ((M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg1, f32 *, 0)) + (M2C_FIELD(arg0, f32 *, 0x10) * M2C_FIELD(arg1, f32 *, 4)) + (M2C_FIELD(arg0, f32 *, 0x20) * M2C_FIELD(arg1, f32 *, 8)) + M2C_FIELD(arg0, f32 *, 0x30));
    M2C_FIELD(arg2, f32 *, 4) = (f32) ((M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg1, f32 *, 0)) + (M2C_FIELD(arg0, f32 *, 0x14) * M2C_FIELD(arg1, f32 *, 4)) + (M2C_FIELD(arg0, f32 *, 0x24) * M2C_FIELD(arg1, f32 *, 8)) + M2C_FIELD(arg0, f32 *, 0x34));
    M2C_FIELD(arg2, f32 *, 8) = (f32) ((M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(arg1, f32 *, 0)) + (M2C_FIELD(arg0, f32 *, 0x18) * M2C_FIELD(arg1, f32 *, 4)) + (M2C_FIELD(arg0, f32 *, 0x28) * M2C_FIELD(arg1, f32 *, 8)) + M2C_FIELD(arg0, f32 *, 0x38));
    temp_f2 = (M2C_FIELD(arg0, f32 *, 0xC) * M2C_FIELD(arg1, f32 *, 0)) + (M2C_FIELD(arg0, f32 *, 0x1C) * M2C_FIELD(arg1, f32 *, 4)) + (M2C_FIELD(arg0, f32 *, 0x2C) * M2C_FIELD(arg1, f32 *, 8)) + M2C_FIELD(arg0, f32 *, 0x3C);
    if (temp_f2 != 0.0f) {
        temp_f1 = 1.0f / temp_f2;
        M2C_FIELD(arg2, f32 *, 0) = (f32) (M2C_FIELD(arg2, f32 *, 0) * temp_f1);
        M2C_FIELD(arg2, f32 *, 4) = (f32) (M2C_FIELD(arg2, f32 *, 4) * temp_f1);
        M2C_FIELD(arg2, f32 *, 8) = (f32) (M2C_FIELD(arg2, f32 *, 8) * temp_f1);
    }
}
