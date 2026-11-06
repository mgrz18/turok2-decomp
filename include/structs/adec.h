#ifndef STRUCTS_ADEC_H
#define STRUCTS_ADEC_H

/* Acclaim's compressed-audio decoder (adec*, alAdecPull): the stream state
 * and the FFT tables of its IMDCT. */

#include "common.h"

/* The open stream (adecStreamOpen, adecNextFrame): sAdecState. */
typedef struct DecodeState {
    /* 0x00 */ s32 first;
    /* 0x04 */ s32 third;
    /* 0x08 */ void *source;  /* the stream's header */
    /* 0x0C */ s32 second;
    /* 0x10 */ s32 count;     /* frames left (sAdecStopFlag is this field's own symbol) */
    /* 0x14 */ void *cursor;  /* the read position */
} DecodeState; /* size 0x18 */

/* The FFT tables (mdctFftInit): n/2 twiddles and the bit-reversal order. */
typedef struct FftTables {
    /* 0x0 */ f32 *cos;
    /* 0x4 */ f32 *sin;      /* -sin */
    /* 0x8 */ s32 *bitrev;
    /* 0xC */ s32 n;
} FftTables; /* size 0x10 */

#endif
