#include "common.h"
#include "m2c_macros.h"

s32 func_00298DF0(void) {
    register u32 stat = *(volatile u32 *)0xA410000C;

    if (stat & 0x100) {
        return 1;
    } else {
        return 0;
    }
}
