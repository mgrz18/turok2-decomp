#include "common.h"
#include "m2c_macros.h"

s32 func_00275C6C(s32, s32, s32, s32, s32);

M2C_UNK func_00232B40(void *);                      /* extern */
s32 func_00275F7C();                                /* extern */

void func_00232AC4(void *arg0, s32 arg1) {
    s32 var_s1;
    void *temp_a2;
    void *temp_v1;
    void *var_s0;

    var_s0 = arg0;
    temp_v1 = M2C_FIELD(var_s0, void **, 0x1B8);
    var_s1 = arg1;
    if (temp_v1 != NULL) {
        if ((M2C_FIELD(temp_v1, s32 *, 0xC) != var_s1) || (M2C_FIELD(temp_v1, s32 *, 8) == -1)) {
            func_00232B40(var_s0);
            goto block_4;
        }
    } else {
block_4:
        temp_a2 = var_s0 + 4;
        M2C_FIELD(var_s0, void **, 0x1B8) = func_00275C6C(func_00275F7C(), var_s1, (s32) temp_a2, (s32) temp_a2, -1);
    }
}
