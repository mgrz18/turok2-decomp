/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_7.c, func_802B468C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002955EC(AudioLowPassFilter *lp);
extern const f64 D_800AA528;
extern const f64 D_800AA530;
void func_002955EC(AudioLowPassFilter *lp)
{
    s32 i, temp;
    s16 fc;
    f64 ffc, fcoef;
    temp = lp->cutoff * 16384;
    fc = temp >> 15;
    lp->gain = 16384 - fc;
    lp->first = 1;
    for (i = 0; i < 8; i++)
        lp->coefficients.taps[i] = 0;
    lp->coefficients.taps[i++] = fc;
    fcoef = ffc = (f64)fc * D_800AA528;
    for (; i < 16; i++) {
        fcoef *= ffc;
        lp->coefficients.taps[i] = (s16)(fcoef * D_800AA530);
    }
}
