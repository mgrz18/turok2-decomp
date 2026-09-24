#include "common.h"
#include "m2c_macros.h"

s32 func_00267E68(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x14) & 0x200;
}
