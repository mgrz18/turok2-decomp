#include "common.h"
#include "m2c_macros.h"

u8 *func_0029E460(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 var_a2;
    u8 *var_a1;
    u8 *var_v1;
    u8 temp_v0;

    var_a1 = arg1;
    var_a2 = arg2;
    var_v1 = arg0;
    if (var_a2 != 0) {
        do {
            temp_v0 = *var_a1;
            var_a1 += 1;
            var_a2 -= 1;
            *var_v1 = temp_v0;
            var_v1 += 1;
        } while (var_a2 != 0);
    }
    return arg0;
}
