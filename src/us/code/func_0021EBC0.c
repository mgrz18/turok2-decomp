#include "common.h"
#include "m2c_macros.h"

void func_0021EBC0(void *arg0) {
    s32 temp_a1;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x98);
    M2C_FIELD(arg0, s32 *, 0x98) = 0;
    M2C_FIELD(arg0, s32 *, 0xA0) = 0;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & ~0x200);
    M2C_FIELD(arg0, s32 *, 0x9C) = temp_a1;
}
