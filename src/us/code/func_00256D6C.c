#include "common.h"
#include "m2c_macros.h"

s32 func_00256D6C(void *arg0) {
    return M2C_FIELD(arg0, s16 *, 0xA08) == 0xE;
}
