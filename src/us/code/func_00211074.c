#include "common.h"
#include "m2c_macros.h"

f32 func_00298470(f32);

void func_00211074(void *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f2;

    temp_f12 = M2C_FIELD(arg0, f32 *, 0);
    temp_f2 = M2C_FIELD(arg0, f32 *, 4);
    temp_f1 = M2C_FIELD(arg0, f32 *, 8);
    temp_f0 = M2C_FIELD(arg0, f32 *, 0xC);
    temp_f0_2 = func_00298470((temp_f12 * temp_f12) + (temp_f2 * temp_f2) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0));
    if (temp_f0_2 != 0.0f) {
        temp_f1_2 = 1.0f / temp_f0_2;
        M2C_FIELD(arg0, f32 *, 0) = (f32) (M2C_FIELD(arg0, f32 *, 0) * temp_f1_2);
        M2C_FIELD(arg0, f32 *, 4) = (f32) (M2C_FIELD(arg0, f32 *, 4) * temp_f1_2);
        M2C_FIELD(arg0, f32 *, 8) = (f32) (M2C_FIELD(arg0, f32 *, 8) * temp_f1_2);
        M2C_FIELD(arg0, f32 *, 0xC) = (f32) (M2C_FIELD(arg0, f32 *, 0xC) * temp_f1_2);
    }
}
