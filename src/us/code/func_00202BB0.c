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
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
void *memset(void *s, int c, uptr n);
typedef struct B12Node B12Node;
struct B12Node {
               u32 key;
               s32 unk4;
               s32 refs;
               u32 flags;
               u32 unk10;
               B12Node *link;
               B12Node *link18;
               u8 pad1C[4];
               B12Node *prev;
               B12Node *next;
};
typedef struct B12Bucket B12Bucket;
struct B12Bucket {
              u32 unk0;
              u32 unk4;
              s32 id;
              B12Bucket *next;
};
typedef struct B12Mgr {
                u8 pad0[0x5F0];
                B12Node * nodes;
                B12Node ** stk;
                B12Node * cur;
                u8 * pool2;
                CList lists[2];
                s32 tbl628[2];
                s32 tbl630[2];
                s32 f638;
                u8 pad63C[0x4];
                OSThread thread;
                u8 pad7F0[0x80];
                OSMesgQueue q870;
                OSMesg buf870[0x40];
                u8 stack[0x800];
                 CList lists3[3];
                 s32 tbl11C4[2];
                 s32 cnt;
                 OSMesgQueue q11D0;
                 OSMesg buf11D0;
                 s32 lockCnt;
                 OSMesgQueue q11F0;
                 OSMesg buf11F0;
                 s32 f120C;
                 u32 stamp;
                 u8 pad1214[0xC];
                 u32 hashMask;
                 B12Bucket * buckets;
} B12Mgr;
extern B12Mgr D_800D7BE0;
void CMutex__Construct(void *lock);
u32 GetMemSize(void);
u8 *CHeap__Alloc(CHeap *heap, u32 size);
void CHashTable__Construct(u32 n);
#ifdef TARGET_PORT
#include "structs/cache.h"

/* The same construction on the typed CCache (structs/cache.h). B12Mgr is a view from
 * D_800D7BE0 = gCache - 0x5F0 with N64 field offsets; on the host D_800D7BE0 is not even next
 * to gCache (gendata places it inside gDecompressor), so the free stack, the lists and the
 * thread would land in the wrong object. Field for field: nodes = memEntries, stk =
 * memEntriesFree, cur = currentCompressPos, pool2 = cacheEntries, lists = memAgeList /
 * memLockedList, tbl628 = dlistValid, tbl630 = dlistBlocking, f638 = currentDlist, q870 = queue,
 * lists3 = cacheFreeList / cacheUsedList / cachePendingList, tbl11C4 = dlistCount, cnt =
 * freeEntryCount, q11D0 = mutex, q11F0 = blocker, f120C = frameDelay, stamp = ageCount. */
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
    B12Node *n;
    B12Node *n1;
    u32 i1;
    s32 *t;
    CMutex__Construct(&D_800D7BE0.q11D0);
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    gCacheEntryCount = 0x400;
    if (GetMemSize() > 0x400000) {
        gCacheEntryCount *= 2;
    }
    D_800D7BE0.nodes = (B12Node *)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CMemEntry));
    D_800D7BE0.stk = (B12Node **)CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CMemEntry *));
    D_800D7BE0.pool2 = CHeap__Alloc(&gHeap, gCacheEntryCount * sizeof(CCacheEntry));
    osCreateMesgQueue(&D_800D7BE0.q11F0, &D_800D7BE0.buf11F0, 1);
    D_800D7BE0.tbl630[1] = 0;
    D_800D7BE0.tbl630[0] = 0;
    CList__Construct(&D_800D7BE0.lists[0], T2_OFFSETOF(CMemEntry, last), T2_OFFSETOF(CMemEntry, next));
    CList__Construct(&D_800D7BE0.lists[1], T2_OFFSETOF(CMemEntry, lockedLast), T2_OFFSETOF(CMemEntry, lockedNext));
    D_800D7BE0.cnt = 0;
    for (i = 0; i < gCacheEntryCount; i++) {
        n1 = D_800D7BE0.nodes;
        n1 = (B12Node *)((u8 *)n1 + i * sizeof(CMemEntry));
        ((CMemEntry *)n1)->flags = 0;
        D_800D7BE0.stk[i] = n1;
        D_800D7BE0.cnt++;
    }
    t = D_800D7BE0.tbl628;
    t[1] = 1;
    t[0] = 1;
    D_800D7BE0.f638 = -1;
    D_800D7BE0.tbl11C4[0] = 0;
    D_800D7BE0.tbl11C4[1] = 0;
    D_800D7BE0.cur = 0;
    D_800D7BE0.f120C = 0; /* 1 in Rage Wars */
    D_800D7BE0.stamp = 0;
    CList__Construct(&D_800D7BE0.lists3[0], T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    CList__Construct(&D_800D7BE0.lists3[1], T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    CList__Construct(&D_800D7BE0.lists3[2], T2_OFFSETOF(CCacheEntry, last), T2_OFFSETOF(CCacheEntry, next));
    for (i = 0; i < gCacheEntryCount; i++) {
        n = (B12Node *)D_800D7BE0.pool2;
        n = (B12Node *)((u8 *)n + i * sizeof(CCacheEntry));
        ((CCacheEntry *)n)->flags = 0;
        CList__AddTail(&D_800D7BE0.lists3[0], n);
    }
    CHashTable__Construct(gCacheEntryCount);
    osCreateMesgQueue(&D_800D7BE0.q870, D_800D7BE0.buf870, 0x40);
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
    memset(D_800D7BE0.stack, (u8)id, 0x800);
    osCreateThread(&D_800D7BE0.thread, id, CCache__Main, 0, D_800D7BE0.stack + 0x800, gThreadPriorityCache);
    osStartThread(&D_800D7BE0.thread);
}
#endif
