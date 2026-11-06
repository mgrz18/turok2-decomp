/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/sdk/libgultra_inferred/802B3B80_4.CPP,
 * func_802AF150_de, with Turok 2's layouts. libultra's __CSPHandleNextSeqEvent (cspplayer.c)
 * with __CSPPostNextSeqEvent inlined. */

#include "common.h"
#include "structs/libaudio.h"

#define AL_SEQ_REF_EVT 0
#define AL_SEQ_MIDI_EVT 1
#define AL_TEMPO_EVT 3
#define AL_SEQ_END_EVT 4
#define AL_SEQP_STOP_EVT 16
#define AL_TRACK_END 18
#define AL_CSP_LOOPSTART 19
#define AL_CSP_LOOPEND 20
#define AL_PLAYING 1
#define AL_STOPPED 2

/* An ALCSPlayer, as this reads it. */

extern char D_800AA290[];
extern char D_800AA294[];

void alCSeqNextEvent(void *seq, ALEvent *evt);            /* alCSeqNextEvent */
s32 __alCSeqNextDelta(void *seq, s32 *deltaTicks);          /* __alCSeqNextDelta */
void __CSPHandleMIDIMsg(ALCSPlayer *seqp, ALEvent *evt);     /* __CSPHandleMIDIMsg */
void __CSPHandleMetaMsg(ALCSPlayer *seqp, ALEvent *evt);
void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, s32 delta);   /* alEvtqPostEvent */
void __assert(char *file, char *text, s32 line);   /* assertion failure */

static __inline__ void postNextSeqEvent(ALCSPlayer *seqp) {
    ALEvent evt;
    s32 deltaTicks;

    if (seqp->state != AL_PLAYING || seqp->target == NULL) {
        return;
    }
    if (!(u8)__alCSeqNextDelta(seqp->target, &deltaTicks)) {
        return;
    }
    evt.type = AL_SEQ_REF_EVT;
    alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks * seqp->uspt);
}

void __CSPHandleNextSeqEvent(ALCSPlayer *seqp) {
    ALEvent evt;

    if (seqp->target == NULL) {
        return;
    }
    alCSeqNextEvent(seqp->target, &evt);
    switch (evt.type) {
        case AL_SEQ_MIDI_EVT:
            __CSPHandleMIDIMsg(seqp, &evt);
            postNextSeqEvent(seqp);
            break;
        case AL_TEMPO_EVT:
            __CSPHandleMetaMsg(seqp, &evt);
            postNextSeqEvent(seqp);
            break;
        case AL_SEQ_END_EVT:
            seqp->state = AL_STOPPED;
            evt.type = AL_SEQP_STOP_EVT;
            alEvtqPostEvent(&seqp->evtq, &evt, 0x7FFFFFFF);
            break;
        case AL_TRACK_END:
        case AL_CSP_LOOPSTART:
        case AL_CSP_LOOPEND:
            postNextSeqEvent(seqp);
            break;
        default:
            __assert(D_800AA290, D_800AA294, 400);
            break;
    }
}
