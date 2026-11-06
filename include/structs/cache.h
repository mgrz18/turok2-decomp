#ifndef STRUCTS_CACHE_H
#define STRUCTS_CACHE_H

/* The resource cache (CHashTable__Construct to Texture_DecompressCallback): a CMemEntry per
 * block of loaded data and a CCacheEntry per load request. Names from
 * Turok 3's cache.h (references/turok3/include/cache.h), which has the
 * same objects; Turok 2's CMemEntry has a lock count at 0x08 that Turok 3
 * folded into the flags, so its later fields sit 4 bytes further.
 * The cache's lists start at gCache_memAgeList: every resident entry (CList,
 * links at 0x18/0x1C) and, from gCache_memLockedList, the locked ones sorted by
 * address (links at 0x20/0x24). gCache_mutex is the cache lock, taken with
 * the gCache_mutex_count count.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "ultra_types.h"
#include "structs/list.h"

struct CCacheEntry;

/* Flags of a CMemEntry. */
#define MEMENTRY_FLAG_LOCKED        0x100   /* lockCount > 0 */
#define MEMENTRY_FLAG_DLISTA_LOCKED 0x200
#define MEMENTRY_FLAG_DLISTB_LOCKED 0x400
#define MEMENTRY_FLAG_ALLOCATED     0x800
#define MEMENTRY_FLAG_ON_LOCKED     0x1000  /* on the locked list gCache_memLockedList */

typedef struct CMemEntry {
    /* 0x00 */ void *data;
    /* 0x04 */ s32 size;
    /* 0x08 */ s32 lockCount;       /* users; the last one clears MEMENTRY_FLAG_LOCKED */
    /* 0x0C */ s32 flags;           /* MEMENTRY_FLAG_* (0x702: in use) */
    /* 0x10 */ s32 age;             /* the frame (gCache_ageCount) it was last used */
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

/* Turok 3's mutex.h CBlocker: a one-message queue a thread waits on. */
typedef struct CBlocker {
    /* 0x00 */ OSMesgQueue queue;
    /* 0x18 */ OSMesg msg;
} CBlocker; /* size 0x1C */

#define CACHE_QUEUE_SIZE 64
#define CACHE_STACK_SIZE 0x800

/* The cache itself (gCache, Turok 3's cache.h CCache). Turok 2 has one more
 * CList, memLockedList, after memAgeList, so everything after it sits 0x14
 * bytes further (0x18 from the thread on, which is 8-aligned). Most files
 * reach the fields through their own symbols (gCache_memAgeList, gCache_mutex,
 * ... in globals/cache.h); the ones whose code needs the fields to be read as
 * members of one object use gCache. */
typedef struct CCache {
    /* 0x000 */ CMemEntry *memEntries;
    /* 0x004 */ CMemEntry **memEntriesFree;     /* a stack of freeEntryCount free entries */
    /* 0x008 */ CMemEntry *currentCompressPos;  /* CCache__Compact's position */
    /* 0x00C */ CCacheEntry *cacheEntries;
    /* 0x010 */ CList memAgeList;               /* resident entries by age */
    /* 0x024 */ CList memLockedList;            /* locked entries by address (Turok 2 only) */
    /* 0x038 */ s32 dlistValid[2];
    /* 0x040 */ s32 dlistBlocking[2];
    /* 0x048 */ s32 currentDlist;
    /* 0x050 */ OSThread thread;
    /* 0x200 */ u8 unk200[0x80];
    /* 0x280 */ OSMesgQueue queue;              /* the cache thread's requests */
    /* 0x298 */ OSMesg messages[CACHE_QUEUE_SIZE];
    /* 0x398 */ u64 stack[CACHE_STACK_SIZE / 8];
    /* 0xB98 */ CList cacheFreeList;
    /* 0xBAC */ CList cacheUsedList;
    /* 0xBC0 */ CList cachePendingList;
    /* 0xBD4 */ s32 dlistCount[2];
    /* 0xBDC */ s32 freeEntryCount;
    /* 0xBE0 */ CMutex mutex;                   /* the cache lock */
    /* 0xC00 */ CBlocker blocker;
    /* 0xC1C */ s32 frameDelay;                 /* Turok 3's "synchronous": loads are synchronous while it is not 0 (CCache__Request); counted down by CCache__Advance */
    /* 0xC20 */ u32 ageCount;                   /* the frame stamp CMemEntry.age is compared with */
} CCache; /* size 0xC28 */

#endif
