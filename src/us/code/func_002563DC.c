#include "common.h"
#include "m2c_macros.h"

void func_002563DC(void *arg0) {
    void *temp_v0;

    temp_v0 = arg0 + 0xCD0;
    M2C_FIELD(arg0, s32 *, 0xCD0) = 0;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(temp_v0, s32 *, 0x37C) = -1;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0x1284) = 0;
}
