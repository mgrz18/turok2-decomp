/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B6300_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 _doModFunc(ALDelay *d, s32 count);
f32 _doModFunc(ALDelay *d, s32 count)
{
    f32 val;
    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > 2.0f) ? d->rsval - (4.0f) : d->rsval;
    val = d->rsval;
    val = (val < 0) ? -val : val;
    val -= 1.0f;
    return d->rsgain * val;
}
