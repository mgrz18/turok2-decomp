/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_13.c, func_802BF804_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_002A08C4(f32 *out, s16 *in);
extern float D_800AAC34;
extern u16 D_800B8A88[];
extern s16 D_800B8AA6;
extern f32 D_800B8AA8[];
s32 func_002A08C4(f32 *out, s16 *in) {
    s16 *scale;
    s16 band;
    s16 i;
    s16 start;
    s16 *values;
    u16 *ends;
    f32 *steps;
    f32 gain;
    start = 0;
    values = in + 0x50;
    band = 0;
    ends = D_800B8A88;
    steps = D_800B8AA8;
    gain = D_800AAC34;
    for (; band < 16; band++) {
        for (i = start; i < ends[band]; i++) {
            out[i] = (f32)values[i] / steps[*(scale = &in[band])] * gain;
        }
        start = ends[band];
    }
    for (band = D_800B8AA6; band < 0x100; band++) {
        out[band] = 0.0f;
    }
    return 1;
}
