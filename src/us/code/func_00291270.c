#include "common.h"
#include "m2c_macros.h"

void func_00291270(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 var_v1;
    u8 *var_a0;
    u8 *var_a1;
    u8 temp_v0;

    var_a0 = arg0;
    var_a1 = arg1;
    var_v1 = 0;
    if (arg2 > 0) {
        do {
            temp_v0 = *var_a0;
            var_a0 += 1;
            var_v1 += 1;
            *var_a1 = temp_v0;
            var_a1 += 1;
        } while (var_v1 < arg2);
    }
}
