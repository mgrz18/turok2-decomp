/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_6.c, func_802B2BBC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002936E0(struct ALSndPlayer * arg0, union ALSndpEvent * arg1);
extern s32 func_00293B1C(void *node);
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
extern s32 func_00290F9C(ALEventQueue *, ALEvent *);
s32 func_00293B1C(void *node)
{
    ALSndPlayer *sndp = (ALSndPlayer *)node;
    ALSndpEvent evt;
    ALSndpEvent *next = (ALSndpEvent *)&sndp->nextEvent;
    do {
        switch (sndp->nextEvent.type) {
            case 5:
                evt.common.type = 5;
                func_00291034(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
                break;
            default:
                func_002936E0(sndp, next);
                break;
        }
        sndp->nextDelta = func_00290F9C(&sndp->evtq, &sndp->nextEvent);
    } while (sndp->nextDelta == 0);
    sndp->curTime += sndp->nextDelta;
    return sndp->nextDelta;
}
