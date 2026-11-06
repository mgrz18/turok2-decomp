/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_13.c, func_802BF514_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern float sMdctPostCosPlusSin[];
extern void mdctInitTwiddles();
extern float sMdctPreSin[];
extern float sMdctPreCos[];
extern float sMdctPostSinMinusCos[];
extern s32 sAdecOverlap[];
void mdctInitTwiddles(void) {
    s16 i;
    f32 angle;
    f32 s;
    f32 c;
    f32 step;
    f32 offset;
    f32 scale;
    f32 *sines;
    f32 *cosines;
    s32 *zeros;
    f32 *sums;
    f32 *differences;
    step = 4.0f;
    offset = (1.0f);
    scale = 0.0030679617f;
    i = 0;
    sines = sMdctPreCos;
    cosines = sMdctPreSin;
    zeros = sAdecOverlap;
    for (; i < 128; i++) {
        angle = (i * step + offset) * scale;
        sines[i] = func_002974C0(angle);
        cosines[i] = func_002982F0(angle);
        zeros[i] = 0;
    }
    i = 0;
    scale = (0.012271847f);
    sums = sMdctPostCosPlusSin;
    differences = sMdctPostSinMinusCos;
    for (; i < 128; i++) {
        angle = i * scale;
        s = func_002974C0(angle);
        c = func_002982F0(angle);
        sums[i] = s + c;
        differences[i] = c - s;
    }
}
