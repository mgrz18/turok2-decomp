#include "common.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Blend a colour toward its brightest channel by percent (0..100). */
void func_00228FF8(u8 percent, u8 r, u8 g, u8 b, u8 *outR, u8 *outG, u8 *outB) {
    s32 keep;
    s32 peak;

    if (percent != 0) {
        keep = 100 - percent;
        peak = MAX(r, MAX(g, b));
        *outR = (u32)((f32)(r * keep + peak * percent) * 0.01f);
        *outG = (u32)((f32)(g * keep + peak * percent) * 0.01f);
        *outB = (u32)((f32)(b * keep + peak * percent) * 0.01f);
    } else {
        *outR = r;
        *outG = g;
        *outB = b;
    }
}
