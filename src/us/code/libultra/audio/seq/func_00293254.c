#include "common.h"
#include "structs/libaudio.h"

s32 __alSeqReadVarLen(ALSeq *seq);

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
