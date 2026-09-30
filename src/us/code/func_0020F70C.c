#include "common.h"
#include "m2c_macros.h"

f32 func_00298470(f32);

void func_0020F70C(void *arg0, f32 arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f1;
    f32 temp_f2;

    temp_f2 = M2C_FIELD(arg0, f32 *, 0);
    temp_f1 = M2C_FIELD(arg0, f32 *, 4);
    temp_f0 = M2C_FIELD(arg0, f32 *, 8);
    temp_f12 = (temp_f2 * temp_f2) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0);
    if ((arg1 * arg1) < temp_f12) {
        temp_f0_2 = arg1 / func_00298470(temp_f12);
        M2C_FIELD(arg0, f32 *, 0) = (f32) (M2C_FIELD(arg0, f32 *, 0) * temp_f0_2);
        M2C_FIELD(arg0, f32 *, 4) = (f32) (M2C_FIELD(arg0, f32 *, 4) * temp_f0_2);
        M2C_FIELD(arg0, f32 *, 8) = (f32) (M2C_FIELD(arg0, f32 *, 8) * temp_f0_2);
    }
}
