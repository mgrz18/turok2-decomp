#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A71D8;

f32 func_00256584(f32 arg0) {
    f32 temp_f12;

    temp_f12 = D_800A71D8 - arg0;
    return D_800A71D8 - (temp_f12 * temp_f12 * temp_f12);
}
