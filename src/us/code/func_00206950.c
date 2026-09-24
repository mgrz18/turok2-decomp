#include "common.h"
#include "m2c_macros.h"

s32 func_00206950(void *arg0, s32 arg1) {
    s32 var_a2;

    var_a2 = 0;
    if (M2C_FIELD(arg0, s32 *, 8) >= arg1) {
        var_a2 = M2C_FIELD(arg0, s32 *, 0xC);
        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (var_a2 + (arg1 << 6));
        M2C_FIELD(arg0, s32 *, 8) = (s32) (M2C_FIELD(arg0, s32 *, 8) - arg1);
    }
    return var_a2;
}
