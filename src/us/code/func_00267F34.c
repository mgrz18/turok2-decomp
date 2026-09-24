#include "common.h"
#include "m2c_macros.h"

s32 func_00267F34(void *arg0, void *arg1) {
    s32 var_v1;

    var_v1 = 0;
    if ((M2C_FIELD(arg0, s32 *, 0x144) <= 0) || (M2C_FIELD(arg1, s32 *, 0x48) & 2)) {
        var_v1 = 1;
    }
    return var_v1;
}
