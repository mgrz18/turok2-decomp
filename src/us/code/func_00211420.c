#include "common.h"

s32 func_0027BC34(void);
extern u32 D_800B6D20;
extern u32 D_800DD788; /* random seed B */

/* LCG step on seed B (re-seeded from D_800B6D20 when func_0027BC34()); returns 0..0x7FFFFFFF. */
s32 func_00211420(void) {
    if (func_0027BC34()) {
        D_800DD788 = D_800B6D20 * 0xA84A5B53 + 0x58348C2D;
    } else {
        D_800DD788 = D_800DD788 * 0xA84A5B53 + 0x58348C2D;
    }
    return ((D_800DD788 << 16) | (D_800DD788 >> 16)) & 0x7FFFFFFF;
}
