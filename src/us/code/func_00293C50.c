#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0029B020(M2C_UNK *, M2C_UNK *, M2C_UNK); /* extern */
extern M2C_UNK D_800AA488;
extern M2C_UNK D_800AA48C;
extern f64 D_800AA4A0;

s32 func_00293C50(s32 arg0, f32 arg1) {
    f64 temp_f0;
    s32 var_v0;

    if (arg1 == 0.0f) {
        func_0029B020(&D_800AA488, &D_800AA48C, 0x11D);
    }
    temp_f0 = (f64) ((f32) arg0 / arg1);
    var_v0 = 0x7FFFFFFF;
    if (!(D_800AA4A0 < temp_f0)) {
        var_v0 = (s32) temp_f0;
    }
    return var_v0;
}
