#include "common.h"
#include "m2c_macros.h"

s32 func_0029A560(void);

s32 func_0029A510(void *arg0) {
    s32 temp_a0;
    u32 temp_v0;

    temp_v0 = func_0029A560();
    temp_a0 = temp_v0 >> 8;
    temp_a0 &= 1;
    if (temp_v0 & 0x80) {
        M2C_FIELD(arg0, s32 *, 4) = (s32) ((M2C_FIELD(arg0, s32 *, 4) | temp_a0) & ~2);
    }
    return temp_a0;
}
