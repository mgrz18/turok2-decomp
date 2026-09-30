#include "common.h"
#include "m2c_macros.h"

f32 func_0020F680(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f1;
    f32 temp_f3;

    temp_f1 = M2C_FIELD(arg0, f32 *, 0) - M2C_FIELD(arg1, f32 *, 0);
    temp_f3 = M2C_FIELD(arg0, f32 *, 4) - M2C_FIELD(arg1, f32 *, 4);
    temp_f0 = M2C_FIELD(arg0, f32 *, 8) - M2C_FIELD(arg1, f32 *, 8);
    return (temp_f1 * temp_f1) + (temp_f3 * temp_f3) + (temp_f0 * temp_f0);
}
