#include "common.h"
#include "m2c_macros.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00417F50(s32 arg0, void *arg1) {
    void *temp_s0;

    temp_s0 = M2C_FIELD(M2C_FIELD(arg1, void **, 0x1C), void **, 0x518);
    M2C_FIELD(temp_s0, u8 *, 0x32) = func_0041648C((s32) arg1, (s32) M2C_FIELD(temp_s0, u8 *, 0x32), 1, 0, 0xA, 0);
    return 0;
}
