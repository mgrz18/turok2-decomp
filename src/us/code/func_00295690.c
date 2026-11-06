/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/alEnvmixerPull.c, alEnvmixerPull (libaudio env.c), with Turok 2's layouts.
 * Kept from that source: `one`/`eq` set at the top of the loop for the ROM's hoisting order, and the
 * HACK `register ALParam *pp asm("$2")` in the type 11 arm (ROM keeps the pointer in v0). */
#include "common.h"
#include "structs/libaudio.h"

extern ALGlobals *D_800B7760;
extern s16 D_800B7900[]; /* eqpower */
void __assert(const char *exp, const char *file, s32 line); /* __assert */
#define assert_ln(EX, ln) ((EX) ? ((void)0) : __assert("EX", "audio/env.c", ln))
void _freePVoice(ALSynth *drvr, PVoice *pvoice); /* _freePVoice */
void __freeParam(ALParam *param); /* __freeParam */
void *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* _pullSubFrame */
f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel); /* _getVol */
void *alEnvmixerPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    s16 inp;
    s16 loutp;
    s32 delta;
    s32 cur = sampleOffset;
    s32 old;
    ALParam *tmp;
    s16 *eq;
    s32 one;
    loutp = 0;
    inp = 0;
    if (e->ctrlList != 0) {
        while (e->ctrlList != 0) {
            one = 1;
            eq = D_800B7900;
            old = cur;
            cur = e->ctrlList->delta;
            delta = cur - old;
            if (delta > outCount) break;
            assert_ln(delta >= 0, 0x67);
            assert_ln(delta <= 0xA0, 0x68);
            switch (e->ctrlList->type) {
            case 13: {
                ALStartParamAlt *s1 = (ALStartParamAlt *)e->ctrlList;
                ALFilter *f = (ALFilter *)e;
                if (s1->unity) {
                    (*f->setParam)(f, 8, 0);
                }
                (*f->setParam)(f, 5, s1->wave);
                (*f->setParam)(f, 9, 0);
                e->first = one;
                e->delta = 0;
                e->segEnd = s1->samples;
                e->volume = (s1->volume * s1->volume) >> 15;
                e->pan = s1->pan;
                e->dryamt = eq[s1->fxMix];
                e->wetamt = eq[0x7F - s1->fxMix];
                if (s1->samples != 0) {
                    e->cvolL = one;
                    e->cvolR = one;
                } else {
                    e->cvolL = (e->volume * eq[e->pan]) >> 15;
                    e->cvolR = (e->volume * eq[0x7F - e->pan]) >> 15;
                }
                if (f->source) {
                    (*f->source->setParam)(f->source, 7, (void *) *(s32 *) &s1->pitch);
                }
                break; }
            case 11:
            case 12:
            case 16:
                ptr = _pullSubFrame(e, &inp, &loutp, delta, sampleOffset, ptr);
                if (e->delta >= e->segEnd) {
                    e->ltgt = (e->volume * eq[e->pan]) >> 15;
                    e->rtgt = (e->volume * eq[0x7F - e->pan]) >> 15;
                    e->cvolL = e->ltgt;
                    e->cvolR = e->rtgt;
                    e->delta = e->segEnd;
                } else {
                    e->cvolL = _getVol((f32)e->cvolL, e->delta, e->lratm, e->lratl);
                    e->cvolR = _getVol((f32)e->cvolR, e->delta, e->rratm, e->rratl);
                }
                if (e->cvolL == 0) e->cvolL = one;
                if (e->cvolR == 0) e->cvolR = one;
                if (e->ctrlList->type == 12) {
                    e->pan = e->ctrlList->data.i;
                }
                if (e->ctrlList->type == 11) {
                    register ALParam *pp __asm__("$2");
                    e->delta = 0;
                    pp = e->ctrlList;
                    e->volume = (pp->data.i * pp->data.i) >> 15;
                    e->segEnd = pp->moredata.i;
                }
                if (e->ctrlList->type == 16) {
                    e->dryamt = eq[e->ctrlList->data.i];
                    e->wetamt = eq[0x7F - e->ctrlList->data.i];
                }
                e->first = one;
                break;
            case 14: {
                ALStartParam *s1 = (ALStartParam *)e->ctrlList;
                if (s1->unity) {
                    (*e->filter.setParam)(e, 8, 0);
                }
                (*e->filter.setParam)(e, 5, s1->wave);
                (*e->filter.setParam)(e, 9, 0);
                break; }
            case 15:
                ptr = _pullSubFrame(e, &inp, &loutp, delta, sampleOffset, ptr);
                (*e->filter.setParam)(e, 4, 0);
                break;
            case 0: {
                ALSynth *drvr = &D_800B7760->drvr;
                ALFreeParam *pp = (ALFreeParam *)e->ctrlList;
                pp->pvoice->offset = 0;
                _freePVoice(drvr, pp->pvoice);
                break; }
            default:
                ptr = _pullSubFrame(e, &inp, &loutp, delta, sampleOffset, ptr);
                (*e->filter.setParam)(e, e->ctrlList->type, (void *) e->ctrlList->data.i);
                break;
            }
            outCount -= delta;
            loutp += delta << 1;
            tmp = e->ctrlList;
            e->ctrlList = tmp->next;
            if (e->ctrlList == 0) {
                e->ctrlTail = 0;
            }
            __freeParam(tmp);
        }
    }
    ptr = _pullSubFrame(e, &inp, &loutp, outCount, sampleOffset, ptr);
    if (e->delta > e->segEnd) {
        e->delta = e->segEnd;
    }
    return ptr;
}
