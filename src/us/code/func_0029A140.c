#include "common.h"
#include "m2c_macros.h"

s32 __osSiDeviceBusy(void) {
    register u32 stat = *(volatile u32 *)0xA4800018;

    if (stat & 3) {
        return 1;
    } else {
        return 0;
    }
}
