#include "common.h"
#include "m2c_macros.h"

void func_0025360C(void *arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 4) == arg1) {
        M2C_FIELD(arg0, s32 *, 0xB4) = 1;
    }
    M2C_FIELD(arg0, s32 *, 4) = arg1;
}
