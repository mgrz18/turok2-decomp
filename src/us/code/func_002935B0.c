/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2650_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_00293B1C(void * arg0);
extern ALGlobals_func_802B2650_de *D_800B7760;
extern void *func_002912A0(u8 *, s32, ALHeap *, s32, s32);
extern void func_00290F20(ALEventQueue *, ALEventListItem *, s32);
extern void func_00293F10(ALSynth_func_802B2650_de *, ALPlayer_s14 *);
extern void func_00291034(ALEventQueue *, ALEvent *, s32);
extern s32 func_00290F9C(ALEventQueue *, ALEvent *);
extern s32 func_00293B1C(void *);
void func_002935B0(ALSndPlayer_func_802B2650_de *sndp, ALSndpConfig *c)
{
    u8 *ptr;
    ALEvent evt;
    ALSoundState *sState;
    u32 i;
    sndp->maxSounds = c->maxSounds;
    sndp->target = -1;
    sndp->frameTime = 16000;
    sState = (ALSoundState *)func_002912A0(0, 0, c->heap, 1, c->maxSounds * sizeof(ALSoundState));
    sndp->sndState = sState;
    for (i = 0; i < c->maxSounds; i++)
        sState[i].sound = 0;
    ptr = func_002912A0(0, 0, c->heap, 1, c->maxEvents * sizeof(ALEventListItem));
    func_00290F20(&sndp->evtq, (ALEventListItem *)ptr, c->maxEvents);
    sndp->drvr = &D_800B7760->drvr;
    sndp->node.next = 0;
    sndp->node.handler = func_00293B1C;
    sndp->node.clientData = sndp;
    func_00293F10(sndp->drvr, &sndp->node);
    evt.type = 5;
    func_00291034(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
    sndp->nextDelta = func_00290F9C(&sndp->evtq, &sndp->nextEvent);
}
