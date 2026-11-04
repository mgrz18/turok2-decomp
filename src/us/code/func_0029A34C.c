#include "common.h"

s32 __osSpDeviceBusy();                                /* extern */
s32 __osSpSetStatus(s32);                     /* extern */

void func_0029A34C(void) {
    do {

    } while (__osSpDeviceBusy() != 0);
    __osSpSetStatus(0x125);
}
