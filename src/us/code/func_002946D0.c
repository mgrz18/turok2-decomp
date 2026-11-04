/* From decompals/ultralib (libultra 2.0I): src/audio/synthesizer.c, alSynNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
#include "common.h"
#include "structs/libaudio.h"

void func_002933E0(ALLink *element, ALLink *after);
void *func_002912A0(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void alSynNew(ALSynth *s, ALSynConfig *config);
ALFxRef *func_00297420(ALSynth *s, s16 bus, ALSynConfig *c, ALHeap *hp);
void func_00295304(ALLoadFilter *f, ALDMANew dma, ALHeap *hp);
s32 func_0029FB64(void *filter, s32 paramID, void *param);
void func_002953B4(ALMainBus *m, void *ptr, s32 len);
s32 func_002964A0(void *filter, s32 paramID, void *param);
void func_00295410(ALAuxBus *m, void *ptr, s32 len);
s32 func_00294E3C(void *filter, s32 paramID, void *param);
void func_0029546C(ALResampler *r, ALHeap *hp);
s32 func_00296690(void *f, s32 paramID, void *param);
void func_002954F8(ALSave *r);
s32 func_002973E0(void *f, s32 paramID, void *param);
void func_0029553C(ALEnvMixer *e, ALHeap *hp);
s32 func_00296118(void *filter, s32 paramID, void *param);
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
    save = func_002912A0(0, 0,(hp),(1),(sizeof(ALSave)));
    func_002954F8(save);
    drvr->outputFilter = (ALFilter *)save;
    drvr->auxBus = func_002912A0(0, 0,(hp),(1),(sizeof(ALAuxBus)));
    drvr->maxAuxBusses = 1;
    sources = func_002912A0(0, 0,(hp),(c->maxPVoices),(sizeof(ALFilter *)));
    func_00295410(drvr->auxBus, sources, c->maxPVoices);
    drvr->mainBus = func_002912A0(0, 0,(hp),(1),(sizeof(ALMainBus)));
    sources = func_002912A0(0, 0,(hp),(c->maxPVoices),(sizeof(ALFilter *)));
    func_002953B4(drvr->mainBus, sources, c->maxPVoices);
    if (c->fxType != 0){
        func_00297420(drvr, 0, c, hp);
    } else
     func_002964A0(drvr->mainBus, AL_FILTER_ADD_SOURCE, &drvr->auxBus[0]);
    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;
    pvoices = func_002912A0(0, 0,(hp),(c->maxPVoices),(sizeof(PVoice)));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        func_002933E0((ALLink *)pv, &drvr->pFreeList);
        pv->vvoice = 0;
        func_00295304(&pv->decoder, drvr->dma, hp);
        func_0029FB64(&pv->decoder, AL_FILTER_SET_SOURCE, 0);
        func_0029546C(&pv->resampler, hp);
        func_00296690(&pv->resampler, AL_FILTER_SET_SOURCE, &pv->decoder);
        func_0029553C(&pv->envmixer, hp);
        func_00296118(&pv->envmixer, AL_FILTER_SET_SOURCE, &pv->resampler);
        func_00294E3C(drvr->auxBus, AL_FILTER_ADD_SOURCE, &pv->envmixer);
        pv->channelKnob = (ALFilter *)&pv->envmixer;
    }
    func_002973E0(save, AL_FILTER_SET_SOURCE, drvr->mainBus);
    params = func_002912A0(0, 0,(hp),(c->maxUpdates),(sizeof(ALParam)));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr= &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }
    drvr->heap = hp;
}
