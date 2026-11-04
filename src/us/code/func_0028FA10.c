/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_11.c, func_802AEAB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char func_0028FB80;
extern void func_002928F0(void *);
extern ALGlobals *D_800B7760; /* alGlobals */
extern s32 func_002912A0(s32, s32, void *, s32, s32);
extern void func_00290F20(void *, void *, s32);
extern void func_00293F10(ALSynth *, ALPlayer *); /* alSynAddPlayer */
void func_0028FA10(ALCSPlayer *arg0, ALSeqpConfig *arg1) {
    ALCSPlayer *obj = arg0;
    ALSeqpConfig *params = arg1;
    ALHeap *heap = params->heap;
    ALSynth *drvr = &D_800B7760->drvr;
    s32 allocation;
    s32 i;
    obj->chanMask = 0xFF;
    obj->uspt = 0x1E8;
    obj->vol = 0x7FFF;
    obj->bank = 0;
    obj->target = 0;
    obj->nextDelta = 0;
    obj->state = 0;
    obj->frameTime = 0x3E80;
    obj->curTime = 0;
    obj->drvr = drvr;
    obj->initOsc = params->initOsc;
    obj->updateOsc = params->updateOsc;
    obj->stopOsc = params->stopOsc;
    obj->nextEvent.type = 9;
    obj->maxChannels = params->maxChannels;
    obj->chanState = func_002912A0(0, 0, heap, params->maxChannels, 0x10);
    func_002928F0(obj);
    allocation = func_002912A0(0, 0, heap, params->maxVoices, 0x38);
    obj->vFreeList = 0;
    i = 0;
    if (params->maxVoices > 0) {
        {
            ALVoiceState *records = (ALVoiceState *)allocation;
            do {
                records->next = obj->vFreeList;
                obj->vFreeList = records;
                i++;
                records++;
            } while (i < params->maxVoices);
        }
    }
    obj->vAllocHead = 0;
    obj->vAllocTail = 0;
    allocation = func_002912A0(0, 0, heap, params->maxEvents, 0x1C);
    func_00290F20(&obj->evtq, (void *)allocation, params->maxEvents);
    {
        ALPlayer *node = &obj->node;
        node->next = 0;
        node->handler = &func_0028FB80;
        node->clientData = node;
        func_00293F10(&D_800B7760->drvr, node);
    }
}
