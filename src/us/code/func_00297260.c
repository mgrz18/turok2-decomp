/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B6300_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_00297260(ALDelay28 *d, s32 count);
extern const float D_800AA718;
extern const float D_800AA720;
f32 func_00297260(ALDelay28 *d, s32 count)
{
    f32 val;
    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > D_800AA718) ? d->rsval - (*(&D_800AA718 + 1)) : d->rsval;
    val = d->rsval;
    val = (val < 0) ? -val : val;
    val -= D_800AA720;
    return d->rsgain * val;
}
