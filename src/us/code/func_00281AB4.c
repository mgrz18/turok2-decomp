#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200574(s32, s32);                    /* extern */
M2C_UNK func_00200738(void *, s32);                 /* extern */
M2C_UNK func_00281B58(s32);                         /* extern */

s32 func_00281AB4(void *arg0, s32 arg1) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0xF0C);
    if (temp_s0 != 0) {
        func_00281B58(temp_s0);
        func_00200738(arg0 + 0xF0C, temp_s0);
        func_00200574(arg1 + 0xE28, temp_s0);
    }
    return temp_s0;
}
