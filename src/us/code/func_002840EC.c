#include "common.h"
#include "m2c_macros.h"

s32 func_00284144();                                /* extern */
s32 func_00284174();                                /* extern */
extern void *D_800C1BB0;

void func_002840EC(void) {
    if ((func_00284174() != 0) && (func_00284144() != 0) && (M2C_FIELD(D_800C1BB0, s32 *, 0x60) == 0)) {
        M2C_FIELD(D_800C1BB0, s32 *, 0x60) = 1;
        M2C_FIELD(D_800C1BB0, f32 *, 0x64) = 1.0f;
    }
}
