#include "common.h"
#include "m2c_macros.h"

void func_00293238(void *arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 0) = (s32) M2C_FIELD(arg0, s32 *, 8);
    M2C_FIELD(arg1, u16 *, 0xC) = (u16) M2C_FIELD(arg0, u16 *, 0x1A);
    M2C_FIELD(arg1, s32 *, 4) = (s32) M2C_FIELD(arg0, s32 *, 0xC);
}
