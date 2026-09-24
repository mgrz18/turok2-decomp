#include "common.h"
#include "m2c_macros.h"

extern void *D_800C1BB0;

s32 func_00284144(void) {
    return M2C_FIELD(D_800C1BB0, s32 *, 0xE0) == 0x50;
}
