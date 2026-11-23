/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_6.c, func_802B2BBC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void _handleEvent(ALSndPlayer * arg0, ALSndpEvent * arg1);
extern s32 _sndpVoiceHandler(void *node);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, ALMicroTime);
extern s32 alEvtqNextEvent(ALEventQueue *, ALEvent *);
s32 _sndpVoiceHandler(void *node)
{
    ALSndPlayer *sndp = (ALSndPlayer *)node;
    ALSndpEvent evt;
    ALSndpEvent *next = (ALSndpEvent *)&sndp->nextEvent;
    do {
        switch (sndp->nextEvent.type) {
            case 5:
                evt.common.type = 5;
                alEvtqPostEvent(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
                break;
            default:
                _handleEvent(sndp, next);
                break;
        }
        sndp->nextDelta = alEvtqNextEvent(&sndp->evtq, &sndp->nextEvent);
    } while (sndp->nextDelta == 0);
    sndp->curTime += sndp->nextDelta;
    return sndp->nextDelta;
}
