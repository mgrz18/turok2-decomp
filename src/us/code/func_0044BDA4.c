#include "common.h"
#include "m2c_macros.h"

s32 *func_002017D4(s32, M2C_UNK);                   /* extern */
extern void *D_800C1BB0;

s32 func_0044BDA4(void) {
    return *func_002017D4(M2C_FIELD(D_800C1BB0, s32 *, 4), 0);
}
