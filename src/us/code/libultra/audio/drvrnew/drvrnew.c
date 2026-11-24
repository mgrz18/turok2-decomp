/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_5.c, func_802B3F10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libaudio.h"

void alFilterNew(void *f, void *handler, void *setParam, s32 type);
void *alHeapDBAlloc(u8 *file, s32 line, void *hp, s32 num, s32 size);
extern s32 alFxParam(void *filter, s32 paramID, void *param);
extern Acmd *alFxPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
extern s32 alFxParamHdl(void *filter, s32 paramID, void *param);

void alFxNew(ALFx *r, ALSynConfig *c, ALHeap *hp)
{
    u16 i, j, k;
    s32 *param = 0;
    ALFilter *f = &r->filter;
    ALDelay *d;
    alFilterNew(f, 0, (sptr)alFxParam, 5);
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
            rsinc = (f32)param[j++] * 0.001f;
            d->rsinc = (rsinc + rsinc) / c->outputRate;
            depth = (f32)param[j++];
            length = d->output - d->input;
            flength = length;
            if (length < 0)
                flength += 4294967296.0;
            d->rsgain = depth * ((f32)flength * 5.7762263e-6f);
            d->rsval = 1.0f;
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
                fcoef = ffc = (f64)fc * 6.103515625e-5;
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

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_7.c, func_802B43A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

typedef sptr LoadNewHeap;
extern void alAdpcmPull(void);
extern s32 alLoadParam(void *filter, s32 paramID, void *param);

void alLoadNew(void *arg0, ALDMANew arg1, LoadNewHeap arg2) {
    alFilterNew(arg0, (void *) alAdpcmPull, alLoadParam, 0);
    ((ALLoadFilter *)arg0)->state = alHeapDBAlloc(0, 0, (void *)arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->lstate = alHeapDBAlloc(0, 0, (void *)arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->dma = arg1(&((ALLoadFilter *)arg0)->dmaState);
    ((ALLoadFilter *)arg0)->lastsam = 0;
    ((ALLoadFilter *)arg0)->first = 1;
    ((ALLoadFilter *)arg0)->memin = 0;
}

#include "structs/sound.h"

extern void *alMainBusPull(void *, s16 *, s32, s32, void *); /* alMainBusPull */
extern s32 alMainBusParam(sptr, s32, sptr);
/* alMainBusNew */
typedef sptr BusSources; /* the source list is a host pointer (s32 truncated it above 4 GB) */

void alMainBusNew(ALMainBus *arg0, BusSources arg1, s32 arg2) {
    alFilterNew(arg0, alMainBusPull, alMainBusParam, 7);
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}

extern s32 func_00294D54(s32, s32, s32, s32, s32);
#ifdef TARGET_PORT
extern Acmd *alAuxBusPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
#endif

extern s32 alAuxBusParam(s32, s32, s32);

void alAuxBusNew(ALAuxBus *arg0, BusSources arg1, s32 arg2) {
#ifdef TARGET_PORT
    /* func_00294D54 + 0xC is alAuxBusPull (alAuxBusPull): the ROM points into a stray word */
    alFilterNew(arg0, alAuxBusPull, alAuxBusParam, 6);
#else
    alFilterNew(arg0, (s8 *)func_00294D54 + 0xC, alAuxBusParam, 6);
#endif
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}

extern Acmd *alResamplePull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
extern s32 alResampleParam(void *filter, s32 paramID, void *param);

void alResampleNew(ALResampler *arg0, sptr arg1) {
    sptr result;
    f32 k;
    alFilterNew(arg0, alResamplePull, alResampleParam, 1);
    result = alHeapDBAlloc(0, 0, arg1, 1, 0x20);
    k = 1.0f;
    arg0->state = (void *)result;
    arg0->delta = 0;
    arg0->first = 1;
    arg0->motion = 0;
    arg0->upitch = 0;
    arg0->ctrlList = 0;
    arg0->ctrlTail = 0;
    arg0->ratio = k;
}

extern void *alSavePull(void *, s16 *, s32, s32, void *); /* alSavePull */
extern s32 alSaveParam(sptr, s32, sptr);
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} SaveFilter;

#ifndef TARGET_PORT /* the port defines alSaveNew in port/audio/t2audio_lib.c */
void alSaveNew(SaveFilter *arg0) {
    alFilterNew(arg0, alSavePull, alSaveParam, 3);
    arg0->unk14 = 0;
    arg0->unk18 = 1;
}
#endif /* !TARGET_PORT */

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/alEnvmixerNew.c, alEnvmixerNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void *alEnvmixerPull(void);
s32 alEnvmixerParam(void *filter, s32 paramID, void *param);

void alEnvmixerNew(ALEnvMixer *e, ALHeap *hp) {
    alFilterNew(&e->filter, alEnvmixerPull, alEnvmixerParam, 4);
    e->state = alHeapDBAlloc(0, 0, hp, 1, 0x50);
    e->first = 1;
    e->motion = 0;
    e->volume = 1;
    e->ltgt = 1;
    e->rtgt = 1;
    e->cvolL = 1;
    e->cvolR = 1;
    e->dryamt = 0;
    e->wetamt = 0;
    e->lratm = 1;
    e->lratl = 0;
    *(volatile u16 *) &e->lratl = 0; /* libultra stores lratl twice (where rratl was meant) */
    e->delta = 0;
    e->segEnd = 0;
    e->pan = 0;
    e->ctrlList = 0;
    e->ctrlTail = 0;
    e->sources = 0;
}
void init_lpfilter(ALLowPass *lp);

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
