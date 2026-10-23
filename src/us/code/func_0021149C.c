#include "common.h"

s32 func_0027BC34(void);
extern u32 D_800B6D20;
extern u32 D_800DD784; /* random seed A */

/* LCG step on seed A, scaled to a float in [0, scale]. */
f32 func_0021149C(f32 scale) {
    if (func_0027BC34()) {
        D_800DD784 = D_800B6D20 * 0xA84A5B53 + 0x58348C2D;
    } else {
        D_800DD784 = D_800DD784 * 0xA84A5B53 + 0x58348C2D;
    }
    return (f32) ((D_800DD784 >> 16) & 0x7FFF) * scale * (1.0f / 32767.0f);
}
