#include "common.h"
#include "m2c_macros.h"

f32 func_00410BB4(f32);
extern void *D_800C1BB0;

f32 func_00284390(void) {
    f32 value;

    if (M2C_FIELD(D_800C1BB0, s32 *, 0x38) != 0) {
        value = func_00410BB4(M2C_FIELD(D_800C1BB0, f32 *, 0x1C));
    } else {
        value = 47.5f;
    }
    return value;
}
