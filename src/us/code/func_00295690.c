/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/alEnvmixerPull.c, alEnvmixerPull (libaudio env.c), with Turok 2's layouts.
 * Kept from that source: `one`/`eq` set at the top of the loop for the ROM's hoisting order, and the
 * HACK `register ALParam *pp asm("$2")` in the type 11 arm (ROM keeps the pointer in v0). */


typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;
typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    void *handler;
    s32 callTime;
    s32 samplesLeft;
} ALPlayer;
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *(*handler)(struct ALFilter_s *filter, s16 *outp, s32 outCount, s32 sampleOffset, void *cmdp);
    s32 (*setParam)(struct ALFilter_s *filter, s32 type, void *param);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct ALParam_s {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s16 moredata;
    union {
        s32 i;
        f32 f;
        void *p;
    } data;
    union {
        s32 i;
        struct {
            s16 vol;
            u8 pan;
            u8 fxmix;
        } v;
    } d2;
    s32 samples;
    void *wave;
} ALParam;
struct ALVoice_s;
typedef struct ALPVoice_s {
    ALLink node;
    struct ALVoice_s *vvoice;
    ALFilter *channelKnob;
    u8 decoder[0x48];
    u8 resampler[0x34];
    u8 envmixer[0x4C];
    s32 offset;
} ALPVoice;
typedef struct ALVoice_s {
    ALLink node;
    ALPVoice *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} ALVoice;
typedef struct ALSynth_s {
    ALPlayer *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    void *dma;
    void *heap;
    ALParam *paramList;
    ALFilter *auxBus;
    ALFilter *mainBus;
    ALFilter *saveBus;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
} ALSynth;
typedef struct {
    u32 w0;
    u32 w1;
} Acmd;
typedef struct {
    ALSynth drvr;
} ALGlobals;
extern ALGlobals *D_800B7760;
typedef struct {
    ALFilter filter;
    void *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    s32 pad44;
    s32 motion;
} ALEnvMixerFull;
extern s16 D_800B7900[]; /* eqpower */
void func_0029B020(const char *exp, const char *file, s32 line); /* __assert */
#define assert_ln(EX, ln) ((EX) ? ((void)0) : func_0029B020("EX", "audio/env.c", ln))
void func_00294B9C(ALSynth *drvr, ALPVoice *pvoice); /* _freePVoice */
void func_00294B84(ALParam *param); /* __freeParam */
void *func_00295BDC(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* _pullSubFrame */
f32 func_002961E8(f32 ivol, s32 samples, s16 ratem, u16 ratel); /* _getVol */
void *func_00295690(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALEnvMixerFull *e = (ALEnvMixerFull *)filter;
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
                ALParam *s1 = e->ctrlList;
                ALFilter *f = (ALFilter *)e;
                if (s1->moredata) {
                    (*f->setParam)(f, 8, 0);
                }
                (*f->setParam)(f, 5, s1->wave);
                (*f->setParam)(f, 9, 0);
                e->first = one;
                e->delta = 0;
                e->segEnd = s1->samples;
                e->volume = (s1->d2.v.vol * s1->d2.v.vol) >> 15;
                e->pan = s1->d2.v.pan;
                e->dryamt = eq[s1->d2.v.fxmix];
                e->wetamt = eq[0x7F - s1->d2.v.fxmix];
                if (s1->samples != 0) {
                    e->cvolL = one;
                    e->cvolR = one;
                } else {
                    e->cvolL = (e->volume * eq[e->pan]) >> 15;
                    e->cvolR = (e->volume * eq[0x7F - e->pan]) >> 15;
                }
                if (f->source) {
                    (*f->source->setParam)(f->source, 7, s1->data.p);
                }
                break; }
            case 11:
            case 12:
            case 16:
                ptr = func_00295BDC(e, &inp, &loutp, delta, sampleOffset, ptr);
                if (e->delta >= e->segEnd) {
                    e->ltgt = (e->volume * eq[e->pan]) >> 15;
                    e->rtgt = (e->volume * eq[0x7F - e->pan]) >> 15;
                    e->cvolL = e->ltgt;
                    e->cvolR = e->rtgt;
                    e->delta = e->segEnd;
                } else {
                    e->cvolL = func_002961E8((f32)e->cvolL, e->delta, e->lratm, e->lratl);
                    e->cvolR = func_002961E8((f32)e->cvolR, e->delta, e->rratm, e->rratl);
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
                    e->segEnd = pp->d2.i;
                }
                if (e->ctrlList->type == 16) {
                    e->dryamt = eq[e->ctrlList->data.i];
                    e->wetamt = eq[0x7F - e->ctrlList->data.i];
                }
                e->first = one;
                break;
            case 14: {
                ALParam *s1 = e->ctrlList;
                if (s1->moredata) {
                    (*e->filter.setParam)(e, 8, 0);
                }
                (*e->filter.setParam)(e, 5, s1->data.p);
                (*e->filter.setParam)(e, 9, 0);
                break; }
            case 15:
                ptr = func_00295BDC(e, &inp, &loutp, delta, sampleOffset, ptr);
                (*e->filter.setParam)(e, 4, 0);
                break;
            case 0: {
                ALSynth *drvr = &D_800B7760->drvr;
                ALParam *pp = e->ctrlList;
                ((ALPVoice *)pp->data.p)->offset = 0;
                func_00294B9C(drvr, (ALPVoice *)pp->data.p);
                break; }
            default:
                ptr = func_00295BDC(e, &inp, &loutp, delta, sampleOffset, ptr);
                (*e->filter.setParam)(e, e->ctrlList->type, e->ctrlList->data.p);
                break;
            }
            outCount -= delta;
            loutp += delta << 1;
            tmp = e->ctrlList;
            e->ctrlList = tmp->next;
            if (e->ctrlList == 0) {
                e->ctrlTail = 0;
            }
            func_00294B84(tmp);
        }
    }
    ptr = func_00295BDC(e, &inp, &loutp, outCount, sampleOffset, ptr);
    if (e->delta > e->segEnd) {
        e->delta = e->segEnd;
    }
    return ptr;
}
