#include "common.h"
#include "m2c_macros.h"

s32 func_00256D44(void *arg0) {
    s32 temp_a0;
    s32 var_a1;

    var_a1 = 0;
    if (M2C_FIELD(arg0, s16 *, 0xA08) == 3) {
        temp_a0 = M2C_FIELD(arg0, s32 *, 0xC04);
        if (temp_a0 >= 0x7DF) {
            var_a1 = temp_a0 < 0x7E4;
        }
    }
    return var_a1;
}
