#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, s32);
s32 func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_002051F4(s32, s32);

extern M2C_UNK D_800A5AC4;

s32 func_0021F5B8(void *arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_v0;

    if (M2C_FIELD(arg0, s32 *, 0xD4) & 0x40000) {
        temp_v0 = func_0020367C(0, M2C_FIELD(arg0, s32 *, 0xA4), M2C_FIELD(arg0, s32 *, 0xB0), 4, 0, 0, (s32) &D_800A5AC4, 1);
        if (temp_v0 != 0) {
            temp_s0 = M2C_FIELD((func_002017D4(*(s32 *)temp_v0, 1) + ((arg1 << 2))), s32 *, 8);
            func_002051F4(0, temp_v0);
            return temp_s0;
        }
    }
    return -1;
}
