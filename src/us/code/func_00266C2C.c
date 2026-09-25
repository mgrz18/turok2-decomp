#include "common.h"
#include "m2c_macros.h"

void func_00266C2C(s32 arg0, s32 arg1, s32 arg2) {
    s32 *var_a0;
    s32 *var_a1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a2;

    var_a2 = arg2;
    temp_v0 = (var_a2 * 4) - 4;
    var_a0 = arg0 + temp_v0;
    var_a1 = arg1 + temp_v0;
    do {
        temp_v0_2 = *var_a1;
        var_a1 = (__typeof__(var_a1))((s8 *)var_a1 - 4);
        var_a2 -= 1;
        *var_a0 = temp_v0_2;
        var_a0 = (__typeof__(var_a0))((s8 *)var_a0 - 4);
    } while (var_a2 != 0);
}
