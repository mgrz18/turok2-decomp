#include "common.h"
#include "m2c_macros.h"

void func_00256878(void *arg0, void *arg1, s32 arg2, s32 *arg3) {
    if (M2C_FIELD(arg1, s32 *, 0xBF4) != M2C_FIELD(arg0, s32 *, 0xBF4)) {
        *arg3 += 1;
    }
}
