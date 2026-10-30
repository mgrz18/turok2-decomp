#include "common.h"

extern f32 D_800ACB50; /* 90.0f: the largest value accepted */

/* r = ceil(x), through an int. */
#define CEILF(r, x)                     \
    if ((x) <= 0.0f) {                  \
        r = (s32) (x);                  \
    } else {                            \
        s32 _i = (s32) (x);             \
        f32 _t = _i;                    \
        if (_t == (x)) {                \
            r = _t;                     \
        } else {                        \
            r = _i + 1;                 \
        }                               \
    }

/* ceil(a / b * 30), or ceil(a / b * 15) if that is too big, or a itself. */
f32 func_00216964(f32 a, f32 b) {
    f32 r = 0.0f;

    if (a == 1.0f) {
        return 1.0f;
    }
    if (b <= 0.01f) {
        return 1.0f;
    }
    CEILF(r, a / b * 30.0f)
    if (r < D_800ACB50) {
        return r;
    }
    CEILF(r, a / b * 15.0f)
    if (r < D_800ACB50) {
        return r;
    }
    return a;
}
