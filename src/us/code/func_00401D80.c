#include "common.h"

/* Returns 0 when both points are strictly outside the same edge of the quad (x/z plane). */
s32 func_00401D80(f32 (*quad)[3], f32 *a, f32 *b) {
    s32 i;
    f32 *p;
    f32 *q;
    f32 dz;
    f32 dx;

    for (i = 0; i < 4; i++) {
        p = quad[i];
        q = (i == 3) ? quad[0] : quad[i + 1];
        dz = q[1] - p[1];
        dx = p[0] - q[0];
        if (dz * (a[0] - p[0]) + dx * (a[2] - p[1]) > 0.0f) {
            if (dz * (b[0] - p[0]) + dx * (b[2] - p[1]) > 0.0f) {
                return 0;
            }
        }
    }
    return 1;
}
