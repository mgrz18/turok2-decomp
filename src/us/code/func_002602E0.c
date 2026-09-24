#include "common.h"
#include "m2c_macros.h"

void func_002602E0(s32 arg0, void *arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = M2C_FIELD(arg1, u8 *, 0xC8) - arg2;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    M2C_FIELD(arg1, u8 *, 0xC8) = (u8) var_v0;
}
