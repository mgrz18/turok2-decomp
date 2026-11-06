#include "common.h"

s32 Crash_PutChar(u8);                          /* extern */

void Crash_PrintString(u8 *arg0) {
    u8 *var_s0;
    u8 temp_a0;

    var_s0 = arg0;
    if (*var_s0 != 0) {
        do {
            temp_a0 = *var_s0;
            var_s0 += 1;
            Crash_PutChar(temp_a0);
        } while (*var_s0 != 0);
    }
}
