#include "common.h"
#include "m2c_macros.h"

void func_0021E980(void *arg0) {
    M2C_FIELD(arg0, s8 *, 0xE3) = 1;
    M2C_FIELD(arg0, s32 *, 0xD8) = 0;
    M2C_FIELD(arg0, s8 *, 0xE2) = 0;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & ~0x400);
}
