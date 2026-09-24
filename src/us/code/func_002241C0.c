#include "common.h"
#include "m2c_macros.h"

u16 func_002241C0(void *arg0, s32 arg1) {
    return M2C_FIELD((M2C_FIELD(arg0, s32 *, 0x74) + ((arg1 << 1))), u16 *, 8);
}
