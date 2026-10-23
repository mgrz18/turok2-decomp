#include "common.h"

s32 func_0027BC34(void);
extern u32 D_800B6D20;
extern u32 D_800DD784; /* random seed A */

/* Random blend between a and b (t in [0, 1] from seed A, steps of 1/9999). */
f32 func_002119FC(f32 a, f32 b) {
    f32 t;

    if (func_0027BC34()) {
        D_800DD784 = D_800B6D20 * 0xA84A5B53 + 0x58348C2D;
    } else {
        D_800DD784 = D_800DD784 * 0xA84A5B53 + 0x58348C2D;
    }
    t = ((s32) (((D_800DD784 << 16) | (D_800DD784 >> 16)) & 0x7FFFFFFF) % 10000) * (1.0f / 9999.0f);
    return t * a + (1.0f - t) * b;
}
