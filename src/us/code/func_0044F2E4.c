#include "common.h"
#include "m2c_macros.h"

s32 func_0044F2E4(void *arg0) {
    s32 temp_v1;
    s32 var_v0;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x20);
    var_v0 = 0;
    if (temp_v1 != 0) {
        var_v0 = temp_v1 + 0xA0;
    }
    return var_v0;
}
