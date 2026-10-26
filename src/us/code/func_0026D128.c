#include "common.h"

/* Two floats packed as their upper 16 bits (see func_0026D200 / func_0026D210). */
typedef struct {
    /* 0x0 */ u16 base;
    /* 0x2 */ u16 range;
} PackedRange;

s32 func_0027BC34(void);
extern u32 D_800B6D20;
extern u32 D_800DD784; /* random seed A */

/* base + random(0..1) * range, both unpacked from half-floats. */
f32 func_0026D128(PackedRange r) {
    u32 base = r.base << 16;
    u32 range;

    if (r.range == 0) {
        return *(f32 *) &base;
    }
    range = r.range << 16;
    if (func_0027BC34()) {
        D_800DD784 = D_800B6D20 * 0xA84A5B53 + 0x58348C2D;
    } else {
        D_800DD784 = D_800DD784 * 0xA84A5B53 + 0x58348C2D;
    }
    return *(f32 *) &base + (f32) ((D_800DD784 >> 16) & 0x7FFF) * *(f32 *) &range * (1.0f / 32767);
}
