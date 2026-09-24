#include "common.h"
#include "m2c_macros.h"

void func_002018F0(void *arg0, void **arg1, s32 *arg2) {
    *arg1 = arg0;
    *arg2 = (M2C_FIELD(arg0, s32 *, 0) * M2C_FIELD(arg0, s32 *, 4)) + 8;
}
