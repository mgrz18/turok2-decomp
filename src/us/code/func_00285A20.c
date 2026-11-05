#include "common.h"
#include "m2c_macros.h"

extern u8 D_800F5EA0[];

s32 func_00285A20(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_800F5EA0[i] != 0) {
            return 1;
        }
    }
    return 0;
}
