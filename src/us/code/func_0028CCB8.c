#include "common.h"

s32 func_0028CD68(u32);                         /* extern */

void func_0028CCB8(u32 arg0) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_0028CD68(arg0 >> ((3 - var_s0) * 8));
        var_s0 += 1;
    } while (var_s0 < 4);
}
