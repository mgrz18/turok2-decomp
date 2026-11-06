/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra_inferred/802B54A0_3.CPP, func_802B0A90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

extern char D_800AA380[];
extern char D_800AA384[];
extern void alSeqNextEvent(ALSeq *, ALEvent *);
extern void __handleMIDIMsg(ALSeqPlayer *, ALEvent *);
extern void __handleMetaMsg(ALSeqPlayer *, ALEvent *);
extern u8 __alSeqNextDelta(ALSeq *, s32 *);
extern int alSeqGetTicks(ALSeq *arg0);
extern void alSeqSetLoc(ALSeq *, ALSeqMarker *);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, ALMicroTime);
static __inline__ void postNextSeqEvent(ALSeqPlayer *seqp) {
    ALEvent evt;
    s32 deltaTicks;
    ALSeq *seq = seqp->target;
    if ((seqp->state != 1) || (seq == 0))
        return;
    if (!__alSeqNextDelta(seq, &deltaTicks))
        return;
    if (seqp->loopCount) {
        if (alSeqGetTicks(seq) + deltaTicks >= seqp->loopEnd->curTicks) {
            alSeqSetLoc(seq, seqp->loopStart);
            if (seqp->loopCount != -1)
                seqp->loopCount--;
        }
    }
    evt.type = 0;
    alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks * seqp->uspt);
}
extern void __assert(const char *arg0, const char *arg1, s32 arg2);
extern char D_800AA380[];
extern char D_800AA384[];
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, ALMicroTime);
void __handleNextSeqEvent(ALSeqPlayer *seqp)
{
    ALEvent evt;
    if (seqp->target == 0)
        return;
    alSeqNextEvent(seqp->target, &evt);
    switch (evt.type) {
    case 1:
        __handleMIDIMsg(seqp, &evt);
        postNextSeqEvent(seqp);
        break;
    case 3:
        __handleMetaMsg(seqp, &evt);
        postNextSeqEvent(seqp);
        break;
    case 4:
        seqp->state = 2;
        evt.type = 16;
        alEvtqPostEvent(&seqp->evtq, &evt, 0x7fffffff);
        break;
    default:
        __assert(D_800AA380, D_800AA384, 412);
    }
}
void __handleMIDIMsg(ALSeqPlayer *seqp, ALEvent *event);
