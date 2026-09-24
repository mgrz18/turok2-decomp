#include "common.h"
#include "m2c_macros.h"

s32 func_002565DC(void *arg0) {
    s32 var_v0;

    var_v0 = 1;
    if (!(M2C_FIELD(arg0, f32 *, 0xAC0) > 5.12f)) {
        var_v0 = 0;
    }
    return var_v0;
}
