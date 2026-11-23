#ifndef STRUCTS_AUDIOMGR_H
#define STRUCTS_AUDIOMGR_H

/* The SDK audio manager (the libultra sample amaudio.c, Acclaim's copy: amCreateAudioMgr,
 * __amMain, __amHandleFrameMsg, __amDmaNew, __amDMA, __clearAudioDMA) and Turok's DMA
 * cache (AudioDMA_CacheBlock). The globals are globals/audiomgr.h's. */

#include "common.h"
#include "structs/libaudio.h"
#include "structs/sched.h"

/* A 0x300-byte window of the ROM's sound data (SDK DMABuffer). */
typedef struct AMDMABuffer {
    /* 0x00 */ ALLink node;        /* the used list in ROM order, or the free list */
    /* 0x08 */ u32 startAddr;      /* the ROM address of ptr[0] */
    /* 0x0C */ u32 lastFrame;      /* audFrameCt when last read; freed two frames later */
    /* 0x10 */ u8 *ptr;            /* 0x300 bytes from the audio heap */
} AMDMABuffer; /* size 0x14 */

/* __amDmaNew's state (SDK DMAState). */
typedef struct AMDMAState {
    /* 0x0 */ u8 initialized;
    /* 0x4 */ AMDMABuffer *firstUsed;
    /* 0x8 */ AMDMABuffer *firstFree;
} AMDMAState; /* size 0xC */

/* One output buffer's frame (SDK AMAudioInfo): the samples and the RSP task that makes them. */
typedef struct AMAudioInfo {
    /* 0x00 */ s16 *data;
    /* 0x04 */ s16 frameSamples;
    /* 0x08 */ OSScTask task;      /* list.t.data_ptr/data_size: the command list */
} AMAudioInfo; /* size 0x70 */

/* Turok's addition: a block of sound data copied to RDRAM (AudioDMA_CacheBlock); __amMain
 * counts each block's state down once a frame. */
typedef struct AMCacheEntry {
    /* 0x00 */ struct AMCacheEntry *next;
    /* 0x04 */ void *key;          /* the block's address */
    /* 0x08 */ u16 state;          /* 3 when used, -1 each frame */
    /* 0x0A */ u16 size;
    /* 0x0C */ s32 padC;
    /* 0x10 */ u8 data[0x80];
} AMCacheEntry; /* size 0x90 */

#endif
