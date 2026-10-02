#include "common.h"
#include "m2c_macros.h"

void func_00468590(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0) = M2C_FIELD(arg0, s32 (**)(), 0x1C)();
    M2C_FIELD(arg0, s32 *, 0xC) = 0;
}
