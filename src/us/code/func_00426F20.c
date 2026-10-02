#include "common.h"
#include "m2c_macros.h"

u8 *func_00426F20(u8 *arg0, u8 *arg1) {
    u8 *var_a1;
    u8 *var_v1;
    u8 temp_v0;

    var_a1 = arg1;
    var_v1 = arg0;
    if (*arg0 != 0) {
        do {
            var_v1 += 1;
        } while (*var_v1 != 0);
    }
    do {
        temp_v0 = *var_a1;
        var_a1 += 1;
        *var_v1 = temp_v0;
        var_v1 += 1;
    } while (temp_v0 & 0xFF);
    return arg0;
}
