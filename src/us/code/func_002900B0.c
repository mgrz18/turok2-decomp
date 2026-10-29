/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/sdk/libgultra_inferred/802B3B80_4.CPP,
 * func_802AF150_de, with Turok 2's layouts. libultra's __CSPHandleNextSeqEvent (cspplayer.c)
 * with __CSPPostNextSeqEvent inlined. */

#include "common.h"

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

typedef struct {
    s16 type;
    u8 msg[0xE];
} ALEvent;

typedef struct {
    u8 data[0x1C];
} ALEventQueue;

/* An ALCSPlayer, as this reads it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ void *target;            /* ALCSeq */
    /* 0x1C */ u8 pad1C[0x8];
    /* 0x24 */ s32 uspt;
    /* 0x28 */ s32 nextDelta;
    /* 0x2C */ s32 state;
    /* 0x30 */ u8 pad30[0x18];
    /* 0x48 */ ALEventQueue evtq;
} ALCSPlayer;

extern char D_800AA290[];
extern char D_800AA294[];

void func_0028F61C(void *seq, ALEvent *evt);            /* alCSeqNextEvent */
s32 func_0028F910(void *seq, s32 *deltaTicks);          /* __alCSeqNextDelta */
void func_00290240(ALCSPlayer *seqp, ALEvent *evt);     /* __CSPHandleMIDIMsg */
void __CSPHandleMetaMsg(ALCSPlayer *seqp, ALEvent *evt);
void func_00291034(ALEventQueue *evtq, ALEvent *evt, s32 delta);   /* alEvtqPostEvent */
void func_0029B020(char *file, char *text, s32 line);   /* assertion failure */

static __inline__ void postNextSeqEvent(ALCSPlayer *seqp) {
    ALEvent evt;
    s32 deltaTicks;

    if (seqp->state != AL_PLAYING || seqp->target == NULL) {
        return;
    }
    if (!(u8)func_0028F910(seqp->target, &deltaTicks)) {
        return;
    }
    evt.type = AL_SEQ_REF_EVT;
    func_00291034(&seqp->evtq, &evt, deltaTicks * seqp->uspt);
}

void func_002900B0(ALCSPlayer *seqp) {
    ALEvent evt;

    if (seqp->target == NULL) {
        return;
    }
    func_0028F61C(seqp->target, &evt);
    switch (evt.type) {
        case AL_SEQ_MIDI_EVT:
            func_00290240(seqp, &evt);
            postNextSeqEvent(seqp);
            break;
        case AL_TEMPO_EVT:
            __CSPHandleMetaMsg(seqp, &evt);
            postNextSeqEvent(seqp);
            break;
        case AL_SEQ_END_EVT:
            seqp->state = AL_STOPPED;
            evt.type = AL_SEQP_STOP_EVT;
            func_00291034(&seqp->evtq, &evt, 0x7FFFFFFF);
            break;
        case AL_TRACK_END:
        case AL_CSP_LOOPSTART:
        case AL_CSP_LOOPEND:
            postNextSeqEvent(seqp);
            break;
        default:
            func_0029B020(D_800AA290, D_800AA294, 400);
            break;
    }
}
