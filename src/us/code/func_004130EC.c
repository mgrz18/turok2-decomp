#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, s32);

extern void *D_800C1BB0;

s32 func_004130EC(s32 arg0) {
    return M2C_FIELD(func_002017D4(func_002017D4(func_002017D4(M2C_FIELD(D_800C1BB0, s32 *, 4), 0), arg0), 2), s32 *, 4);
}
