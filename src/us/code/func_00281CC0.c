#include "common.h"
#include "m2c_macros.h"

u32 func_0027BD30(s32);                             /* extern */

s32 func_00281CC0(s32 arg0) {
    s32 var_v1;
    u32 temp_s0;
    u32 temp_s1;
    u32 temp_v0;

    temp_s1 = func_0027BD30(arg0 + 0x18);
    temp_s0 = func_0027BD30(arg0 + 0x2C);
    temp_v0 = func_0027BD30(arg0 + 0x40);
    var_v1 = 0;
    if ((temp_s1 != 0) && (temp_s0 != 0)) {
        var_v1 = temp_v0 > 0U;
    }
    return var_v1;
}
