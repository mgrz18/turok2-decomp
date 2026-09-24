#include "common.h"
#include "m2c_macros.h"

s32 func_00265ED8(void *arg0) {
    return (((u32) M2C_FIELD(arg0, u32 *, 0xC) >> 0x16) ^ 1) & 1;
}
