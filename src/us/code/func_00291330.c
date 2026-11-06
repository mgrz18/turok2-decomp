/* Boundary fixed in versions/cuts.us.txt: splat starts func_00291324 three words early (stray words); the
 * function starts at 0x291330. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/csplayer.c, alCSPNew, with Turok 2's layouts.
 * It is libultra 2.0I's alSeqpNew (seqplayer.c): it clears the ALSeqPlayer's
 * loopStart/loopEnd/loopCount and installs __seqpVoiceHandler (__seqpVoiceHandler). */
#include "common.h"
#include "structs/libaudio.h"
#include "globals/libaudio.h"

#define AL_STOPPED 0
#define AL_USEC_PER_FRAME 16000
#define AL_SEQP_API_EVT 9

void *alHeapDBAlloc(char *file, s32 line, void *hp, s32 num, s32 size); /* alHeapDBAlloc */
void __initChanState(ALSeqPlayer *seqp);                                   /* __initChanState */
void alEvtqNew(ALEventQueue *evtq, void *items, s32 itemCount);     /* alEvtqNew */
void alSynAddPlayer(void *drvr, ALPlayer *client);                       /* alSynAddPlayer */
s32 __seqpVoiceHandler(void *node);                                          /* __seqpVoiceHandler */

#define alHeapAlloc(hp, num, size) alHeapDBAlloc(0, 0, hp, num, size)

void alSeqpNew(ALSeqPlayer *seqp, ALSeqpConfig *c) {
    s32 i;
    void *items;
    ALVoiceState *vs;
    ALVoiceState *voices;

    void *hp = c->heap;

    seqp->bank = 0;
    seqp->target = 0;
    seqp->drvr = &alGlobals->drvr;
    seqp->chanMask = 0xFF;
    seqp->uspt = 488;
    seqp->nextDelta = 0;
    seqp->state = AL_STOPPED;
    seqp->vol = 0x7FFF;
    seqp->debugFlags = c->debugFlags;
    seqp->frameTime = AL_USEC_PER_FRAME;
    seqp->curTime = 0;
    seqp->initOsc = c->initOsc;
    seqp->updateOsc = c->updateOsc;
    seqp->stopOsc = c->stopOsc;
    seqp->loopStart = 0;
    seqp->loopEnd = 0;
    seqp->loopCount = 0;

    seqp->nextEvent.type = AL_SEQP_API_EVT;

    seqp->maxChannels = c->maxChannels;
    seqp->chanState = alHeapAlloc(hp, c->maxChannels, 0x10);
    __initChanState(seqp);

    voices = alHeapAlloc(hp, c->maxVoices, sizeof(ALVoiceState));
    seqp->vFreeList = 0;
    for (i = 0; i < c->maxVoices; i++) {
        vs = &voices[i];
        vs->next = seqp->vFreeList;
        seqp->vFreeList = vs;
    }

    seqp->vAllocHead = 0;
    seqp->vAllocTail = 0;

    items = alHeapAlloc(hp, c->maxEvents, 0x1C);
    alEvtqNew(&seqp->evtq, items, c->maxEvents);

    seqp->node.next = 0;
    seqp->node.handler = __seqpVoiceHandler;
    seqp->node.clientData = seqp;
    alSynAddPlayer(alGlobals, &seqp->node);
}
