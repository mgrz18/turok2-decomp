#include "common.h"
#include "m2c_macros.h"

s32 func_0023CEC0(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f1;
    s32 var_v0;

    temp_f1 = M2C_FIELD(arg0, f32 *, 0) - M2C_FIELD(arg1, f32 *, 0);
    temp_f0 = M2C_FIELD(arg0, f32 *, 8) - M2C_FIELD(arg1, f32 *, 8);
    var_v0 = 1;
    if (!(((temp_f1 * temp_f1) + (temp_f0 * temp_f0)) <= M2C_FIELD(arg0, f32 *, 0x10))) {
        var_v0 = 0;
    }
    return var_v0;
}
