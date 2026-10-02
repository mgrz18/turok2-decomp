#include "common.h"
#include "m2c_macros.h"

s32 func_00200574(s32, s32);
s32 func_00200738(s32, s32);
s32 func_0042F9B0(s32);

s32 func_0042FF58(void *arg0) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0xC);
    if (temp_s0 != 0) {
        func_0042F9B0(temp_s0);
        func_00200738((s32) (arg0 + 0xC), temp_s0);
        func_00200574((s32) (arg0 + 0x20), temp_s0);
    }
    return temp_s0;
}
