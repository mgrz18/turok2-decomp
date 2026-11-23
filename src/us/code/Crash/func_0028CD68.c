#include "common.h"

void Crash_PutChar(s32);                         /* extern */

void Crash_PrintHex8(s32 arg0) {
    s32 temp_a0;
    s32 temp_s0;
    s32 var_a0;
    s32 var_a0_2;

    temp_s0 = arg0 & 0xF;
    temp_a0 = (arg0 >> 4) & 0xF;
    if (temp_a0 >= 0xA) {
        var_a0 = temp_a0 + 0x57;
    } else {
        var_a0 = temp_a0 | 0x30;
    }
    Crash_PutChar(var_a0);
    var_a0_2 = temp_s0 | 0x30;
    if (temp_s0 >= 0xA) {
        var_a0_2 = temp_s0 + 0x57;
    }
    Crash_PutChar(var_a0_2);
}
