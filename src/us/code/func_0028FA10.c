/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_11.c, func_802AEAB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char __CSPVoiceHandler;
extern void __initChanState(void *);
extern ALGlobals *alGlobals; /* alGlobals */
extern s32 alHeapDBAlloc(s32, s32, void *, s32, s32);
extern void alEvtqNew(void *, void *, s32);
extern void alSynAddPlayer(ALSynth *, ALPlayer *); /* alSynAddPlayer */
void alCSPNew(ALCSPlayer *arg0, ALSeqpConfig *arg1) {
    ALCSPlayer *obj = arg0;
    ALSeqpConfig *params = arg1;
    ALHeap *heap = params->heap;
    ALSynth *drvr = &alGlobals->drvr;
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
    obj->chanState = alHeapDBAlloc(0, 0, heap, params->maxChannels, 0x10);
    __initChanState(obj);
    allocation = alHeapDBAlloc(0, 0, heap, params->maxVoices, 0x38);
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
    allocation = alHeapDBAlloc(0, 0, heap, params->maxEvents, 0x1C);
    alEvtqNew(&obj->evtq, (void *)allocation, params->maxEvents);
    {
        ALPlayer *node = &obj->node;
        node->next = 0;
        node->handler = &__CSPVoiceHandler;
        node->clientData = node;
        alSynAddPlayer(&alGlobals->drvr, node);
    }
}
