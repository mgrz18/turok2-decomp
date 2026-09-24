#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200574(void *, s32);                 /* extern */
M2C_UNK func_00200738(void *, s32);                 /* extern */

s32 func_002836E4(void *arg0) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x8A0);
    if (temp_s0 != 0) {
        func_00200738(arg0 + 0x8A0, temp_s0);
        func_00200574(arg0 + 0x8B4, temp_s0);
    }
    return temp_s0;
}
