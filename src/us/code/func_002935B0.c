/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2650_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int _sndpVoiceHandler(void * arg0);
extern ALGlobals *alGlobals;
extern void *alHeapDBAlloc(u8 *, s32, ALHeap *, s32, s32);
extern void alEvtqNew(ALEventQueue *, ALEventListItem *, s32);
extern void alSynAddPlayer(ALSynth *, ALPlayer *);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, s32);
extern s32 alEvtqNextEvent(ALEventQueue *, ALEvent *);
extern s32 _sndpVoiceHandler(void *);
void alSndpNew(ALSndPlayer *sndp, ALSndpConfig *c)
{
    u8 *ptr;
    ALEvent evt;
    ALSoundState *sState;
    u32 i;
    sndp->maxSounds = c->maxSounds;
    sndp->target = -1;
    sndp->frameTime = 16000;
    sState = (ALSoundState *)alHeapDBAlloc(0, 0, c->heap, 1, c->maxSounds * sizeof(ALSoundState));
    sndp->sndState = sState;
    for (i = 0; i < c->maxSounds; i++)
        sState[i].sound = 0;
    ptr = alHeapDBAlloc(0, 0, c->heap, 1, c->maxEvents * sizeof(ALEventListItem));
    alEvtqNew(&sndp->evtq, (ALEventListItem *)ptr, c->maxEvents);
    sndp->drvr = &alGlobals->drvr;
    sndp->node.next = 0;
    sndp->node.handler = _sndpVoiceHandler;
    sndp->node.clientData = sndp;
    alSynAddPlayer(sndp->drvr, &sndp->node);
    evt.type = 5;
    alEvtqPostEvent(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
    sndp->nextDelta = alEvtqNextEvent(&sndp->evtq, &sndp->nextEvent);
}
