#include "common.h"
#include "m2c_macros.h"

s32 func_002571D8(void *arg0, void *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (M2C_FIELD(arg0, f32 *, 0xC) < M2C_FIELD(arg1, f32 *, 0xC)) {
        var_v0 = -1;
    }
    return var_v0;
}
