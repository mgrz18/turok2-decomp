#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00207914(void *, s32);                 /* extern */
M2C_UNK func_002316A0(s32, void *);                 /* extern */

void func_0023685C(s32 arg0, void *arg1, s32 arg2) {
    s32 temp_a1;
    void *temp_s0;

    temp_a1 = M2C_FIELD(arg1, s32 *, 0x34);
    if (temp_a1 != -1) {
        temp_s0 = arg1 + 0x14;
        func_00207914(temp_s0, temp_a1);
        if (arg2 != 0) {
            func_002316A0(arg2, temp_s0);
        }
    }
}
