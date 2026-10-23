#include "common.h"

/* Smallest n with 2^n >= 1 + 1 / (2 * x); 0 when x >= 0.5. */
s32 func_002189EC(f32 x) {
    s32 n = 0;
    f32 t = 1.0f / (x + x) + 1.0f;

    if (x >= 0.5f) {
        return n;
    }
    while ((f32)(1 << n) < t) {
        n++;
    }
    return n;
}
