#include "common.h"
#include "m2c_macros.h"

extern f32 D_800C0AE0;

f32 func_00412FF8(f32 arg0, f32 arg1, f32 arg2) {
    f32 temp_f1;

    temp_f1 = D_800C0AE0 - arg2;
    return (arg0 * ((temp_f1 * temp_f1 * temp_f1) - temp_f1)) + (arg1 * ((arg2 * arg2 * arg2) - arg2));
}
