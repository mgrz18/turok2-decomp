/* From decompals/ultralib (libultra 2.0I): src/audio/synthesizer.c, alSynNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

void alLink(ALLink *element, ALLink *after);
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void alSynNew(ALSynth *drvr, ALSynConfig *c);
ALFxRef *alSynAllocFX(ALSynth *s, s16 bus, ALSynConfig *c, ALHeap *hp);
void alLoadNew(ALLoadFilter *f, ALDMANew dma, ALHeap *hp);
s32 alLoadParam(void *filter, s32 paramID, void *param);
void alMainBusNew(ALMainBus *m, void *ptr, s32 len);
s32 alMainBusParam(void *filter, s32 paramID, void *param);
void alAuxBusNew(ALAuxBus *m, void *ptr, s32 len);
s32 alAuxBusParam(void *filter, s32 paramID, void *param);
void alResampleNew(ALResampler *r, ALHeap *hp);
s32 alResampleParam(void *f, s32 paramID, void *param);
void alSaveNew(ALSave *r);
s32 alSaveParam(void *f, s32 paramID, void *param);
void alEnvmixerNew(ALEnvMixer *e, ALHeap *hp);
s32 alEnvmixerParam(void *filter, s32 paramID, void *param);

void alSynNew(ALSynth *drvr, ALSynConfig *c)
{
    s32 i;
    ALVoice *vv;
    PVoice *pv;
    ALVoice *vvoices;
    PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALSave *save;
    ALFilter *sources;
    ALParam *params;
    ALParam *paramPtr;
    drvr->head = 0;
    drvr->numPVoices = c->maxPVoices;
    drvr->curSamples = 0;
    drvr->paramSamples = 0;
    drvr->outputRate = c->outputRate;
    drvr->maxOutSamples = 160;
    drvr->dma = (ALDMANew) c->dmaproc;
    save = alHeapDBAlloc(0, 0,(hp),(1),(sizeof(ALSave)));
    alSaveNew(save);
    drvr->outputFilter = (ALFilter *)save;
    drvr->auxBus = alHeapDBAlloc(0, 0,(hp),(1),(sizeof(ALAuxBus)));
    drvr->maxAuxBusses = 1;
    sources = alHeapDBAlloc(0, 0,(hp),(c->maxPVoices),(sizeof(ALFilter *)));
    alAuxBusNew(drvr->auxBus, sources, c->maxPVoices);
    drvr->mainBus = alHeapDBAlloc(0, 0,(hp),(1),(sizeof(ALMainBus)));
    sources = alHeapDBAlloc(0, 0,(hp),(c->maxPVoices),(sizeof(ALFilter *)));
    alMainBusNew(drvr->mainBus, sources, c->maxPVoices);
    if (c->fxType != 0){
        alSynAllocFX(drvr, 0, c, hp);
    } else
     alMainBusParam(drvr->mainBus, AL_FILTER_ADD_SOURCE, &drvr->auxBus[0]);
    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;
    pvoices = alHeapDBAlloc(0, 0,(hp),(c->maxPVoices),(sizeof(PVoice)));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        alLink((ALLink *)pv, &drvr->pFreeList);
        pv->vvoice = 0;
        alLoadNew(&pv->decoder, drvr->dma, hp);
        alLoadParam(&pv->decoder, AL_FILTER_SET_SOURCE, 0);
        alResampleNew(&pv->resampler, hp);
        alResampleParam(&pv->resampler, AL_FILTER_SET_SOURCE, &pv->decoder);
        alEnvmixerNew(&pv->envmixer, hp);
        alEnvmixerParam(&pv->envmixer, AL_FILTER_SET_SOURCE, &pv->resampler);
        alAuxBusParam(drvr->auxBus, AL_FILTER_ADD_SOURCE, &pv->envmixer);
        pv->channelKnob = (ALFilter *)&pv->envmixer;
    }
    alSaveParam(save, AL_FILTER_SET_SOURCE, drvr->mainBus);
    params = alHeapDBAlloc(0, 0,(hp),(c->maxUpdates),(sizeof(ALParam)));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr= &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }
    drvr->heap = hp;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_12.c, func_802B3A44_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libaudio.h"
/* A node of a list sorted by time: next at 0, its time at 0x10. */
typedef struct TimedNode {
    /* 0x00 */ struct TimedNode *next;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s32 time;
} TimedNode;

/* The list: its head at 0 and the current time at 0x20. */
typedef struct {
    /* 0x00 */ TimedNode *head;
    /* 0x04 */ u8 pad4[0x1C];
    /* 0x20 */ s32 now;
} TimedList;

/* Only the field read here: a rate, in units per million. */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 rate;
} RateSource;

/* Output after every other function of the file (DEFERRED, common.h): the ROM has __nextSampleTime's
 * strings before _timeToSamples' float literals. */
s32 _timeToSamples(RateSource *arg0, s32 arg1);
s32 __nextSampleTime(TimedList *arg0, TimedNode **arg1);
s32 _timeToSamplesNoRound(RateSource *arg0, s32 arg1);
/* A list owner: the node list head at 0x14, the list itself (passed to alLink) at 4. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u8 list[0x10];
    /* 0x14 */ s32 head;
} ListOwner294;
void _collectPVoices(ListOwner294 *arg0);

Acmd *alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen)
{
    ALPlayer *client;
    ALFilter *output;
    ALSynth *drvr = &alGlobals->drvr;
    s16 tmp = 0;
    Acmd *cmdlEnd = cmdList;
    Acmd *cmdPtr;
    s32 nOut;
    s16 *lOutBuf = outBuf;
    if (drvr->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }
    for (drvr->paramSamples = __nextSampleTime(drvr, &client);
         drvr->paramSamples - drvr->curSamples < outLen;
         drvr->paramSamples = __nextSampleTime(drvr, &client)) {
        drvr->paramSamples &= ~0xf;
        client->samplesLeft += _timeToSamplesNoRound(drvr, (*client->handler)(client));
    }
    drvr->paramSamples &= ~0xf;
    while (outLen > 0) {
        nOut = ((drvr->maxOutSamples) < (outLen) ? (drvr->maxOutSamples) : (outLen));
        cmdPtr = cmdlEnd;
        { Acmd *_a = (Acmd *)(cmdPtr++); _a->words.w0 = (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); };
        output = drvr->outputFilter;
        (*output->setParam)(output, 6, lOutBuf);
        cmdlEnd = (*output->handler)(output, &tmp, nOut, drvr->curSamples, cmdPtr);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        drvr->curSamples += nOut;
    }
    *cmdLen = (s32)(cmdlEnd - cmdList);
    _collectPVoices(drvr);
    return cmdlEnd;
}

/* libultra's __allocParam (synthesizer.c): take an ALParam off the free list
 * of alGlobals->drvr. */

ALParam *__allocParam(void) {
    ALParam *head;
    ALParam *update;

    head = alGlobals->drvr.paramList;
    update = NULL;
    if (head != NULL) {
        update = head;
        alGlobals->drvr.paramList = update->next;
        update->next = NULL;
    }
    return update;
}

/* libultra's __freeParam (synthesizer.c): push an ALParam back onto the free
 * list of alGlobals->drvr. */
void __freeParam(ALParam *param) {
    ALSynth *drvr = &alGlobals->drvr;

    param->next = drvr->paramList;
    drvr->paramList = param;
}

void alUnlink(ALLink *element);

/* The port: _freePVoice .. _timeToSamplesNoRound are port/audio/t2audio_lib.c's (port/cmake/link.cmake). */
#ifndef TARGET_PORT
void _freePVoice(sptr arg0, sptr arg1) {
    alUnlink((ALLink *) arg1);
    alLink((ALLink *) arg1, (ALLink *) (arg0 + 0x14));
}

void _collectPVoices(ListOwner294 *arg0) {
    s32 var_s0;
    ListOwner294 *var_s1;

    var_s1 = arg0;
    var_s0 = var_s1->head;
    if (var_s0 != 0) {
        do {
            alUnlink((ALLink *) var_s0);
            alLink((ALLink *) var_s0, (ALLink *) var_s1->list);
            var_s0 = var_s1->head;
        } while (var_s0 != 0);
    }
}

/* arg1 scaled by the rate, rounded down to a multiple of 16. */
DEFERRED s32 _timeToSamples(RateSource *arg0, s32 arg1) {
    return (s32) (((f32) arg1 * (f32) arg0->rate * 0.000001f) + 0.5f) & ~0xF;
}

s32 __assert(s32 *, s32 *, s32); /* extern */

DEFERRED s32 __nextSampleTime(TimedList *arg0, TimedNode **arg1) {
    s32 var_s0;
    TimedNode *var_a0;

    var_s0 = 0x7FFFFFFF;
    if (arg0->head == NULL) {
        __assert("EX", "audio/synthesizer.c", 0x133);
    }
    *arg1 = NULL;
    var_a0 = arg0->head;
    if (var_a0 != NULL) {
        do {
            if ((var_a0->time - arg0->now) < var_s0) {
                *arg1 = var_a0;
                var_s0 = var_a0->time - arg0->now;
            }
            var_a0 = var_a0->next;
        } while (var_a0 != NULL);
    }
    return (*arg1)->time;
}

DEFERRED s32 _timeToSamplesNoRound(RateSource *arg0, s32 arg1) {
    return (s32) (((f32) arg1 * (f32) arg0->rate * 0.000001f) + 0.5f);
}
#endif
