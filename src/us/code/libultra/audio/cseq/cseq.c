/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra_inferred/802AD870_7.CPP, func_802AE030_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

struct MeasuredRecAt18 { unsigned char padding[24]; void * value; };
struct MeasuredRecAt0V3 { u8 value; };
u8 __getTrackByte(ALCSeq *seq, u32 track);
u32 __readVarLen(ALCSeq *arg0, u32 arg1);
u32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 index, ALEvent *event);

u32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 index, ALEvent *event) {
    u8 first;
    u8 command;
    u32 first_value;
    u32 command_value;
    first = __getTrackByte(seq, index);
    first_value = first & 0xFF;
    if (first_value == 0xFF) {
        command = __getTrackByte(seq, index);
        command_value = command & 0xFF;
        if (command_value == 0x51) {
            event->type = 3;
            event->msg.tempo.status = first;
            event->msg.tempo.type = command;
            event->msg.tempo.byte1 = __getTrackByte(seq, index);
            event->msg.tempo.byte2 = __getTrackByte(seq, index);
            event->msg.tempo.byte3 = __getTrackByte(seq, index);
            seq->lastStatus[index] = 0;
        } else if (command_value == 0x2F) {
            u32 active = seq->validTracks ^ (1U << index);
            seq->validTracks = active;
            if (active != 0) {
                event->type = 0x12;
            } else {
                event->type = 4;
            }
        } else if (command_value == 0x2E) {
            __getTrackByte(seq, index);
            __getTrackByte(seq, index);
            seq->lastStatus[index] = 0;
            event->type = 0x13;
        } else if (command_value == 0x2D) {
            void *base;
            void *p;
            u8 lead;
            u8 count_byte;
            u32 count;
            u32 byte0;
            u32 byte1;
            u32 byte2;
            u32 byte3;
            void *next;
            base = (u8 *)seq + (index * 4);
            p = ((struct MeasuredRecAt18 *)(base))->value;
            lead = ((struct MeasuredRecAt0V3 *)(p))->value;
            p = (u8 *)p + 1;
            count_byte = ((struct MeasuredRecAt0V3 *)(p))->value;
            count = count_byte & 0xFF;
            if (count == 0) {
                next = (u8 *)p + 5;
                ((struct MeasuredRecAt0V3 *)(p))->value = lead;
            } else {
                if (count != first_value) {
                    ((struct MeasuredRecAt0V3 *)(p))->value = count_byte - 1;
                }
                p = (u8 *)p + 1;
                byte0 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte1 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte2 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte3 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                next = (u8 *)p - ((byte0 << 24) + (byte1 << 16) +
                                   (byte2 << 8) + byte3);
            }
            ((struct MeasuredRecAt18 *)(base))->value = next;
            seq->lastStatus[index] = 0;
            event->type = 0x14;
        }
    } else {
        event->type = 1;
        if (first & 0x80) {
            event->msg.midi.status = first;
            event->msg.midi.byte1 = __getTrackByte(seq, index);
            seq->lastStatus[index] = first;
        } else {
            u8 running_status = seq->lastStatus[index];
            event->msg.midi.byte1 = first;
            event->msg.midi.status = running_status;
        }
        command_value = event->msg.midi.status & 0xF0;
        if ((command_value != 0xC0) && (command_value != 0xD0)) {
            event->msg.midi.byte2 = __getTrackByte(seq, index);
            if ((event->msg.midi.status & 0xF0) == 0x90) {
                event->msg.midi.duration = __readVarLen(seq, index);
            }
        } else {
            event->msg.midi.byte2 = 0;
        }
    }
    return 1;
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/sdk/libgultra_inferred/802AD870_8.CPP,
 * func_802AE290_de, with Turok 2's layouts. libultra's alCSeqNewMarker, with alCSeqNew_28F1F0 and
 * alCSeqNextEvent_28F1F0 inlined. */

#include "common.h"
#include "structs/libaudio.h"
#define AL_SEQ_END_EVT 4
#define AL_TRACK_END 0x12

static __inline__ void alCSeqNew_28F1F0(ALCSeq *seq, u8 *ptr) {
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
            seq->curLoc[i] = (u8 *)((uptr)ptr + tmpOff);
            seq->evtDeltaTicks[i] = __readVarLen(seq, i);
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

static __inline__ void alCSeqNextEvent_28F1F0(ALCSeq *seq, ALEvent *evt) {
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
        seq->evtDeltaTicks[firstTrack] += __readVarLen(seq, firstTrack);
    }
    deltaFlag = &seq->deltaFlag;
    *deltaFlag = 1;
}

/* alCSeqNewMarker: plays a copy of the sequence up to ticks and records its state in m. */
void alCSeqNewMarker(ALCSeq *seq, ALCSeqMarker *m, u32 ticks) {
    ALEvent evt;
    ALCSeq tempSeq;
    s32 i;

    alCSeqNew_28F1F0(&tempSeq, (u8 *)seq->base);
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
        alCSeqNextEvent_28F1F0(&tempSeq, &evt);
        if (evt.type == AL_SEQ_END_EVT) {
            break;
        }
    } while (tempSeq.lastTicks < ticks);
}

u8 __getTrackByte(ALCSeq *seq, u32 track)
{
    u8 theByte;
    if (seq->curBULen[track]) {
        theByte = *seq->curBUPtr[track];
        seq->curBUPtr[track]++;
        seq->curBULen[track]--;
    } else {
        theByte = *seq->curLoc[track];
        seq->curLoc[track]++;
        if (theByte == 0xFE) {
            u8 loBackUp, hiBackUp, theLen, nextByte;
            u32 backup;
            nextByte = *seq->curLoc[track];
            seq->curLoc[track]++;
            if (nextByte != 0xFE) {
                hiBackUp = nextByte;
                loBackUp = *seq->curLoc[track];
                seq->curLoc[track]++;
                theLen = *seq->curLoc[track];
                seq->curLoc[track]++;
                backup = (u32)hiBackUp;
                backup = backup << 8;
                backup += loBackUp;
                seq->curBUPtr[track] = seq->curLoc[track] - (backup + 4);
                seq->curBULen[track] = (u32)theLen;
                theByte = *seq->curBUPtr[track];
                seq->curBUPtr[track]++;
                seq->curBULen[track]--;
            }
        }
    }
    return theByte;
}

void alCSeqNew(RuntimeStateRecAtB8 *state, uptr base) {
    f64 converted;
    s32 count;
    u32 entry;
    u32 i;
    i = 0;
    state->resources = (ResourceTable *)base;
    state->active = 0;
    state->unk10 = 0;
    state->unkC = 0;
    state->one14 = 1;
    do {
        state->flagsA8[i] = 0;
        state->unk58[i] = 0;
        state->flags98[i] = 0;
        entry = state->resources->entries[i];
        if (entry != 0) {
            state->active |= 1U << i;
            state->objects[i] = (void *)(base + entry);
            state->results[i] = __readVarLen((ALCSeq *)state, i);
        } else {
            state->objects[i] = 0;
        }
        i++;
    } while (i < 16U);
    count = state->resources->entries[16];
    converted = (f64)count;
    if (count < 0) {
        converted += 4294967296.0;
    }
    state->scale = 1.0f / (f32)converted;
}

void alCSeqNextEvent(RuntimeState *state, DecodeResult_func_802AE6BC_de *result) {
    u32 minimum;
    u32 index;
    u32 selected;
    u32 decrement;
    minimum = -1;
    index = 0;
    decrement = state->previous;
    do {
        if ((state->active >> index) & 1) {
            if (state->first != 0) {
                state->results[index] -= decrement;
            }
            if (state->results[index] < minimum) {
                minimum = state->results[index];
                selected = index;
            }
        }
        index++;
    } while (index < 16);
    __alCSeqGetTrackEvent((ALCSeq *)state, selected, (ALEvent *)result);
    result->value = minimum;
    state->previous = minimum;
    state->total += minimum;
    if (result->type != 0x12) {
        u32 delta;
        delta = __readVarLen((ALCSeq *)state, selected);
        state->results[selected] += delta;
    }
    state->first = 1;
}

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ s32 unkC;
} Unk0028F710;

s32 alCSeqGetTicks(Unk0028F710 *arg0) {
    return arg0->unkC;
}
f32 alCSeqTicksToSec(void **arg0, s32 arg1, s32 arg2);

f32 alCSeqTicksToSec(void **arg0, s32 arg1, s32 arg2) {
    f64 angle;
    f32 arg1f;
    f32 scaled;
    s32 temp_v0;
    f64 val;
    angle = (f64)arg2;
    arg1f = (f32)arg1;
    if (arg2 < 0) {
        angle += 4294967296.0;
    }
    temp_v0 = ((ObjRecAt40V1 *)((*arg0)))->unk40;
    scaled = arg1f * (f32)angle;
    val = (f64)temp_v0;
    if (temp_v0 < 0) {
        val += 4294967296.0;
    }
    return scaled / ((f32)val * 1.0e6f);
}

typedef struct {
    /* 0x00 */ char pad0[0x40];
    /* 0x40 */ u32 rate;
} TrackInfo;
typedef struct {
    /* 0x00 */ TrackInfo *info;
} Track;

u32 alCSeqSecToTicks(Track *t, f32 scale, u32 div) {
    return (scale * ((f32) t->info->rate * 1000000.0f)) / (f32) div;
}
void alCSeqSetLoc(CopyDest802B3998 *dst, CopySource802B3998 *src);

void alCSeqSetLoc(CopyDest802B3998 *dst, CopySource802B3998 *src) {
    int i;
    dst->word4 = src->word0;
    dst->wordC = src->word4;
    dst->word10 = src->word8;
    for (i = 0; i < 16; i++) {
        dst->words18[i] = src->wordsC[i];
        dst->words58[i] = src->words4C[i];
        dst->bytes98[i] = src->bytes8C[i];
        dst->bytesA8[i] = src->bytes9C[i];
        dst->wordsB8[i] = src->wordsAC[i];
    }
}
void alCSeqGetLoc(CopyDest802B3998 *src, CopySource802B3998 *dst);

void alCSeqGetLoc(CopyDest802B3998 *src, CopySource802B3998 *dst) {
    s32 i;
    dst->word0 = src->word4;
    dst->word4 = src->wordC;
    dst->word8 = src->word10;
    for (i = 0; i < 16; i++) {
        dst->wordsC[i] = src->words18[i];
        dst->words4C[i] = src->words58[i];
        dst->bytes8C[i] = src->bytes98[i];
        dst->bytes9C[i] = src->bytesA8[i];
        dst->wordsAC[i] = src->wordsB8[i];
    }
}

typedef struct {
    char pad0[0x4];
    u32 mask;                   /* 0x04 */
    char pad8[0x10 - 0x8];
    u32 elapsed;                /* 0x10 */
    s32 pending;                /* 0x14 */
    char pad18[0xB8 - 0x18];
    u32 timers[16];             /* 0xB8 */
} Timers;

s32 __alCSeqNextDelta(Timers *t, u32 *out) {
    u32 elapsed = t->elapsed;
    u32 min = -1;
    u32 i;

    if (t->mask == 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        if ((t->mask >> i) & 1) {
            if (t->pending != 0) {
                t->timers[i] -= elapsed;
            }
            if (t->timers[i] < min) {
                min = t->timers[i];
            }
        }
    }
    t->pending = 0;
    *out = min;
    return 1;
}

u32 __readVarLen(ALCSeq *arg0, u32 arg1) {
    sptr temp_v0;
    sptr var_s0;
    var_s0 = (u8) __getTrackByte(arg0, arg1);
    if (var_s0 & 0x80) {
        var_s0 &= 0x7F;
        do {
            temp_v0 = (u8) __getTrackByte(arg0, arg1);
            var_s0 = (var_s0 << 7) + (temp_v0 & 0x7F);
        } while (temp_v0 & 0x80);
    }
    return var_s0;
}
