#include "common.h"

s32 Crash_PrintHex8(s32);                         /* extern */

void Crash_PrintHex16(s32 arg0) {
    s32 var_s0;

    var_s0 = 0;
    do {
        Crash_PrintHex8((s32) (arg0 & 0xFFFF) >> ((1 - var_s0) * 8));
        var_s0 += 1;
    } while (var_s0 < 2);
}
