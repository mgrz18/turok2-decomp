#include "common.h"
#include "m2c_macros.h"

u32 CShakeAxis__Advance(s32);                             /* extern */

s32 CShake__Advance(s32 arg0) {
    s32 var_v1;
    u32 temp_s0;
    u32 temp_s1;
    u32 temp_v0;

    temp_s1 = CShakeAxis__Advance(arg0 + 0x18);
    temp_s0 = CShakeAxis__Advance(arg0 + 0x2C);
    temp_v0 = CShakeAxis__Advance(arg0 + 0x40);
    var_v1 = 0;
    if ((temp_s1 != 0) && (temp_s0 != 0)) {
        var_v1 = temp_v0 > 0U;
    }
    return var_v1;
}
