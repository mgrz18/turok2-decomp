#include "common.h"
#include "m2c_macros.h"

void CList__Construct(s32, s32, s32);
s32 CList__AddTail(s32, s32);

void func_0042FFF0(s32 arg0) {
    s32 var_s0;
    s32 var_s1;

    CList__Construct(arg0 + 0x8B4, 0, 4);
    CList__Construct(arg0 + 0x8A0, 0, 4);
    var_s1 = 0;
    var_s0 = arg0;
    do {
        CList__AddTail(arg0 + 0x8A0, var_s0);
        var_s1 += 1;
        var_s0 += 0x228;
    } while (var_s1 < 4);
    M2C_FIELD(arg0, s16 *, 0x8C8) = 0;
}
