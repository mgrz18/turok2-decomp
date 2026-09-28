#include "common.h"
#include "m2c_macros.h"

extern u8 *D_800AF618[];

s32 func_00256B34(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    u8 *p = D_800AF618[arg1];

    for (i = 0; i < 3; i++) {
        u8 *e = ((u8 **)(p + 0x20))[i];

        if (e != NULL) {
            if (M2C_FIELD(e, s16 *, 4) == arg2) {
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}
