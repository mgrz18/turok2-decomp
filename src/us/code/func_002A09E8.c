/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/adecReadBits.c, mdctFft.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_002A09E8(f32 *d, s32 n, s32 sign, f32 *cos, f32 *sin, s32 *bitrev);
void func_002A09E8(f32 *d, s32 n, s32 sign, f32 *cos, f32 *sin, s32 *bitrev) {
    s32 i, k, half, dist, stride, idx, w, nn;
    f32 wc, ws, xr, xi, tr, ti;
    for (i = 0; i < n; i++) {
        if (i < bitrev[i]) {
            tr = d[i * 2];
            d[i * 2] = d[bitrev[i] * 2];
            d[bitrev[i] * 2] = tr;
            tr = d[i * 2 + 1];
            d[i * 2 + 1] = d[bitrev[i] * 2 + 1];
            d[bitrev[i] * 2 + 1] = tr;
        }
    }
    dist = 2;
    half = 1;
    stride = n >> half;
    nn = n << half;
    while (stride > 0) {
        w = 0;
        for (k = 0; k < half; k++) {
            wc = cos[w];
            ws = (f32)sign * sin[w];
            for (idx = k * 2; idx < nn; idx += dist * 2) {
                xr = d[idx + dist];
                xi = d[idx + dist + 1];
                tr = wc * xr - ws * xi;
                ti = wc * xi + ws * xr;
                d[idx + dist] = d[idx] - tr;
                d[idx + dist + 1] = d[idx + 1] - ti;
                d[idx] = d[idx] + tr;
                d[idx + 1] = d[idx + 1] + ti;
            }
            w += stride;
        }
        stride >>= 1;
        half <<= 1;
        dist <<= 1;
    }
}
