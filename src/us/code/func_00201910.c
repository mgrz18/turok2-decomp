#include "common.h"
#include "m2c_macros.h"

void *func_00201910(void *arg0, s32 *arg1) {
    *arg1 = M2C_FIELD(arg0, s32 *, 0) * M2C_FIELD(arg0, s32 *, 4);
    return arg0 + 8;
}
