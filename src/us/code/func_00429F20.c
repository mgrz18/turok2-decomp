#include "common.h"
#include "m2c_macros.h"

s32 func_00206920(s32);
s32 func_00297AE0(s32);

void func_00429F20(void *arg0, s32 arg1, s32 arg2) {
    s32 var_s0;
    s32 var_s1;

    var_s1 = 0;
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = (s32) (arg2 / 2);
    if (arg2 > 0) {
        var_s0 = arg1;
        do {
            func_00297AE0(var_s0);
            var_s1 += 1;
            var_s0 += 0x40;
        } while (var_s1 < arg2);
    }
    func_00206920((s32) arg0);
}
