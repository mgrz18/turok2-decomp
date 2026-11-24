/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00250E10.c, func_00250E10.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/list.h"
#include "globals/cache.h"
#include "globals/engine.h"
#include "globals/sysrc.h"
#include "protos/list.h"
#include "protos/cache.h"
#include "protos/ultra.h"
#include "protos/osint.h"
#include "protos/heap.h"
#include "protos/hash.h"
void *memset(void *s, int c, uptr n);
void CMutex__Construct(void *lock);
u32 GetMemSize(void);
#include "structs/cache.h"
#ifdef TARGET_PORT

/* The same construction as the N64 code below, in the natural form (indexed loops, the
 * dlistValid stores in order). Both write gCache's members; the N64 code was a B12Mgr view
 * from 0x800D7BE0 = gCache - 0x5F0 (Rage Wars' layout) until round 25: the ROM never loads
 * that address, each access is its own %hi/%lo of gCache + the member offset, so the typed
 * members give the same code. The N64 form keeps the running-pointer loops and the dlistValid
 * stores through a pointer, which the ROM's register use needs. */
void CCache__Construct(s32 unused, s32 id) {
    u32 mask;
    u32 i;
    CMutex__Construct(&gCache.mutex);
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    gCacheEntryCount = 0x400;
    if (GetMemSize() > 0x400000) {
        gCacheEntryCount *= 2;
    }
    gCache.memEntries = (CMemEntry *)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CMemEntry));
    gCache.memEntriesFree = (CMemEntry **)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CMemEntry *));
    gCache.cacheEntries = (CCacheEntry *)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CCacheEntry));
    osCreateMesgQueue(&gCache.blocker.queue, &gCache.blocker.msg, 1);
    gCache.dlistBlocking[1] = 0;
    gCache.dlistBlocking[0] = 0;
    CList__Construct(&gCache.memAgeList, T2_OFFSETOF(CMemEntry, last), T2_OFFSETOF(CMemEntry, next));
    CList__Construct(&gCache.memLockedList, T2_OFFSETOF(CMemEntry, lockedLast), T2_OFFSETOF(CMemEntry, lockedNext));
    gCache.freeEntryCount = 0;
    for (i = 0; i < gCacheEntryCount; i++) {
        gCache.memEntries[i].flags = 0;
        gCache.memEntriesFree[i] = &gCache.memEntries[i];
        gCache.freeEntryCount++;
    }
    gCache.dlistValid[1] = 1;
    gCache.dlistValid[0] = 1;
    gCache.currentDlist = -1;
    gCache.dlistCount[0] = 0;
    gCache.dlistCount[1] = 0;
    gCache.currentCompressPos = 0;
    gCache.frameDelay = 0; /* 1 in Rage Wars */
    gCache.ageCount = 0;
    CList__Construct(&gCache.cacheFreeList, T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    CList__Construct(&gCache.cacheUsedList, T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    CList__Construct(&gCache.cachePendingList, T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    for (i = 0; i < gCacheEntryCount; i++) {
        gCache.cacheEntries[i].flags = 0;
        CList__AddTail(&gCache.cacheFreeList, &gCache.cacheEntries[i]);
    }
    CHashTable__Construct(gCacheEntryCount);
    osCreateMesgQueue(&gCache.queue, gCache.messages, CACHE_QUEUE_SIZE);
    mask = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    memset(gCache.stack, (u8)id, sizeof(gCache.stack));
    osCreateThread(&gCache.thread, id, CCache__Main, 0, (u8 *)gCache.stack + sizeof(gCache.stack), gThreadPriorityCache);
    osStartThread(&gCache.thread);
}
#else
void CCache__Construct(s32 unused, s32 id) {
    u32 mask;
    u32 m2;
    u32 i;
    CCacheEntry *n;
    CMemEntry *n1;
    u32 i1;
    s32 *t;
    CMutex__Construct(&gCache.mutex);
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    gCacheEntryCount = 0x400;
    if (GetMemSize() > 0x400000) {
        gCacheEntryCount *= 2;
    }
    gCache.memEntries = (CMemEntry *)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CMemEntry));
    gCache.memEntriesFree = (CMemEntry **)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CMemEntry *));
    gCache.cacheEntries = (CCacheEntry *)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CCacheEntry));
    osCreateMesgQueue(&gCache.blocker.queue, &gCache.blocker.msg, 1);
    gCache.dlistBlocking[1] = 0;
    gCache.dlistBlocking[0] = 0;
    CList__Construct(&gCache.memAgeList, T2_OFFSETOF(CMemEntry, last), T2_OFFSETOF(CMemEntry, next));
    CList__Construct(&gCache.memLockedList, T2_OFFSETOF(CMemEntry, lockedLast), T2_OFFSETOF(CMemEntry, lockedNext));
    gCache.freeEntryCount = 0;
    for (i = 0; i < gCacheEntryCount; i++) {
        n1 = gCache.memEntries;
        n1 = (CMemEntry *)((u8 *)n1 + i * sizeof(CMemEntry));
        n1->flags = 0;
        gCache.memEntriesFree[i] = n1;
        gCache.freeEntryCount++;
    }
    t = gCache.dlistValid;
    t[1] = 1;
    t[0] = 1;
    gCache.currentDlist = -1;
    gCache.dlistCount[0] = 0;
    gCache.dlistCount[1] = 0;
    gCache.currentCompressPos = 0;
    gCache.frameDelay = 0; /* 1 in Rage Wars */
    gCache.ageCount = 0;
    CList__Construct(&gCache.cacheFreeList, T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    CList__Construct(&gCache.cacheUsedList, T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    CList__Construct(&gCache.cachePendingList, T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    for (i = 0; i < gCacheEntryCount; i++) {
        n = gCache.cacheEntries;
        n = (CCacheEntry *)((u8 *)n + i * sizeof(CCacheEntry));
        n->flags = 0;
        CList__AddTail(&gCache.cacheFreeList, n);
    }
    CHashTable__Construct(gCacheEntryCount);
    osCreateMesgQueue(&gCache.queue, gCache.messages, CACHE_QUEUE_SIZE);
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
    memset(gCache.stack, (u8)id, sizeof(gCache.stack));
    osCreateThread(&gCache.thread, id, CCache__Main, 0, (u8 *)gCache.stack + sizeof(gCache.stack), gThreadPriorityCache);
    osStartThread(&gCache.thread);
}
#endif
