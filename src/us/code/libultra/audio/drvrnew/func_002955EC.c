/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_7.c, func_802B468C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void init_lpfilter(ALLowPass *lp);
void init_lpfilter(ALLowPass *lp)
{
    s32 i, temp;
    s16 fc;
    f64 ffc, fcoef;
    temp = lp->fc * 16384;
    fc = temp >> 15;
    lp->fgain = 16384 - fc;
    lp->first = 1;
    for (i = 0; i < 8; i++)
        lp->fcvec.fccoef[i] = 0;
    lp->fcvec.fccoef[i++] = fc;
    fcoef = ffc = (f64)fc * 6.103515625e-5;
    for (; i < 16; i++) {
        fcoef *= ffc;
        lp->fcvec.fccoef[i] = (s16)(fcoef * 16384.0);
    }
}
