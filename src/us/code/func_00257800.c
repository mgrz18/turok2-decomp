#include "common.h"
#include "m2c_macros.h"

void func_00257800(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s32 *, 0x1C) = 0;
    M2C_FIELD(arg0, s32 *, 0x20) = 0;
    M2C_FIELD(arg0, s32 *, 0x1298) = 0;
    M2C_FIELD(arg0, s32 *, 0xC04) = 1;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & 0xFF7FFFFF);
}
