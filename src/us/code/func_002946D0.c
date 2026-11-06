/* From decompals/ultralib (libultra 2.0I): src/audio/synthesizer.c, alSynNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
#include "common.h"
#include "structs/libaudio.h"

void alLink(ALLink *element, ALLink *after);
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void alSynNew(ALSynth *s, ALSynConfig *config);
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
