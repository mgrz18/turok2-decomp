#include "common.h"
#include "m2c_macros.h"

s32 func_00298650(void) {
    register s32 status = *(volatile u32 *)0xA450000C;

    if (status & 0x80000000) {
        return 1;
    } else {
        return 0;
    }
}
