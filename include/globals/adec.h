#ifndef GLOBALS_ADEC_H
#define GLOBALS_ADEC_H

/* Acclaim's compressed-audio decoder (adec*, alAdecPull) and its MDCT, 0x800B8A20-0x800B8C54. */

#include "common.h"
#include "structs/adec.h"

/* A segment of the Huffman segment table (adecDecodeFrame). */
typedef struct AdecHuffSeg {
    /* 0x00 */ s32 kind;        /* 0 ends the table */
    /* 0x04 */ s32 start;
    /* 0x08 */ s32 len;
    /* 0x0C */ s32 pos;
    /* 0x10 */ void *tree;
} AdecHuffSeg; /* size 0x14 */

extern s32 sAdecFrame;                    /* the frame adecNextFrame returned (alAdecPull) */
extern FftTables gMdctFftTables;   /* mdctFftInit */
extern u16 gAdecBitMasks[17];             /* (1 << n) - 1 (adecReadBits) */
extern u16 gAdecSignBits[17];             /* 1 << (n - 1) (adecReadBits) */
extern u16 gAdecBandEnds[16];             /* band ends, 4 ... 0x100 (adecDequantize) */
extern f32 gAdecStepSizes[64];            /* quantiser steps (adecDequantize) */
extern u16 sAdecBitsLeft;                 /* bits still unread in the current word */
extern u16 sAdecHuffBits;                 /* adecHuffDecode's bit buffer */
extern u16 sAdecHuffBitsAvail;            /* bits left in it */
extern s16 sAdecHeaderDone;               /* the first frame (raw header) is done */
extern s16 sAdecImdctReady;               /* the IMDCT tables are built */
extern u16 sAdecFrameCounter;             /* adecNextFrame's counter (wraps at 6) */
extern AdecHuffSeg gAdecHuffSegments[];   /* the Huffman segment table, ended by kind 0 */

/* The decoder's BSS, 0x80125000-0x80125C4C. */
extern u16 sAdecBitWord;                  /* the current stream word (adecReadBits) */
extern u16 sAdecPrevScale[16];            /* the previous scale indices (adecDecodeFrame) */
extern f32 sMdctPreCos[128];              /* pre-twiddle cos (mdctInitTwiddles) */
extern f32 sMdctPreSin[128];              /* pre-twiddle sin */
extern f32 sMdctPostCosPlusSin[128];      /* post-twiddle cos + sin */
extern f32 sMdctPostSinMinusCos[128];     /* post-twiddle sin - cos */
extern f32 sAdecOverlap[128];             /* the IMDCT overlap */
extern FftTables *sMdctFft;               /* mdctFftInit(0x80) (adecImdctFrame) */
extern DecodeState sAdecState;            /* adecStreamOpen, adecNextFrame */
extern s32 sAdecStopFlag;                 /* sAdecState.count as a symbol of its own (adecGetStopFlag) */

#endif
