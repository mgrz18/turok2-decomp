#ifndef STRUCTS_CACHE_H
#define STRUCTS_CACHE_H

/* The resource cache (CHashTable__Construct to Texture_DecompressCallback): a CMemEntry per
 * block of loaded data and a CCacheEntry per load request. Names from
 * Turok 3's cache.h (references/turok3/include/cache.h), which has the
 * same objects; Turok 2's CMemEntry has a lock count at 0x08 that Turok 3
 * folded into the flags, so its later fields sit 4 bytes further.
 * The cache's lists start at D_800D81E0: every resident entry (CList,
 * links at 0x18/0x1C) and, from D_800D81F4, the locked ones sorted by
 * address (links at 0x20/0x24). D_800D8DB0 is the cache lock, taken with
 * the D_800D8DCC count.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

struct CCacheEntry;

/* Flags of a CMemEntry. */
#define MEMENTRY_FLAG_LOCKED        0x100   /* lockCount > 0 */
#define MEMENTRY_FLAG_DLISTA_LOCKED 0x200
#define MEMENTRY_FLAG_DLISTB_LOCKED 0x400
#define MEMENTRY_FLAG_ALLOCATED     0x800
#define MEMENTRY_FLAG_ON_LOCKED     0x1000  /* on the locked list D_800D81F4 */

typedef struct CMemEntry {
    /* 0x00 */ void *data;
    /* 0x04 */ s32 size;
    /* 0x08 */ s32 lockCount;       /* users; the last one clears MEMENTRY_FLAG_LOCKED */
    /* 0x0C */ s32 flags;           /* MEMENTRY_FLAG_* (0x702: in use) */
    /* 0x10 */ s32 age;             /* the frame (D_800D8DF0) it was last used */
    /* 0x14 */ struct CCacheEntry *cacheEntry;
    /* 0x18 */ struct CMemEntry *last;
    /* 0x1C */ struct CMemEntry *next;
    /* 0x20 */ struct CMemEntry *lockedLast;
    /* 0x24 */ struct CMemEntry *lockedNext;
} CMemEntry; /* size 0x28 */

typedef struct CCacheEntry {
    /* 0x00 */ CMemEntry *memEntry;
    /* 0x04 */ CMemEntry *referencedMemEntry;
    /* 0x08 */ u32 requestId;       /* the ROM address it loads from */
    /* 0x0C */ u32 requestLength;
    /* 0x10 */ s32 flags;
    /* 0x14 */ void (*decompressCallback)(s32 notifyId, struct CCacheEntry *entry);
    /* 0x18 */ s32 notifyId;
    /* 0x1C */ u32 memFlags;
    /* 0x20 */ void *decompressReplyQueue; /* OSMesgQueue * */
    /* 0x24 */ struct CCacheEntry *next;
} CCacheEntry;

#endif
