#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, M2C_UNK);                    /* extern */
M2C_UNK func_00201818(s32, M2C_UNK);                /* extern */

void func_002267A0(void *arg0, s32 arg1, M2C_UNK arg2, M2C_UNK arg3) {
    s32 temp_v0;
    s32 var_a0;

    if (arg1 != 0) {
        var_a0 = M2C_FIELD(arg0, s32 *, 0x6C);
    } else {
        var_a0 = M2C_FIELD(arg0, s32 *, 0x70);
    }
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(var_a0, 0), arg3), arg2);
    func_002017D4(temp_v0, 0);
    func_00201818(temp_v0, 1);
    func_002017D4(temp_v0, 1);
}
