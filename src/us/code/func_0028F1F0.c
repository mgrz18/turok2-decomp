/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/sdk/libgultra_inferred/802AD870_8.CPP,
 * func_802AE290_de, with Turok 2's layouts. libultra's alCSeqNewMarker, with alCSeqNew and
 * alCSeqNextEvent inlined. */

#include "common.h"

#define AL_SEQ_END_EVT 4
#define AL_TRACK_END 0x12

typedef struct {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
} ALMIDIEvent;

typedef struct {
    s16 type;
    union {
        ALMIDIEvent midi;
    } msg;
} ALEvent;

typedef struct {
    u32 trackOffset[16];
    u32 division;
} ALCMidiHdr;

typedef struct {
    ALCMidiHdr *base;
    u32 validTracks;
    f32 qnpt;
    u32 lastTicks;
    u32 lastDeltaTicks;
    u32 deltaFlag;
    u8 *curLoc[16];
    u8 *curBUPtr[16];
    u8 curBULen[16];
    u8 lastStatus[16];
    u32 evtDeltaTicks[16];
} ALCSeq;

typedef struct {
    u32 validTracks;
    s32 lastTicks;
    u32 lastDeltaTicks;
    u8 *curLoc[16];
    u8 *curBUPtr[16];
    u8 curBULen[16];
    u8 lastStatus[16];
    u32 evtDeltaTicks[16];
} ALCSeqMarker;

u32 func_0028F98C(ALCSeq *seq, u32 track);     /* __alCSeqNextDelta */
u32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 track, ALEvent *event);

static __inline__ void alCSeqNew(ALCSeq *seq, u8 *ptr) {
    u32 i, tmpOff, flagTmp;
    f64 t;
    s32 division;
    u32 *deltaFlag;

    seq->base = (ALCMidiHdr *)ptr;
    seq->validTracks = 0;
    seq->lastDeltaTicks = 0;
    seq->lastTicks = 0;
    deltaFlag = &seq->deltaFlag;
    *deltaFlag = 1;
    for (i = 0; i < 16; i++) {
        seq->lastStatus[i] = 0;
        seq->curBUPtr[i] = 0;
        seq->curBULen[i] = 0;
        tmpOff = seq->base->trackOffset[i];
        if (tmpOff) {
            flagTmp = 1 << i;
            seq->validTracks |= flagTmp;
            seq->curLoc[i] = (u8 *)((u32)ptr + tmpOff);
            seq->evtDeltaTicks[i] = func_0028F98C(seq, i);
        } else {
            seq->curLoc[i] = 0;
        }
    }
    division = seq->base->division;
    t = (f64)division;
    if (division < 0) {
        t += 4294967296.0;
    }
    seq->qnpt = 1.0f / (f32)t;
}

static __inline__ void alCSeqNextEvent(ALCSeq *seq, ALEvent *evt) {
    u32 i;
    u32 firstTime = 0xFFFFFFFF;
    u32 firstTrack;
    u32 lastTicks = seq->lastDeltaTicks;
    u32 *deltaFlag;

    for (i = 0; i < 16; i++) {
        if ((seq->validTracks >> i) & 1) {
            if (seq->deltaFlag) {
                seq->evtDeltaTicks[i] -= lastTicks;
            }
            if (seq->evtDeltaTicks[i] < firstTime) {
                firstTime = seq->evtDeltaTicks[i];
                firstTrack = i;
            }
        }
    }
    __alCSeqGetTrackEvent(seq, firstTrack, evt);
    evt->msg.midi.ticks = firstTime;
    seq->lastTicks += firstTime;
    seq->lastDeltaTicks = firstTime;
    if (evt->type != AL_TRACK_END) {
        seq->evtDeltaTicks[firstTrack] += func_0028F98C(seq, firstTrack);
    }
    deltaFlag = &seq->deltaFlag;
    *deltaFlag = 1;
}

/* alCSeqNewMarker: plays a copy of the sequence up to ticks and records its state in m. */
void func_0028F1F0(ALCSeq *seq, ALCSeqMarker *m, u32 ticks) {
    ALEvent evt;
    ALCSeq tempSeq;
    s32 i;

    alCSeqNew(&tempSeq, (u8 *)seq->base);
    do {
        m->validTracks = tempSeq.validTracks;
        m->lastTicks = tempSeq.lastTicks;
        m->lastDeltaTicks = tempSeq.lastDeltaTicks;
        for (i = 0; i < 16; i++) {
            m->curLoc[i] = tempSeq.curLoc[i];
            m->curBUPtr[i] = tempSeq.curBUPtr[i];
            m->curBULen[i] = tempSeq.curBULen[i];
            m->lastStatus[i] = tempSeq.lastStatus[i];
            m->evtDeltaTicks[i] = tempSeq.evtDeltaTicks[i];
        }
        alCSeqNextEvent(&tempSeq, &evt);
        if (evt.type == AL_SEQ_END_EVT) {
            break;
        }
    } while (tempSeq.lastTicks < ticks);
}
