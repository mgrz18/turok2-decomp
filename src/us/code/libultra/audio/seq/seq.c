/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seq.c, alSeqNextEvent.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void alSeqNextEvent(ALSeq *seq, ALEvent *event);
u8 __alSeqReadByte(u8 **arg0);
typedef struct {
    /* 0x0 */ char pad[8];
    /* 0x8 */ u8 *p;
} Reader;
u32 __alSeqReadVarLen(Reader *r);

void alSeqNextEvent(ALSeq *seq, ALEvent *event) {
    u8 status;
    u8 type;
    s32 deltaTicks;
    deltaTicks = __alSeqReadVarLen(seq);
    seq->lastTicks += deltaTicks;
    status = __alSeqReadByte(seq);
    if (status == 0xFF) {
        type = __alSeqReadByte(seq);
        if (type == 0x51) {
            event->type = 3;
            event->msg.tempo.ticks = deltaTicks;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.len = __alSeqReadByte(seq);
            event->msg.tempo.byte1 = __alSeqReadByte(seq);
            event->msg.tempo.byte2 = __alSeqReadByte(seq);
            event->msg.tempo.byte3 = __alSeqReadByte(seq);
        } else if (type == 0x2F) {
            event->type = 4;
            event->msg.tempo.ticks = deltaTicks;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.len = __alSeqReadByte(seq);
        }
        seq->lastStatus = 0;
    } else {
        event->type = 1;
        event->msg.midi.ticks = deltaTicks;
        if (status & 0x80) {
            event->msg.midi.status = status;
            event->msg.midi.byte1 = __alSeqReadByte(seq);
            seq->lastStatus = status;
        } else {
            event->msg.midi.status = seq->lastStatus;
            event->msg.midi.byte1 = status;
        }
        if ((event->msg.midi.status & 0xF0) != 0xC0 && (event->msg.midi.status & 0xF0) != 0xD0) {
            event->msg.midi.byte2 = __alSeqReadByte(seq);
        } else {
            event->msg.midi.byte2 = 0;
        }
    }
}

#include "common.h"

int __alSeqReadWord(void *arg0);
/* MIDI file reader state (checks the "MThd" and "MTrk" chunk tags). */
typedef struct {
    /* 0x00 */ s32 data;
    /* 0x04 */ s32 trackStart;
    /* 0x08 */ s32 cursor;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 tickScale;
    /* 0x18 */ s16 division;
    /* 0x1A */ s16 unk1A;
} MidiReader;
s16 __alSeqReadShort(u8 **arg0);

void alSeqNew(MidiReader *arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;

    arg0->data = arg1;
    arg0->unk10 = arg2;
    arg0->unk1A = 0;
    arg0->unkC = 0;
    arg0->cursor = arg1;
    if (__alSeqReadWord((sptr) arg0) == 0x4D546864) {
        __alSeqReadWord((sptr) arg0);
        if (((__alSeqReadShort(arg0) << 0x10) == 0) && (__alSeqReadShort(arg0) == 1)) {
            temp_v0 = __alSeqReadShort(arg0);
            arg0->division = temp_v0;
            if (!(temp_v0 & 0x8000)) {
                arg0->tickScale = (f32) (1.0f / (f32) temp_v0);
                if (__alSeqReadWord((sptr) arg0) == 0x4D54726B) {
                    __alSeqReadWord((sptr) arg0);
                    arg0->trackStart = arg0->cursor;
                }
            }
        }
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
} Arg;

s32 alSeqGetTicks(Arg *arg) {
    return arg->unkC;
}

typedef struct {
    /* 0x00 */ u8 *base;
    /* 0x04 */ u8 *trackStart;
    /* 0x08 */ u8 *curPtr;
    /* 0x0C */ s32 lastTicks;
    /* 0x10 */ s32 len;
    /* 0x14 */ f32 qnpt;
    /* 0x18 */ s16 division;
    /* 0x1A */ s16 lastStatus;
} Seq;

/* alSeqTicksToSec */
f32 alSeqTicksToSec(Seq *seq, s32 ticks, u32 tempo) {
    return ((f32) ticks * (f32) tempo) / ((f32) seq->division * 1000000.0f);
}

/* alSeqSecToTicks */
u32 alSeqSecToTicks(Seq *seq, f32 sec, u32 tempo) {
    return sec * (seq->division * 1000000.0f) / tempo;
}
void alSeqNewMarker(ALSeq *seq, ALSeqMarker *m, u32 ticks);

void alSeqNewMarker(ALSeq *seq, ALSeqMarker *m, u32 ticks) {
    ALEvent evt;
    u8 *ptr;
    u16 status;
    s32 lastTicks;
    u8 *savePtr;
    s32 saveTicks;
    u16 saveStatus;
    if (ticks == 0) {
        m->curPtr = seq->trackStart;
        m->lastStatus = 0;
        m->lastTicks = 0;
        m->curTicks = 0;
    } else {
        savePtr = seq->curPtr;
        saveStatus = seq->lastStatus;
        saveTicks = seq->lastTicks;
        seq->lastStatus = 0;
        seq->lastTicks = 0;
        seq->curPtr = seq->trackStart;
        do {
            ptr = seq->curPtr;
            status = seq->lastStatus;
            lastTicks = seq->lastTicks;
            alSeqNextEvent(seq, &evt);
            if (evt.type == 4) {
                ptr = seq->curPtr;
                status = seq->lastStatus;
                lastTicks = seq->lastTicks;
                break;
            }
        } while (seq->lastTicks < ticks);
        m->curPtr = ptr;
        m->lastStatus = status;
        m->lastTicks = lastTicks;
        m->curTicks = seq->lastTicks;
        seq->curPtr = savePtr;
        seq->lastStatus = saveStatus;
        seq->lastTicks = saveTicks;
    }
}

typedef struct {
    /* 0x0 */ s32 a;
    /* 0x4 */ s32 b;
    /* 0x8 */ s32 c;
    /* 0xC */ u16 d;
} Src29;
typedef struct {
    /* 0x00 */ char pad0[8];
    /* 0x08 */ s32 a;
    /* 0x0C */ s32 b;
    /* 0x10 */ char pad10[0xA];
    /* 0x1A */ u16 d;
} Dst29;

void alSeqSetLoc(Dst29 *dst, Src29 *src) {
    dst->a = src->a;
    dst->d = src->d;
    dst->b = src->b;
}

/* The source record: words at 8 and 0xC, a halfword at 0x1A. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1A - 0x10];
    /* 0x1A */ u16 unk1A;
} CopySource;
/* What alSeqGetLoc fills from it. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ u16 unkC;
} CopyDest;

void alSeqGetLoc(CopySource *arg0, CopyDest *arg1) {
    arg1->unk0 = arg0->unk8;
    arg1->unkC = arg0->unk1A;
    arg1->unk4 = arg0->unkC;
}

#include "structs/libaudio.h"

/* libaudio seq.c (2.0I), as in ultralib: the delta time of the next event, read without moving
 * the sequence; FALSE at the end of the data. readVarLen is __alSeqReadVarLen here (not inlined).
 * Round 22 de-faked it: the function returns char (the m2c draft returned s32 and needed two
 * equal arms). */
char __alSeqNextDelta(ALSeq *seq, s32 *pDeltaTicks) {
    u8 *savedPtr;

    /* sct 1/16/96 - Put in safety check here to make sure we don't read past sequence data. */
    if (seq->curPtr >= seq->base + seq->len)
        return 0;

    savedPtr = seq->curPtr;
    *pDeltaTicks = __alSeqReadVarLen(seq); /* read the delta time */
    seq->curPtr = savedPtr;

    return 1;
}

/* Reads a MIDI-style variable-length quantity: 7 bits per byte, high bit set
 * on every byte but the last. */
u32 __alSeqReadVarLen(Reader *r) {
    u32 v;
    u32 c;
    c = *r->p++;
    v = c;
    if (v & 0x80) {
        v &= 0x7F;
        do {
            v <<= 7;
            c = *r->p++;
            v += c & 0x7F;
        } while (c & 0x80);
    }
    return v;
}

#include "m2c_macros.h"

u8 __alSeqReadByte(u8 **arg0) {
    return *arg0[2]++;
}

s16 __alSeqReadShort(u8 **arg0) {
    s32 hi = *arg0[2]++;
    s32 lo = *arg0[2]++;

    return (hi << 8) | lo;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B6F50_2.c, func_802B23F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

int __alSeqReadWord(void *arg0) {
    ALSeq *stream = arg0;
    unsigned char *p;
    unsigned int b0, b1, b2, b3;
    p = stream->curPtr;
    b0 = p[0];
    p = p + 1;
    stream->curPtr = p;
    b1 = p[0];
    stream->curPtr = p + 1;
    b2 = p[1];
    stream->curPtr = p + 2;
    b3 = p[2];
    stream->curPtr = p + 3;
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}
