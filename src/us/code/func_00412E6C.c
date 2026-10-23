#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

void func_00412E6C(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;
    s32 temp_a3;
    u8 *temp_v0;
    u8 *temp_v1;

    temp_a3 = arg1 >> 3;
    temp_a0 = 1 << (arg1 & 7);
    if (arg2 != 0) {
        temp_v1 = arg0 + temp_a3;
        *temp_v1 |= temp_a0;
        return;
    }
    temp_v0 = arg0 + temp_a3;
    *temp_v0 &= ~temp_a0;
}
