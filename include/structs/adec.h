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

/* Acclaim compressed-audio stream decoder state (one instance: adecReadBits, adecDecodeFrame,
 * adecImdctFrame). */
typedef struct {
    /* 0x00 */ u32 hdr0;
    /* 0x04 */ u32 hdr2;
    /* 0x08 */ u32 *hdr;
    /* 0x0C */ u32 nframes;
    /* 0x10 */ u32 remaining;
    /* 0x14 */ u16 *rd;         /* bitstream read pointer */
    /* 0x18 */ s16 band[16];    /* per-band scale indices of the current frame */
    /* 0x38 */ u8 pad38[0xB8 - 0x38];
    /* 0xB8 */ s16 coef[256];   /* quantized coefficients of the current frame */
} AdecStream;

#endif
