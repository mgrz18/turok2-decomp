/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra_inferred/802B54A0_3.CPP, func_802B0A90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

extern char D_800AA380[];
extern char D_800AA384[];
extern void func_00292DE0(ALSeq *, ALEvent *);
extern void func_00291BF4(ALSeqPlayer *, ALEvent *);
extern void func_00292D4C(ALSeqPlayer *, ALEvent *);
extern u8 func_00293254(ALSeq *, s32 *);
extern int func_00293028(ALSeq *arg0);
extern void func_0029321C(ALSeq *, ALSeqMarker *);
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
static __inline__ void postNextSeqEvent(ALSeqPlayer *seqp) {
    ALEvent evt;
    s32 deltaTicks;
    ALSeq *seq = seqp->target;
    if ((seqp->state != 1) || (seq == 0))
        return;
    if (!func_00293254(seq, &deltaTicks))
        return;
    if (seqp->loopCount) {
        if (func_00293028(seq) + deltaTicks >= seqp->loopEnd->curTicks) {
            func_0029321C(seq, seqp->loopStart);
            if (seqp->loopCount != -1)
                seqp->loopCount--;
        }
    }
    evt.type = 0;
    func_00291034(&seqp->evtq, &evt, deltaTicks * seqp->uspt);
}
extern void func_0029B020(const char *arg0, const char *arg1, s32 arg2);
extern char D_800AA380[];
extern char D_800AA384[];
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
void func_002919F0(ALSeqPlayer *seqp)
{
    ALEvent evt;
    if (seqp->target == 0)
        return;
    func_00292DE0(seqp->target, &evt);
    switch (evt.type) {
    case 1:
        func_00291BF4(seqp, &evt);
        postNextSeqEvent(seqp);
        break;
    case 3:
        func_00292D4C(seqp, &evt);
        postNextSeqEvent(seqp);
        break;
    case 4:
        seqp->state = 2;
        evt.type = 16;
        func_00291034(&seqp->evtq, &evt, 0x7fffffff);
        break;
    default:
        func_0029B020(D_800AA380, D_800AA384, 412);
    }
}
void func_00291BF4(ALSeqPlayer *seqp, ALEvent *event);
