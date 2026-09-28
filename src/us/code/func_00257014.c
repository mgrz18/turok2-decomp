#include "common.h"
#include "m2c_macros.h"

f32 func_00210EF0(s32);

extern f32 D_800B6D28;

f32 func_00257014(f32 arg0, f32 arg1, s32 arg2, f32 arg3) {
    f32 temp_f3;
    f32 var_f0;
    f32 var_f1;
    f32 var_f2;

    func_00210EF0((s32) &arg0);
    func_00210EF0((s32) &arg1);
    if (arg2 > 0) {
        if (arg1 < arg0) {
            var_f0 = arg1 + 6.283186f;
            goto block_5;
        }
    } else if (arg1 > arg0) {
        var_f0 = arg1 - 6.283186f;
block_5:
        arg1 = var_f0;
    }
    temp_f3 = arg1 - arg0;
    var_f1 = temp_f3 * arg3 * D_800B6D28;
    var_f2 = var_f1;
    if (var_f1 < 0.0f) {
        var_f2 = -var_f1;
    }
    if (temp_f3 < 0.0f) {
        if (!(-temp_f3 < var_f2)) {

        } else {
            goto block_12;
        }
    } else if (temp_f3 < var_f2) {
block_12:
        var_f1 = temp_f3;
    }
    return var_f1;
}
