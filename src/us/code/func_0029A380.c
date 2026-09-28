#include "common.h"
#include "m2c_macros.h"

s32 func_0029A380(void) {
    register u32 stat = *(volatile u32 *)0xA4040010;

    if (stat & 0x1C) {
        return 1;
    } else {
        return 0;
    }
}
