#include "common.h"

s32 Crash_PrintHex8(u32);                         /* extern */

void Crash_PrintHex32(u32 arg0) {
    s32 var_s0;

    var_s0 = 0;
    do {
        Crash_PrintHex8(arg0 >> ((3 - var_s0) * 8));
        var_s0 += 1;
    } while (var_s0 < 4);
}
