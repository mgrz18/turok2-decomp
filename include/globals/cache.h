#ifndef GLOBALS_CACHE_H
#define GLOBALS_CACHE_H

/* The resource cache (Turok 3's CCache gCache, its fields separate symbols
 * here: Turok 2's CCache has one more CList after memAgeList) and its hash table. */

#include "common.h"
#include "ultra_types.h"
#include "structs/list.h"
#include "structs/cache.h"
#include "structs/hash.h"

extern CCache gCache;                       /* the fields below are also symbols of their own */
extern u32 gHashMask;                       /* the bucket mask of every lookup */
extern u32 gHashTableMask;                  /* CHashTable__Insert's probe wrap */
extern HashEntry *gHashTable;               /* gHashTableSize entries (CHashTable__Construct) */
extern CMemEntry **gCache_memEntriesFree;
extern CMemEntry *gCache_currentCompressPos;
extern CList gCache_memAgeList;             /* resident entries by age */
extern CList gCache_memLockedList;          /* locked entries by address (Turok 2 only) */
extern s32 gCache_dlistValid[2];
extern s32 gCache_dlistBlocking[2];
extern s32 gCache_currentDlist;
extern OSMesgQueue gCache_queue;            /* the cache thread's requests */
extern CList gCache_cacheFreeList;
extern CList gCache_cacheUsedList;
extern CList gCache_cachePendingList;
extern s32 gCache_dlistCount[2];
extern s32 gCache_freeEntryCount;
extern CMutex gCache_mutex;                 /* the cache lock */
extern s32 gCache_mutex_count;              /* gCache_mutex.count, kept: the code reaches it by its own %hi/%lo */
extern s32 gCache_frameDelay;
extern u32 gCache_ageCount;                 /* the frame stamp CMemEntry.age is compared with */

/* The cache's sizes (Turok 3's hash.c / cache.c). */

extern s32 gHashSize;           /* RoundToPowerOfTwo(entries) (CHashTable__Construct) */
extern u32 gHashTableSize;      /* gHashSize * 2 */
extern u32 gCacheEntryCount;    /* 0x400, doubled with the expansion pak */

#endif
