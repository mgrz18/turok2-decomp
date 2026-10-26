#include "common.h"

typedef struct {
    f32 r, g, b;
} ColorF;

#define FLOOR(x) ((x) >= 0.0f ? (s32) (x) : (s32) (x) - ((f32) (s32) (x) != (x)))

static __inline__ f32 floorF(f32 x) {
    return x >= 0.0f ? (s32) x : (s32) x - ((f32) (s32) x != x);
}

/* x modulo range, in 0..range. */
static __inline__ f32 wrap(f32 x, f32 range) {
    x /= range;
    return (x - floorF(x)) * range;
}

/* HSV to RGB, every component in 0..1 (hue wraps). */
ColorF func_00211DB0(f32 h, f32 s, f32 v) {
    ColorF c;
    f32 r;
    f32 g;
    f32 b;
    f32 f;
    f32 p;
    f32 q;
    f32 t;
    s32 i;

    h = wrap(h * 6.0f, 6.0f);
    i = FLOOR(h);
    f = h - i;
    p = v * (1.0f - s);
    q = v * (1.0f - s * f);
    t = v * (1.0f - s * (1.0f - f));
    b = g = r = 0.0f;
    switch (i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
    }
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}
