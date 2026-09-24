#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002933E0(s32, void *);                 /* extern */

void func_00290F20(void *arg0, s32 arg1, s32 arg2) {
    s32 var_s0;
    s32 var_s1;

    var_s1 = 0;
    M2C_FIELD(arg0, s32 *, 0x10) = 0;
    M2C_FIELD(arg0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0xC) = 0;
    M2C_FIELD(arg0, s32 *, 0) = 0;
    M2C_FIELD(arg0, s32 *, 4) = 0;
    if (arg2 > 0) {
        var_s0 = arg1;
        do {
            func_002933E0(var_s0, arg0);
            var_s1 += 1;
            var_s0 += 0x1C;
        } while (var_s1 < arg2);
    }
}
