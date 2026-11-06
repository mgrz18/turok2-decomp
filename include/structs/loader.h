#ifndef STRUCTS_LOADER_H
#define STRUCTS_LOADER_H

/* The cartridge reader thread (CLoader__Construct to CLoader__AllocLoaderEntry;
 * Turok 3's loader.h, references/turok3/include/loader.h). A load is cut
 * into CLoaderEntry requests of at most LOADER_CHUNK_SIZE bytes sent to
 * queue; CLoader__Main DMAs each in, waits on piReplyQueue (OS_EVENT_PI)
 * and sends replyMessage to the entry's replyQueue. Turok 2's OSThread is
 * 0x1B0 bytes (Turok 3's view has 0x1C0), so everything after the thread
 * sits 0x10 to 0x18 bytes lower than in Turok 3.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "ultra_types.h"
#include "structs/list.h"

#define LOADER_QUEUE_SIZE   512
#define LOADER_ENTRY_COUNT  (LOADER_QUEUE_SIZE + 16)
#define LOADER_DMA_COMPLETE_MESSAGE 2001
#define LOADER_CHUNK_SIZE   0x2000
#define LOADER_STACK_SIZE   0x400

typedef struct CLoaderEntry {
    /* 0x00 */ u32 address;                 /* ROM address */
    /* 0x04 */ void *dest;
    /* 0x08 */ s32 length;
    /* 0x0C */ OSMesgQueue *replyQueue;
    /* 0x10 */ OSMesg replyMessage;
    /* 0x14 */ u32 flags;                   /* 1 while allocated */
    /* 0x18 */ struct CLoaderEntry *prev;
    /* 0x1C */ struct CLoaderEntry *next;
} CLoaderEntry; /* size 0x20 */

typedef struct CLoader {
    /* 0x0000 */ OSThread thread;
    /* 0x01B0 */ u8 unk1B0[0x80];
    /* 0x0230 */ OSMesgQueue queue;
    /* 0x0248 */ OSMesg messages[LOADER_QUEUE_SIZE];
    /* 0x0A48 */ OSMesgQueue piReplyQueue;
    /* 0x0A60 */ OSMesg piReplyMessage;
    /* 0x0A68 */ u64 stack[LOADER_STACK_SIZE / 8];  /* u64, hence the pad at 0xA64 */
    /* 0x0E68 */ CLoaderEntry entries[LOADER_ENTRY_COUNT];
    /* 0x5068 */ CList freeList;
    /* 0x507C */ CList usedList;
} CLoader; /* size 0x5090 */

#endif
