#include "common.h"
#include "m2c_macros.h"

s32 func_00271778(void *arg0, s32 arg1) {
    return M2C_FIELD(arg0, s32 *, 0x2B70) + (arg1 * 0xC);
}
