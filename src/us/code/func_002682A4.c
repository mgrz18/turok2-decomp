#include "common.h"
#include "m2c_macros.h"

s32 func_002682A4(void *arg0) {
    return ((u32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), u32 *, 4) >> 2) & 1;
}
