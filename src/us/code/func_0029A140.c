#include "common.h"
#include "m2c_macros.h"

s32 func_0029A140(void) {
    register u32 stat = *(volatile u32 *)0xA4800018;

    if (stat & 3) {
        return 1;
    } else {
        return 0;
    }
}
