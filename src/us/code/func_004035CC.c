#include "common.h"
#include "m2c_macros.h"

s32 func_004035CC(void **arg0, void **arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (M2C_FIELD(*arg0, f32 *, 0x24C) < M2C_FIELD(*arg1, f32 *, 0x24C)) {
        var_v0 = -1;
    }
    return var_v0;
}
