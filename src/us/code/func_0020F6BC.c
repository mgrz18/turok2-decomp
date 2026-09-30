#include "common.h"
#include "m2c_macros.h"

f32 func_00298470(f32);

void func_0020F6BC(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;

    temp_f0 = M2C_FIELD(arg0, f32 *, 0) - M2C_FIELD(arg1, f32 *, 0);
    temp_f2 = M2C_FIELD(arg0, f32 *, 4) - M2C_FIELD(arg1, f32 *, 4);
    temp_f12 = M2C_FIELD(arg0, f32 *, 8) - M2C_FIELD(arg1, f32 *, 8);
    func_00298470((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f12 * temp_f12));
}
