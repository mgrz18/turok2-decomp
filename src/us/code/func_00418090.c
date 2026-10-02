#include "common.h"
#include "m2c_macros.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00418090(s32 arg0, void *arg1) {
    void *temp_s0;

    temp_s0 = M2C_FIELD(M2C_FIELD(arg1, void **, 0x1C), void **, 0x518);
    M2C_FIELD(temp_s0, s8 *, 0x2B) = func_0041648C((s32) arg1, (s32) M2C_FIELD(temp_s0, s8 *, 0x2B), 1, 0, 1, 1);
    return 0;
}
