#ifndef STRUCTS_DECOMPRESSOR_H
#define STRUCTS_DECOMPRESSOR_H

/* The decompressor thread (CDecompressor__Construct to CDecompressor__Flush;
 * Turok 3's decompressor.h). Cache entries (CCacheEntry) are sent to queue;
 * CDecompressor__Main runs each entry's decompressCallback and replies to
 * its decompressReplyQueue. CDecompressor__Flush keeps the thread at the
 * high priority while queue.validCount is not 0.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "ultra_types.h"

#define DECOMPRESSOR_QUEUE_SIZE 128
#define DECOMPRESSOR_STACK_SIZE 0x1000

typedef struct CDecompressor {
    /* 0x0000 */ OSThread thread;
    /* 0x01B0 */ u8 unk1B0[0x80];
    /* 0x0230 */ OSMesgQueue queue;
    /* 0x0248 */ OSMesg messages[DECOMPRESSOR_QUEUE_SIZE];
    /* 0x0448 */ u64 stack[DECOMPRESSOR_STACK_SIZE / 8];
    /* 0x1448 */ u32 lastDecompression;     /* gFrameCount when it last took an entry */
} CDecompressor; /* size 0x1450 */

#endif
