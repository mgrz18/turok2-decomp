#include "common.h"
#include "m2c_macros.h"

void func_00264960(void *arg0, s32 *arg1) {
    *arg1 |= 0x10000000;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
}
