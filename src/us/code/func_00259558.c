#include "common.h"
#include "m2c_macros.h"

s32 func_00259558(s32 arg0, void *arg1) {
    f32 temp_f0;
    s32 var_v0;

    var_v0 = 0x4F1A;
    temp_f0 = -M2C_FIELD(arg1, f32 *, 0x114);
    M2C_FIELD(arg1, f32 *, 0x114) = temp_f0;
    if (!(temp_f0 > 0.0f)) {
        var_v0 = 0x4F24;
    }
    return var_v0;
}
