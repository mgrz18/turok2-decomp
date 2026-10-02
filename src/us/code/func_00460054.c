#include "common.h"
#include "m2c_macros.h"

s32 func_00460054(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    if ((u32) (var_a0 - 0x61) < 0x1AU) {
        var_a0 -= 0x20;
    }
    return var_a0;
}
