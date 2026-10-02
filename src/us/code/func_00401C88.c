#include "common.h"
#include "m2c_macros.h"

void func_00401C88(void *arg0) {
    if ((u32) (M2C_FIELD(arg0, s32 *, 4) - 1) >= 2U) {
        M2C_FIELD(arg0, s32 *, 4) = 1;
    }
}
