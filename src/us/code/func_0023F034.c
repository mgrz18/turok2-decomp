#include "common.h"
#include "m2c_macros.h"

s32 func_0023F034(s32 arg0, void *arg1) {
    s32 r = 0;

    if (M2C_FIELD(arg1, u8 *, 0) == 1) {
        r = (u32) r < (u32) (M2C_FIELD(arg1, s32 *, 0xD4) & 0x300000);
    }
    return r;
}
