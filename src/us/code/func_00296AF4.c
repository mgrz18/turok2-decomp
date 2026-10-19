/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B5B94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern const double D_800AA700;
extern const float D_800AA708;
extern void func_002955EC(AudioLowPassFilter *lp);
extern float D_800AA6F8;
extern ALGlobals_func_802B5B94_de *D_800B7760;
s32 func_00296AF4(ALFxCode802BAC64 *filter, s32 paramID, void *param)
{
    ALFxCode802BAC64 *f = filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;
    switch (p) {
        case 0:
            f->delay[s].input = (u32)val & 0xFFFFFFF8;
            break;
        case 1:
            f->delay[s].output = (u32)val & 0xFFFFFFF8;
            break;
        case 3:
            f->delay[s].ffcoef = (s16)val;
            break;
        case 2:
            f->delay[s].fbcoef = (s16)val;
            break;
        case 4:
            f->delay[s].gain = (s16)val;
            break;
        case 5:
            f->delay[s].rsinc = (((f32)val * D_800AA6F8) * 2.0f) / D_800B7760->drvr.unk44;
            break;
        case 6: {
            f32 fval = (f32)val;
            s32 length = (f->delay[s].output - f->delay[s].input);
            double flength = length;
            if (length < 0)
                flength += D_800AA700;
            f->delay[s].rsgain = fval * ((f32)flength * D_800AA708);
            break;
        }
        case 7:
            if (f->delay[s].lp) {
                f->delay[s].lp->mode = (s16)val;
                func_002955EC(f->delay[s].lp);
            }
            break;
    }
    return 0;
}
