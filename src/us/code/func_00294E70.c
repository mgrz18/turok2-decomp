/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_5.c, func_802B3F10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libaudio.h"

extern double D_800AA508;
extern float D_800AA514;
extern double D_800AA518;
extern void alFilterNew(void *f, int h, int s, int type);
extern s32 alHeapDBAlloc(s32 file, s32 line, void *hp, s32 num, s32 size);
extern s32 alFxParam(void *filter, s32 paramID, void *param);
extern Acmd *alFxPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
extern s32 alFxParamHdl(void *filter, s32 paramID, void *param);
extern const f32 D_800AA500;
extern const f32 D_800AA510;
void alFxNew(ALFx *r, ALSynConfig *c, ALHeap *hp)
{
    u16 i, j, k;
    s32 *param = 0;
    ALFilter *f = &r->filter;
    ALDelay *d;
    alFilterNew(f, 0, (s32)alFxParam, 5);
    f->handler = alFxPull;
    r->paramHdl = alFxParamHdl;
    switch (c->fxType) {
      case 1: param = SMALLROOM_PARAMS; break;
      case 2: param = BIGROOM_PARAMS; break;
      case 5: param = ECHO_PARAMS; break;
      case 3: param = CHORUS_PARAMS; break;
      case 4: param = FLANGE_PARAMS; break;
      case 6: param = c->params; break;
      default: param = NULL_PARAMS; break;
    }
    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = (void *)alHeapDBAlloc(0, 0, hp, r->section_count, sizeof(ALDelay));
    r->base = (void *)alHeapDBAlloc(0, 0, hp, r->length, sizeof(s16));
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
            d->rs = (void *)alHeapDBAlloc(0, 0, hp, 1, sizeof(ALResampler));
            ((ALResampler *)d->rs)->state = (void *)alHeapDBAlloc(0, 0, hp, 1, 32);
            ((ALResampler *)d->rs)->delta = 0;
            ((ALResampler *)d->rs)->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }
        if (param[j]) {
            d->lp = (void *)alHeapDBAlloc(0, 0, hp, 1, sizeof(ALLowPass));
            ((ALLowPass *)d->lp)->fstate = (void *)alHeapDBAlloc(0, 0, hp, 1, sizeof(POLEF_STATE));
            ((ALLowPass *)d->lp)->fc = param[j++];
            {
                ALLowPass *lp = d->lp;
                s32 n, temp;
                s16 fc;
                f64 ffc, fcoef;
                temp = lp->fc * 16384;
                fc = temp >> 15;
                lp->fgain = 16384 - fc;
                lp->first = 1;
                for (n = 0; n < 8; n++)
                    lp->fcvec.fccoef[n] = 0;
                lp->fcvec.fccoef[n++] = fc;
                fcoef = ffc = (f64)fc * D_800AA518;
                for (; n < 16; n++) {
                    fcoef *= ffc;
                    lp->fcvec.fccoef[n] = (s16)(fcoef * 16384.0);
                }
            }
        } else {
            d->lp = 0;
            j++;
        }
    }
}
