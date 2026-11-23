#include "common.h"
#include "protos/ultra.h"

s32 __osSpDeviceBusy();                                /* extern */
extern void __osSpSetStatus(u32 arg0);

void osSpTaskStartGo(OSTask *tp) {
    do {

    } while (__osSpDeviceBusy() != 0);
    __osSpSetStatus(0x125);
}
