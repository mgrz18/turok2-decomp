/* libultra seq.c: alSeqTicksToSec and alSeqSecToTicks, one file. The pair
 * shares .rodata: alSeqSecToTicks pool starts at 0x800AA434, 4 past an 8-byte
 * boundary, and its 2^32 double is 8-aligned only counted from 0x800AA428,
 * where alSeqTicksToSec pool opens with the same double. */
#include "common.h"

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
f32 func_00293034(Seq *seq, s32 ticks, u32 tempo) {
    return ((f32) ticks * (f32) tempo) / ((f32) seq->division * 1000000.0f);
}

/* alSeqSecToTicks */
u32 func_00293080(Seq *seq, f32 sec, u32 tempo) {
    return sec * (seq->division * 1000000.0f) / tempo;
}
