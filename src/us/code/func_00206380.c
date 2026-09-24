#include "common.h"
#include "m2c_macros.h"

s32 func_00203068(s32, s32, s32);

void func_00206380(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = arg1;
    if (arg2 & 0x10) {
        var_v0 = 0;
    }
    func_00203068(0, 2, var_v0);
}
