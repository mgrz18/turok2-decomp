#include "common.h"
#include "m2c_macros.h"

s32 func_00200574(s32, s32);
s32 func_00200738(s32, s32);

M2C_UNK func_0042F8D0(s32);                         /* extern */

s32 func_00468E78(void *arg0) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0xC);
    if (temp_s0 != 0) {
        func_0042F8D0(temp_s0);
        func_00200738((s32) (arg0 + 0xC), temp_s0);
        func_00200574((s32) (arg0 + 0x20), temp_s0);
    }
    return temp_s0;
}
