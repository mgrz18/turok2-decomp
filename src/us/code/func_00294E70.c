/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_5.c, func_802B3F10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern double D_800AA508;
extern float D_800AA514;
extern double D_800AA518;
extern void func_00296340(void *f, int h, int s, int type);
extern s32 func_002912A0(s32 file, s32 line, void *hp, s32 num, s32 size);
extern char func_00297038;
extern char func_00296750;
extern char func_00296AF4;
extern s32 D_800B7770[];
extern s32 D_800B77D8[];
extern s32 D_800B7860[];
extern s32 D_800B7888[];
extern s32 D_800B78B0[];
extern s32 D_800B78D8[];
extern const f32 D_800AA500;
extern const f32 D_800AA510;
void func_00294E70(ALFx *r, ALSynConfig *c, ALHeap *hp)
{
    u16 i, j, k;
    s32 *param = 0;
    ALFilter_s14 *f = &r->filter;
    ALDelay28 *d;
    func_00296340(f, 0, (s32)&func_00297038, 5);
    f->handler = &func_00296750;
    r->paramHdl = &func_00296AF4;
    switch (c->fxType) {
      case 1: param = D_800B7770; break;
      case 2: param = D_800B77D8; break;
      case 5: param = D_800B7860; break;
      case 3: param = D_800B7888; break;
      case 4: param = D_800B78B0; break;
      case 6: param = c->params; break;
      default: param = D_800B78D8; break;
    }
    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = (void *)func_002912A0(0, 0, hp, r->section_count, sizeof(ALDelay28));
    r->base = (void *)func_002912A0(0, 0, hp, r->length, sizeof(s16));
    r->input = r->base;
    for (k = 0; k < r->length; k++)
        r->base[k] = 0;
    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];
        if (param[j]) {
            s32 length;
            f64 flength;
            f32 rsinc;
            f32 depth;
            rsinc = (f32)param[j++] * D_800AA500;
            d->rsinc = (rsinc + rsinc) / c->outputRate;
            depth = (f32)param[j++];
            length = d->output - d->input;
            flength = length;
            if (length < 0)
                flength += D_800AA508;
            d->rsgain = depth * ((f32)flength * D_800AA510);
            d->rsval = D_800AA514;
            d->rsdelta = 0;
            d->rs = (void *)func_002912A0(0, 0, hp, 1, sizeof(struct ALResampler_s));
            ((struct ALResampler_s *)d->rs)->state = (void *)func_002912A0(0, 0, hp, 1, 32);
            ((struct ALResampler_s *)d->rs)->delta = 0;
            ((struct ALResampler_s *)d->rs)->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }
        if (param[j]) {
            d->lp = (void *)func_002912A0(0, 0, hp, 1, sizeof(struct AudioLowPassFilter));
            ((struct AudioLowPassFilter *)d->lp)->state = (void *)func_002912A0(0, 0, hp, 1, sizeof(AudioPoleFilterState));
            ((struct AudioLowPassFilter *)d->lp)->cutoff = param[j++];
            {
                struct AudioLowPassFilter *lp = d->lp;
                s32 n, temp;
                s16 fc;
                f64 ffc, fcoef;
                temp = lp->cutoff * 16384;
                fc = temp >> 15;
                lp->gain = 16384 - fc;
                lp->first = 1;
                for (n = 0; n < 8; n++)
                    lp->coefficients.taps[n] = 0;
                lp->coefficients.taps[n++] = fc;
                fcoef = ffc = (f64)fc * D_800AA518;
                for (; n < 16; n++) {
                    fcoef *= ffc;
                    lp->coefficients.taps[n] = (s16)(fcoef * 16384.0);
                }
            }
        } else {
            d->lp = 0;
            j++;
        }
    }
}
