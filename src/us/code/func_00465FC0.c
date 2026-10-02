#include "common.h"
#include "m2c_macros.h"

void func_00200500(s32, s32, s32);
s32 func_00200574(s32, s32);

void func_00465FC0(s32 arg0) {
    s32 var_s0;
    s32 var_s1;

    func_00200500(arg0 + 0x5300, 0x288, 0x28C);
    func_00200500(arg0 + 0x5314, 0x288, 0x28C);
    var_s1 = 0;
    var_s0 = arg0;
    do {
        func_00200574(arg0 + 0x5300, var_s0);
        var_s1 += 1;
        var_s0 += 0x298;
    } while (var_s1 < 0x20);
}
