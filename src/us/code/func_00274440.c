#include "common.h"

extern f32 D_800B2848[];
#define TABLE (D_800B2848 + 1)

/* Linear interpolation in a table sampled every 60 units from -6000. */
f32 func_00274440(f32 x) {
    f32 pos = (x + 6000.0f) * 0.016666668f;
    s16 i = pos;
    f32 fi = i;
    s16 j;

    f32 a;

    if (fi != pos) {
        j = i + 1;
        a = TABLE[i];
        return a + (pos - fi) * (TABLE[j] - a);
    }
    return TABLE[i];
}
