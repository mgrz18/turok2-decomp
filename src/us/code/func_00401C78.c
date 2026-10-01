#include "common.h"
#include "m2c_macros.h"

s32 func_00401C78(void *arg0) {
    return (u32) (M2C_FIELD(arg0, s32 *, 4) - 1) < 2U;
}
