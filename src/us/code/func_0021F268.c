#include "common.h"
#include "m2c_macros.h"

f32 func_00298470(f32);

void func_0021F268(void *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f1;

    temp_f12 = M2C_FIELD(arg0, f32 *, 0x34);
    temp_f1 = M2C_FIELD(arg0, f32 *, 0x38);
    temp_f0 = M2C_FIELD(arg0, f32 *, 0x3C);
    func_00298470(((temp_f12 * temp_f12) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0)) * 0.33333334f);
}
