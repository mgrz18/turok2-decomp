#include "common.h"
#include "m2c_macros.h"

s32 func_00204D90(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0xC) & 0x100;
}
