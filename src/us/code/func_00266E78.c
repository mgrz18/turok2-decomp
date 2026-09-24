#include "common.h"
#include "m2c_macros.h"

s32 func_00266E78(s32 arg0) {
    s32 var_v0;

    var_v0 = arg0 + 7;
    if (var_v0 < 0) {
        var_v0 = arg0 + 0xE;
    }
    return var_v0 >> 3;
}
