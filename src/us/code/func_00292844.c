#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002929A0(void *, s32);                 /* extern */
M2C_UNK func_00292A20(void *, s32, s32);            /* extern */

void func_00292844(void *arg0, void *arg1) {
    s32 temp_s1;
    s32 var_s0;
    void *var_v0;

    var_v0 = arg1;
    do {
        temp_s1 = M2C_FIELD(var_v0, s32 *, 0xC);
        var_v0 += 4;
    } while (temp_s1 == 0);
    var_s0 = 0;
    if (M2C_FIELD(arg0, u8 *, 0x34) != 0) {
        do {
            func_002929A0(arg0, var_s0);
            func_00292A20(arg0, temp_s1, var_s0);
            var_s0 += 1;
        } while (var_s0 < (s32) M2C_FIELD(arg0, u8 *, 0x34));
    }
    if (M2C_FIELD(arg1, s32 *, 8) != 0) {
        func_002929A0(arg0, var_s0);
        func_00292A20(arg0, M2C_FIELD(arg1, s32 *, 8), 9);
    }
}
