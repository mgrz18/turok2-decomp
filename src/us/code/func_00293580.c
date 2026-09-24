#include "common.h"
#include "m2c_macros.h"

s32 func_00293580(void *arg0) {
    return M2C_FIELD(((M2C_FIELD(arg0, s32 *, 0x3C) * 0x30) + M2C_FIELD(arg0, s32 *, 0x40)), s32 *, 0x28);
}
