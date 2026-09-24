#include "common.h"
#include "m2c_macros.h"

s32 func_002680EC(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x48) & 0x400;
}
