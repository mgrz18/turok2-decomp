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
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_3.c, func_802511AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void CCache__WaitForDisplayLists(void) {
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    u32 temp_a0;
    u32 temp_v0;
    var_s1 = 0;
    var_s0 = 0;
    do {
        if (gCache_dlistCount[var_s0] != 0) {
            if (gCache_currentDlist != var_s0) {
                var_s1 += 1;
                gCache_dlistBlocking[var_s0] = 1;
                goto block_7;
            }
            gCache_dlistValid[var_s0] = 0;
            CCache__ClearDisplayListLocks(0, var_s0);
            var_s0 += 1;
        } else {
block_7:
            var_s0 += 1;
        }
    } while (var_s0 < 2);
    if (var_s1 != 0) {
        temp_v0 = __osDisableInt();
        temp_v1 = gCache_mutex_count - 1;
        gCache_mutex_count = temp_v1;
        if (temp_v1 != 0) {
            __osRestoreInt(temp_v0);
            osSendMesg(&gCache_mutex.queue, 0, 1);
            var_s0_2 = 0;
        } else {
            __osRestoreInt(temp_v0);
            var_s0_2 = 0;
        }
        if (var_s1 > 0) {
            do {
                osRecvMesg(&gCache.blocker.queue, 0, 1);
                var_s0_2 += 1;
            } while (var_s0_2 < var_s1);
        }
        temp_a0 = __osDisableInt();
        temp_v1_2 = gCache_mutex_count + 1;
        gCache_mutex_count = temp_v1_2;
        if (temp_v1_2 != 1) {
            __osRestoreInt(temp_a0);
            osRecvMesg(&gCache_mutex.queue, 0, 1);
            return;
        }
        __osRestoreInt(temp_a0);
    }
}

static __inline__ CMemEntry *find_stale(CMemEntry **recent) {
    CMemEntry *node;
    for (node = gCache.memAgeList.m_pTail; node != 0; node = node->last) {
        if (!(node->flags & 0x702)) {
            if ((u32)(gCache_ageCount - node->age) >= 5) {
                return node;
            }
            *recent = node;
            break;
        }
    }
    for (; node != 0; node = node->last) {
        if (!(node->flags & 0x702) && (u32)(gCache_ageCount - node->age) >= 5) {
            return node;
        }
    }
    return 0;
}

s32 CCache__FreeOldestMemEntry(s32 unused0, s32 unused1, s32 keep_recent) {
    CMemEntry *recent;
    CMemEntry *node;
    s32 result;
    recent = 0;
    node = find_stale(&recent);
    if (node == 0) {
        if (keep_recent == 0) {
            node = recent;
        }
    }
    if (node != 0) {
        CCache__DetachCacheEntry(0, node);
        result = CHeap__Free(&gHeap, node->data);
        CList__Remove(&gCache_memAgeList, node);
        if (node->flags & 0x1000) {
            CList__Remove(&gCache_memLockedList, node);
        }
        if (gCache_currentCompressPos == node) {
            gCache_currentCompressPos = 0;
            node->flags = 0;
        } else {
            node->flags = 0;
        }
#ifdef TARGET_PORT
        gCache_memEntriesFree[gCache_freeEntryCount] = node; /* the free stack holds host pointers */
#else
        *(CMemEntry **)(gCache_freeEntryCount * 4 + (char *)gCache_memEntriesFree) = node;
#endif
        gCache_freeEntryCount++;
        return result;
    }
    return 0;
}

/* CCache__PopFreeMemEntry, inlined. */
static __inline__ CMemEntry *pool_alloc(s32 flags) {
    CMemEntry *entry;

    if (gCache.freeEntryCount == 0) {
        return NULL;
    }
    gCache.freeEntryCount -= 1;
    entry = gCache.memEntriesFree[gCache.freeEntryCount];
    entry->flags = MEMENTRY_FLAG_ALLOCATED;
    entry->lockCount = 0;
    entry->data = NULL;
    entry->lockedNext = NULL;
    entry->lockedLast = NULL;
    entry->cacheEntry = NULL;
    entry->age = gCache.ageCount;
    CList__AddHead(&gCache.memAgeList, entry);
    entry->flags |= (flags & 0xC);
    return entry;
}

CMemEntry *CCache__AllocMemEntry(s32 unused0, s32 flags) {
    CMemEntry *entry;

    entry = pool_alloc(flags);
    if (entry == NULL) {
        s32 keep = flags & 0x10;
        if (CCache__FreeOldestMemEntry(0, 2, keep == 0) != 0) {
            entry = pool_alloc(flags);
        }
    }
    return entry;
}

extern void memmove(void *dst, void *src, s32 size);

/* Moves unlocked blocks down the heap (free, then allocate again lower) and
 * files the moved ones back in the address-sorted locked list; one block per
 * call, or all of them. */
void CCache__Compact(s32 unused, s32 all) {
    CList moved;
    CMemEntry *entry;
    CMemEntry *next;
    CMemEntry *scan;
    u8 *newData;

    CList__Construct(&moved, T2_OFFSETOF(CMemEntry, lockedLast), T2_OFFSETOF(CMemEntry, lockedNext));
    if (all != 0 || gCache.currentCompressPos == NULL) {
        gCache.currentCompressPos = gCache.memLockedList.m_pHead;
        if (gCache.memLockedList.m_pHead == NULL) {
            goto drain;
        }
    }
    do {
        entry = gCache.currentCompressPos;
        gCache.currentCompressPos = entry->lockedNext;
        if (!(entry->flags & 0x701) && (uptr)gHeap.freeHead + sizeof(CHeapBlock) < (uptr)entry->data) {
            CHeap__Free(&gHeap, entry->data);
            newData = CHeap__Alloc(&gHeap, entry->size);
            if (newData < (u8 *)entry->data) {
                memmove(newData, entry->data, entry->size);
                entry->data = newData;
                CList__Remove(&gCache.memLockedList, entry);
                CList__AddTail(&moved, entry);
            }
            if (all == 0) {
                break;
            }
        }
    } while (gCache.currentCompressPos != NULL);
drain:
    entry = moved.m_pHead;
    while (entry != NULL) {
        next = entry->lockedNext;
        CList__RemoveHead(&moved);
        scan = gCache_memLockedList.m_pHead;
        while (scan != NULL) {
            if ((uptr)scan->data > (uptr)entry->data) {
                CList__InsertBefore(&gCache_memLockedList, scan, entry);
                break;
            }
            scan = scan->lockedNext;
        }
        if (scan == NULL) {
            CList__AddTail(&gCache_memLockedList, entry);
        }
        entry->flags |= MEMENTRY_FLAG_ON_LOCKED;
        entry = next;
    }
}

#ifdef TARGET_PORT /* the bucket stride is the host entry's size (pointer-wide value/next): with the
                    * N64 0x10 the lookup probed the wrong bucket, never found a resident entry and
                    * re-requested it every frame (asynchronous loads, i.e. gameplay, never drew) */
#define HASH_STRIDE sizeof(HashNode)
#else
#define HASH_STRIDE 0x10
#endif

void *CCache__LookupAndLock(s32 arg0, s32 arg1) {
    s32 temp_v1;
    s32 temp_v1_2;
    u32 temp_a0;
    u32 temp_v0;
    HashNode *node;
    void *value;
    void **out;
    CMemEntry *object;
    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    out = &value;
    node = (HashNode *)((u8 *)gHashTable +
        ((((arg1 << 5) ^ ((u32)arg1 >> 1) ^ ((u32)arg1 >> 9) ^
           ((u32)arg1 >> 17)) & gHashMask) * HASH_STRIDE));
    if (node->key != arg1) {
        goto not_initial;
    }
    value = node->value;
    goto found;
loop_found:
    *out = node->value;
    goto found;
not_initial:
    value = 0;
    if (node == 0) {
        goto found;
    }
loop:
    if (node->key == arg1) {
        goto loop_found;
    }
    node = node->next;
    if (node != 0) {
        goto loop;
    }
found:
    object = 0;
    if (value != 0) {
        object = *(void **)value;
        object->lockCount += 1;
        object->flags |= MEMENTRY_FLAG_LOCKED;
        object->age = gCache.ageCount;
        CList__MoveToHead(&gCache_memAgeList, object);
    }
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_v0);
    }
    return object;
}

extern void CDecompressor__DecompressSync(CDecompressor *, CCacheEntry *);
extern void CLoader__LoadSync(CLoader *, uptr, uptr, void *);
extern void CLoader__LoadAsync(CLoader *, u32, u32, void *, void *, CCacheEntry *, s32);
#ifdef TARGET_PORT
#include "t2asset_game.h"
#endif

static __inline__ void acquire(void) {
    u32 token = __osDisableInt();
    s32 count = gCache_mutex_count + 1;
    gCache_mutex_count = count;
    if (count != 1) {
        __osRestoreInt(token);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
}

static __inline__ void release(void) {
    u32 token = __osDisableInt();
    s32 count = gCache_mutex_count - 1;
    gCache_mutex_count = count;
    if (count != 0) {
        __osRestoreInt(token);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
}

static __inline__ void find(u32 key, CCacheEntry **out) {
    HashEntry *node = gHashTable + (((key << 5) ^ (key >> 1) ^ (key >> 9) ^ (key >> 17)) & gHashMask);
    if (node->key != key) goto not_initial;
    *out = node->value;
    goto done;
loop_found:
    *out = node->value;
    goto done;
not_initial:
    *out = 0;
    if (node == 0) goto done;
loop:
    if (node->key == key) goto loop_found;
    node = node->next;
    if (node != 0) goto loop;
done:;
}

static __inline__ void addref(CMemEntry *node) {
    node->lockCount++;
    node->flags |= 0x100;
}

static __inline__ void stamp(CMemEntry *node) {
    node->age = gCache_ageCount;
    CList__MoveToHead(&gCache_memAgeList, node);
}

static __inline__ CMemEntry *allocate(u32 size, u32 flags) {
    CMemEntry *node = CCache__AllocMemEntry(0, flags);
    if (node != 0) {
        addref(node);
        node->data = CCache__HeapAlloc(0, size, (flags >> 5) & 1, flags);
        if (node->data != 0) {
            node->size = size;
            node->flags |= flags;
            CCache__AddToAddressList(0, node);
        } else {
            if (--node->lockCount == 0) node->flags &= ~0x100;
            CCache__FreeMemEntry(0, node);
            node = 0;
        }
    }
    return node;
}

/* Returns the cached resource for `key` (loading `size` bytes from ROM offset
 * `key` when absent): synchronously when `direct` (or the manager mask) is set,
 * otherwise queues the read and returns 0. */
CMemEntry *CCache__Request(s32 unused, uptr key, u32 size, u32 mode, sptr owner, void *tag, char *name, u32 direct) {
    CCacheEntry *request;
    CCacheEntry *scan;
    CCacheEntry *resolved;
    CMemEntry *node;
    u32 flags;
    u32 oldFlags;
    s32 busy;
    CCacheEntry *allocated;

#ifdef TARGET_PORT
    /* Host sizes from here on (lookup, pending check, allocate, requestLength, the loader): the
     * archive serves RNC units unpacked, so a whole-unit request needs the unpacked size;
     * prefix probes keep theirs (docs/assets/PORT.md 10.4). */
    size = T2Port_AssetHostSize(key, size);
#endif
    acquire();
    find(key, &request);
    if (request != 0 && request->requestLength <= size) {
        addref(request->memEntry);
        stamp(request->memEntry);
        release();
        return request->memEntry;
    }
    direct |= gCache.frameDelay;
    busy = 0;
    if (direct != 0) {
        allocated = CCache__AllocCacheEntry(0, mode);
    } else {
        scan = gCache.cachePendingList.m_pHead;
        while (scan != 0) {
            if (scan->requestId == key && size <= scan->requestLength) {
                busy = 1;
                break;
            }
            scan = scan->next;
        }
        if (direct == 0 && busy) goto failed;
        allocated = CCache__AllocCacheEntry(0, mode);
    }
    request = allocated;
    if (request != 0) {
        if (tag != 0) flags = (mode & 0x1C) | 0x22;
        else flags = mode;
        request->referencedMemEntry = 0;
        node = allocate(size, flags);
        request->memEntry = node;
        if (request->memEntry != 0) {
            if (size & 1) size++;
            request->requestId = key;
            request->requestLength = size;
            request->notifyId = owner;
            request->decompressCallback = tag;
            request->memFlags = mode;
            CList__Remove(&gCache.cacheUsedList, request);
            CList__AddTail(&gCache.cachePendingList, request);
            if (direct != 0) {
                if (tag != 0) request->flags |= 4;
                release();
                CLoader__LoadSync(&gLoader, key, size, request->memEntry->data);
                if (tag != 0) CDecompressor__DecompressSync(&gDecompressor, request);
                acquire();
                if (tag != 0) request->flags &= ~4;
                resolved = CCache__CompleteRequest(0, request, 0);
                if (resolved != 0 && resolved != request) addref(resolved->memEntry);
                release();
                if (resolved != 0) return resolved->memEntry;
                return 0;
            } else {
                oldFlags = request->flags;
                request->flags = oldFlags | 2;
                if (tag != 0) request->flags = oldFlags | 0xE;
                release();
                CLoader__LoadAsync(&gLoader, key, size, request->memEntry->data, &gCache.queue, request, 0);
                return 0;
            }
        }
        CCache__FreeCacheEntry(0, request);
    }
failed:
    release();
    return 0;
}

#ifdef TARGET_PORT /* the bucket stride is the host entry's size (pointer-wide value/next): with the
                    * N64 0x10 the lookup probed the wrong bucket, never found a resident entry and
                    * re-requested it every frame (asynchronous loads, i.e. gameplay, never drew) */
#define HASH_STRIDE sizeof(HashEntry)
#else
#define HASH_STRIDE 0x10
#endif

static __inline__ void addref_203CA0(CMemEntry *node) {
    node->lockCount += 1;
    node->flags |= 0x100;
}

static __inline__ void unref(CMemEntry *node) {
    if (--node->lockCount == 0) {
        node->flags &= ~0x100;
    }
}

static __inline__ void acquire_203CA0(void) {
    u32 token = __osDisableInt();
    s32 counter = gCache_mutex_count + 1;
    gCache_mutex_count = counter;
    if (counter != 1) {
        __osRestoreInt(token);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
}

static __inline__ void release_203CA0(void) {
    u32 token = __osDisableInt();
    s32 counter = gCache_mutex_count - 1;
    gCache_mutex_count = counter;
    if (counter != 0) {
        __osRestoreInt(token);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
}

static __inline__ void lock(void) {
    u32 token = __osDisableInt();
    s32 counter = gCache_mutex_count + 1;
    gCache_mutex_count = counter;
    if (counter != 1) {
        __osRestoreInt(token);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
}

CMemEntry *CCache__RequestDecompression(s32 unused, s32 key, CMemEntry *first, CMemEntry *second, s32 mode, void *owner,
                    void *tag, void *spare, s32 async) {
    HashEntry *node;
    CCacheEntry *found;
    CCacheEntry **out;
    CCacheEntry *scan;
    s32 busy;
    acquire_203CA0();
    out = &found;
    node = (HashEntry *)((u8 *)gHashTable +
        ((((key << 5) ^ ((u32)key >> 1) ^ ((u32)key >> 9) ^
           ((u32)key >> 17)) & gHashMask) * HASH_STRIDE));
    if (node->key != key) {
        goto not_initial;
    }
    found = node->value;
    goto done;
loop_found:
    *out = node->value;
    goto done;
not_initial:
    found = 0;
    if (node == 0) {
        goto done;
    }
loop:
    if (node->key == key) {
        goto loop_found;
    }
    node = node->next;
    if (node != 0) {
        goto loop;
    }
done:
    busy = 0;
    if (found != 0) {
        addref_203CA0(found->memEntry);
        stamp(found->memEntry);
        release_203CA0();
        return found->memEntry;
    }
    async |= gCache.frameDelay;
    scan = gCache.cachePendingList.m_pHead;
    while (scan != 0) {
        if (scan->requestId == key) {
            busy = 1;
            break;
        }
        scan = scan->next;
    }
    if (busy == 0) {
        found = CCache__AllocCacheEntry(0, mode);
        if (found != 0) {
            found->requestId = key;
            found->requestLength = 0;
            found->notifyId = (sptr)owner;
            found->decompressCallback = tag;
            found->memFlags = mode;
            found->memEntry = first;
            found->referencedMemEntry = second;
            if (first != 0) {
                addref_203CA0(first);
            }
            if (second != 0) {
                addref_203CA0(second);
            }
            CList__Remove(&gCache.cacheUsedList, found);
            CList__AddTail(&gCache.cachePendingList, found);
            release_203CA0();
            if (async != 0) {
                found->flags &= ~0xE;
                CDecompressor__DecompressSync(&gDecompressor, found);
                acquire_203CA0();
                found = CCache__CompleteRequest(0, found, 0);
                release_203CA0();
                if (found != 0) {
                    return found->memEntry;
                }
                return 0;
            }
            if (CDecompressor__DecompressAsync(&gDecompressor, found, &gCache.queue) == 0) {
                acquire_203CA0();
                if (first != 0) {
                    unref(first);
                }
                if (second != 0) {
                    unref(second);
                }
                CList__Remove(&gCache.cachePendingList, found);
                CList__AddHead(&gCache.cacheUsedList, found);
                CCache__FreeCacheEntry(0, found);
                release_203CA0();
            }
            return 0;
        }
    }
    release_203CA0();
    return 0;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00252450.c, func_00252450.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

/* The port: typed on gCache (structs/cache.h) and CMemEntry/CCacheEntry. The ROM reaches the
 * cache through a B12Mgr view of gCache (Rage Wars offsets) and B12Node/B12Msg views at N64 offsets: on the host
 * they missed the cache's own fields (the display-list locks landed in CMemEntry.lockCount, so
 * a model still in the frame's display list could be freed; the cache thread waited on a queue
 * that was never created, so async loads and display-list releases never completed). */
void CCache__Main(void) {
    CCacheEntry *msg;
    CMemEntry *p;
    u32 mask;
    u32 m2;
    while (1) {
        osRecvMesg(&gCache.queue, (OSMesg *)&msg, 1);
        if ((uptr)msg < 2) {
            mask = __osDisableInt();
            if (++gCache_mutex_count != 1) {
                __osRestoreInt(mask);
                osRecvMesg(&gCache.mutex.queue, 0, 1);
            } else {
                __osRestoreInt(mask);
            }
            CCache__ReleaseDisplayList(0, (uptr)msg);
            m2 = __osDisableInt();
            if (--gCache_mutex_count != 0) {
                __osRestoreInt(m2);
                osSendMesg(&gCache.mutex.queue, 0, 1);
            } else {
                __osRestoreInt(m2);
            }
        } else {
            msg->flags &= ~2;
            if (msg->flags & 8) {
                msg->flags &= ~8;
                if (CDecompressor__DecompressAsync(&gDecompressor, msg, &gCache.queue) != 0) {
                    continue;
                }
                mask = __osDisableInt();
                if (++gCache_mutex_count != 1) {
                    __osRestoreInt(mask);
                    osRecvMesg(&gCache.mutex.queue, 0, 1);
                } else {
                    __osRestoreInt(mask);
                }
                CList__Remove(&gCache.cachePendingList, msg);
                CList__AddHead(&gCache.cacheUsedList, msg);
                p = msg->memEntry;
                p->flags &= ~2;
                while (p->lockCount != 0) {
                    p->lockCount--;
                    if (p->lockCount == 0) {
                        p->flags &= ~0x100;
                    }
                }
                if (!(p->flags & 0x702)) {
                    CCache__DetachCacheEntry(0, p);
                    CHeap__Free(&gHeap, p->data);
                    CCache__FreeMemEntry(0, p);
                }
                CCache__FreeCacheEntry(0, msg);
                m2 = __osDisableInt();
                if (--gCache_mutex_count != 0) {
                    __osRestoreInt(m2);
                    osSendMesg(&gCache.mutex.queue, 0, 1);
                } else {
                    __osRestoreInt(m2);
                }
            } else {
                msg->flags &= ~4;
                mask = __osDisableInt();
                if (++gCache_mutex_count != 1) {
                    __osRestoreInt(mask);
                    osRecvMesg(&gCache.mutex.queue, 0, 1);
                } else {
                    __osRestoreInt(mask);
                }
                CCache__CompleteRequest(0, msg, 1);
                m2 = __osDisableInt();
                if (--gCache_mutex_count != 0) {
                    __osRestoreInt(m2);
                    osSendMesg(&gCache.mutex.queue, 0, 1);
                } else {
                    __osRestoreInt(m2);
                }
            }
        }
    }
}

static __inline__ void find_2044A8(u32 key, CCacheEntry **out) {
    HashEntry *node = gHashTable + (((key << 5) ^ (key >> 1) ^ (key >> 9) ^ (key >> 17)) & gHashMask);

    if (node->key == key) {
        *out = node->value;
        return;
    }
    *out = 0;
    while (node != 0) {
        if (node->key == key) {
            *out = node->value;
            return;
        }
        node = node->next;
    }
}

static __inline__ void release_2044A8(CMemEntry *node) {
    CList__Remove(&gCache_memAgeList, node);
    if (node->flags & 0x1000) {
        CList__Remove(&gCache_memLockedList, node);
    }
#ifdef TARGET_PORT
    /* the same fields by name: the N64 word offsets from gCache_memAgeList are not the host's */
    if (gCache_currentCompressPos == node) {
        gCache_currentCompressPos = 0;
    }
    node->flags = 0;
    gCache_memEntriesFree[gCache_freeEntryCount] = node;
    gCache_freeEntryCount += 1;
#else
    if (((CMemEntry **)&gCache_memAgeList)[-2] == node) {    /* gCache_currentCompressPos */
        ((CMemEntry **)&gCache_memAgeList)[-2] = 0;
    }
    node->flags = 0;
    /* gCache_freeEntryCount, reached from gCache_memAgeList */
    gCache_memEntriesFree[((s32 *)&gCache_memAgeList)[0x2F3]] = node;
    ((s32 *)&gCache_memAgeList)[0x2F3] += 1;
#endif
}

CCacheEntry *CCache__CompleteRequest(s32 unused, CCacheEntry *request, s32 dropReference) {
    CCacheEntry *existing;
    CMemEntry *node;

    CList__Remove(&gCache_cachePendingList, request);
    CList__AddHead(&(&gCache_cachePendingList)[-1], request);   /* gCache_cacheUsedList, reached from the pending list */
    if (request->memEntry != 0) {
        find_2044A8(request->requestId, &existing);
        if (existing != 0) {
            if ((s32) existing->requestLength < (s32) request->requestLength) {
                CHashTable__Remove(existing->requestId);
                existing->memEntry->cacheEntry = 0;
                if (!(existing->memEntry->flags & 0x702)) {
                    CHeap__Free(&gHeap, existing->memEntry->data);
                    release_2044A8(existing->memEntry);
                }
                CCache__FreeCacheEntry(0, existing);
            } else {
                CHeap__Free(&gHeap, request->memEntry->data);
                release_2044A8(request->memEntry);
                CCache__FreeCacheEntry(0, request);
                return existing;
            }
        }
        CHashTable__Insert(request->requestId, request);
        request->memEntry->cacheEntry = request;
        if (dropReference != 0) {
            node = request->memEntry;
            if (--node->lockCount == 0) {
                node->flags &= ~0x100;
            }
        }
        return request;
    }
    CCache__FreeCacheEntry(0, request);
    return 0;
}

#include "gfx.h"
#include "gbi_t2.h"

/* A tracked allocation (list at gCache_memLockedList). */
typedef struct HeapNode HeapNode;
struct HeapNode {
    /* 0x00 */ u32 start;
    /* 0x04 */ u32 size;
    /* 0x08 */ s32 count;
    /* 0x0C */ u32 flags;
    /* 0x10 */ u32 stamp;
    /* 0x14 */ u8 pad14[0x10];
    /* 0x24 */ HeapNode *next;
};
/* A span of the heap (list at gHeap_usedHead). */
typedef struct HeapSpan HeapSpan;
struct HeapSpan {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ HeapSpan *next;
    /* 0x10 */ u32 size;
    /* 0x14 */ u32 free;
};
extern HeapSpan *gHeap_usedHead[];
extern void Render_SetCombineMode(s32);
extern void Render_SetRenderMode(s32);
/* gDPFillRectangle with ulx masked as a shift pair ((x << 22) >> 22): combine folds it back to the
 * `andi 0x3FF`, but at loop.c time it is two movables, which keeps left's (x & 0x3FF) << 14 inside the
 * pass loop as in the ROM (Rage Wars FINDINGS s703 no. 11). */
#define RECTA(pkt, ulx, uly, lrx, lry) \
    GFX_W(pkt, _SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL(lrx, 14, 10) | _SHIFTL(lry, 2, 10), \
          ((((unsigned int) (ulx) << 22) >> 22) << 14) | _SHIFTL(uly, 2, 10))
/* gDPSetPrimColor with the colour word given as one expression. */
#define PRIMC(pkt, wd) GFX_W(pkt, _SHIFTL(G_SETPRIMCOLOR, 24, 8), wd)
/* Half of a colour component, clamped to 0xFF. */
#define HALF(c) ((((s32) ((c) >> 1)) < 0x100) ? ((s32) ((c) >> 1)) : 0xFF)

/* Debug view of the heap: a grey box with one band per tracked allocation, scaled to the total heap
 * size (yellow = locked, red = bit 8, blue = bits 9/10, else green) with a half-bright edge line; nodes
 * not touched for 5 or more frames get a white overlay with a brighter edge. */
void CCache__DrawDebug(s32 unused, s32 left, s32 top, s32 right, s32 bottom) {
    u32 mask;
    u32 mask2;
    HeapSpan *span;
    HeapNode *node;
    uptr total;
    s32 height;
    s32 upper;
    s32 lower;
    s32 pass;
    u32 r;
    u32 g;
    u32 b;
    u32 base;
    u32 rg;
    s32 select;

    gDPPipeSync(gDList++);
    gDPSetCycleType1Cycle(gDList++);
    Render_SetCombineMode(0x19);
    Render_SetRenderMode(0x15);
    gDPSetPrimColor(gDList++, 0, 0, 0xA0, 0xA0, 0xA0, 0x00);
    gDPFillRectangle(gDList++, left - 1, top - 1, right + 1, bottom + 1);
    Render_SetRenderMode(0x1B);
    gDPSetPrimColor(gDList++, 0, 0, 0xA0, 0xA0, 0xA0, 0xFF);
    gDPFillRectangle(gDList++, left, top, right, bottom);

    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }

    base = 0x80000000;
    span = gHeap_usedHead[0];
    total = (uptr) span - base;
    for (; span != 0; span = span->next) {
        total += span->size + span->free;
    }

    gDPSetPrimColor(gDList++, 0, 0, 0x64, 0x64, 0x00, 0xFF);
    upper = (bottom - (u32) (((uptr) gHeap_usedHead[0] - base) * (bottom - top)) / total) - 1;
    height = bottom - top;
    RECTA(gDList++, left, upper, right, bottom + 1);

    for (pass = 0; pass < 2; pass++) {
        for (node = (HeapNode *)gCache_memLockedList.m_pHead; node != 0; node = node->next) {
            if (pass != 0) {
                select = (node->flags & 0x101) == 0x100;
            } else {
                select = (node->flags & 0x101) != 0x100;
            }
            if (select) {
                if (node->flags & 1) { r = 0xFF; g = 0xFF; b = 0; }
                else if (node->flags & 0x100) { r = 0xFF; g = 0; b = 0; }
                else if (node->flags & 0x600) { r = 0; g = 0; b = 0xFF; }
                else { r = 0; g = 0xFF; b = 0; }

                gDPSetPrimColor(gDList++, 0, 0, r, g, b, 0xFF);
                upper = (bottom - (u32) (((node->start + node->size) - base) * height) / total) - 1;
                lower = (bottom - (u32) (((node->start - base) - 0x20) * height) / total) - 1;
                if (upper < lower) {
                    RECTA(gDList++, left, upper, right, lower);
                }
                gDPSetPrimColor(gDList++, 0, 0, HALF(r), HALF(g), HALF(b), 0xFF);
                RECTA(gDList++, left, lower - 1, right, lower);

                if (!(node->flags & 0x702)) {
                    b = 0xFF;
                    if (gCache_ageCount - node->stamp >= 5) {
                        gDPSetPrimColor(gDList++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                        if (upper < lower) {
                            gDPFillRectangle(gDList++, left + 8, upper, right, lower);
                        } else {
                            upper = lower;
                        }
                        /* 1.5 times the highlight colour, clamped; rg in a variable keeps fold from merging
                         * its constant with the 0xFF (the ROM has lui 0xFFFF / or / ori 0xFF). */
                        rg = 0xFFFF0000;
                        PRIMC(gDList++, ({
                            s32 bright = ((s32) b * 3) >> 1;
                            bright < 0x100 ? (rg | ((bright & 0xFF) << 8)) | 0xFF : 0xFFFFFFFF;
                        }));
                        do { } while (0); /* sched1 barrier: keeps the colour store before the next lw */
                        gDPFillRectangle(gDList++, left + 8, lower - 1, right, lower);
                    }
                }
            }
        }
    }

    mask2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(mask2);
        osSendMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask2);
    }
}

s32 CMemEntry__IsLocked(CMemEntry *arg0) {
    return arg0->flags & 0x100;
}

void CMemEntry__Lock(CMemEntry *arg0) {
    arg0->lockCount = (s32) (arg0->lockCount + 1);
    arg0->flags = (s32) (arg0->flags | 0x100);
}

typedef struct {
    /* 0x0 */ u8 pad0[8];
    /* 0x8 */ s32 refCount;
    /* 0xC */ s32 flags;
} Arg0;

/* Drops a reference; clears flag 0x100 at zero. */
void CMemEntry__Unlock(Arg0 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->refCount - 1;
    arg0->refCount = temp_v0;
    if (temp_v0 == 0) {
        arg0->flags = (s32) (arg0->flags & ~0x100);
    }
}

/* Turok 3's CCache__Advance, once a frame: counts the frame delay down,
 * ages the cache and compacts it 10 steps under the cache lock. */
void CCache__Advance(void) {
    u32 mask;
    u32 mask2;
    s32 i;

    if (gCache.frameDelay != 0) {
        gCache.frameDelay--;
    }
    gCache.ageCount++;

    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, NULL, 1);
        i = 0;
    } else {
        __osRestoreInt(mask);
        i = 0;
    }
    do {
        CCache__Compact(0, 0);
        i++;
    } while (i < 10);
    mask2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(mask2);
        osSendMesg(&gCache.mutex.queue, NULL, 1);
        return;
    }
    __osRestoreInt(mask2);
}

B12Node *CCache__Allocate(s32 unused, s32 a1, u32 a2, sptr a3) {
    u32 mask;
    u32 m2;
    B12Node *r;
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    r = ((B12Node *(*)(s32, s32, u32, s32))CCache__AllocateInternal)(0, a1, a2, a3);   /* the ROM passes a fourth argument */
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
    return r;
}

/* De-faked in round 21 (j4): the N64 body's unlock sent the message in two identical arms under a
 * meaningless test; the plain if/else of the cache mutex (as the lock above it) matches. */
#ifdef TARGET_PORT
/* The port: the same through CMemEntry (structs/cache.h). B12Node is the N64 layout (key = data,
 * unk4 = size, flags at 0xC); on the host `data` is 8 bytes, so its flags slot is lockCount. The
 * address is sptr as protos/cache.h declares it (64BIT.md part 2). */
#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/cache.h"
#include "protos/ultra.h"
#include "protos/osint.h"

sptr CHeap__AllocAt(CHeap *, sptr, s32);
struct B12Node *CCache__AllocateAt(s32 arg0, sptr arg1, s32 arg2) {
    s32 mask;
    CMemEntry *entry;

    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    entry = CCache__AllocMemEntry(0, 0);
    if (entry != NULL) {
        entry->data = (void *)(uptr)CHeap__AllocAt(&gHeap, arg1, arg2);
        if (entry->data != NULL) {
            entry->size = arg2;
            entry->flags |= 3;
            CCache__AddToAddressList(0, entry);
        } else {
            CCache__FreeMemEntry(0, entry);
            entry = NULL;
        }
    }
    mask = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return (struct B12Node *)entry;
    }
    __osRestoreInt(mask);
    return (struct B12Node *)entry;
}
#else
#include "rw/junk_types.h" /* B12Node: a cached resource */
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/cache.h"
#include "protos/ultra.h"

sptr CHeap__AllocAt(CHeap *, sptr, s32);
B12Node *CCache__AllocateAt(s32 arg0, sptr arg1, s32 arg2)
{
  s32 temp_a0;
  sptr temp_v0;
  s32 temp_v0_2;
  s32 temp_v1;
  s32 temp_v1_2;
  B12Node *var_s0;
  temp_a0 = __osDisableInt();
  temp_v1 = gCache_mutex_count + 1;
  gCache_mutex_count = temp_v1;
  if (temp_v1 != 1)
  {
    __osRestoreInt(temp_a0);
    osRecvMesg((&gCache_mutex.queue), 0, 1);
  }
  else
  {
    __osRestoreInt(temp_a0);
  }
  var_s0 = CCache__AllocMemEntry(0, 0);
  if (var_s0 != 0)
  {
    temp_v0 = CHeap__AllocAt(&gHeap, arg1, arg2);
    var_s0->key = temp_v0;
    if (temp_v0 != 0)
    {
      var_s0->unk4 = arg2;
      var_s0->flags = var_s0->flags | 3;
      CCache__AddToAddressList(0, var_s0);
    }
    else
    {
      CCache__FreeMemEntry(0, var_s0);
      var_s0 = 0;
    }
  }
  temp_v0_2 = __osDisableInt();
  temp_v1_2 = gCache_mutex_count - 1;
  gCache_mutex_count = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    __osRestoreInt(temp_v0_2);
    osSendMesg((&gCache_mutex.queue), 0, 1);
  }
  else
  {
    __osRestoreInt(temp_v0_2);
  }
  return var_s0;
}
#endif

void CCache__TouchAndLock(s32 arg0, CMemEntry *arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    CCache__TouchMemEntry(0, arg1);
    arg1->lockCount = (s32) (arg1->lockCount + 1);
    arg1->flags = (s32) (arg1->flags | 0x100);
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}

/* Releases one use of a resource; the last one clears its 0x100 flag. */
void CCache__Unlock(s32 arg0, CMemEntry *arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_v0 = arg1->lockCount - 1;
    arg1->lockCount = temp_v0;
    if (temp_v0 == 0) {
        arg1->flags = (s32) (arg1->flags & ~0x100);
    }
    temp_v0_2 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_2);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_2);
}

void CCache__Deallocate(sptr arg0, void *arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    CCache__DeallocateInternal(0, arg1);
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}

void CCache__Flush(void) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    ((void (*)(s32))CCache__WaitForDisplayLists)(0);   /* the ROM passes an argument */
    do {

    } while (((s32 (*)(s32, s32, s32))CCache__FreeOldestMemEntryWithFlags)(0, 0, 0) != 0);   /* reads the v0 it leaves */
    CCache__Compact(0, 1);
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}

/* The port: typed on gCache (structs/cache.h) and CMemEntry/CCacheEntry. The ROM reaches the
 * cache through a B12Mgr view of gCache (Rage Wars offsets) and B12Node/B12Msg views at N64 offsets: on the host
 * they missed the cache's own fields (the display-list locks landed in CMemEntry.lockCount, so
 * a model still in the frame's display list could be freed; the cache thread waited on a queue
 * that was never created, so async loads and display-list releases never completed). */
void CCache__BeginDisplayList(s32 unused, s32 idx) {
    u32 mask;
    u32 m2;
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    gCache.currentDlist = idx;
    gCache.dlistValid[idx] = 1;
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
}

/* The port: typed on gCache (structs/cache.h) and CMemEntry/CCacheEntry. The ROM reaches the
 * cache through a B12Mgr view of gCache (Rage Wars offsets) and B12Node/B12Msg views at N64 offsets: on the host
 * they missed the cache's own fields (the display-list locks landed in CMemEntry.lockCount, so
 * a model still in the frame's display list could be freed; the cache thread waited on a queue
 * that was never created, so async loads and display-list releases never completed). */
s32 CCache__EndDisplayList(void) {
    u32 mask;
    u32 m2;
    s32 r;
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    r = gCache.dlistValid[gCache.currentDlist];
    gCache.currentDlist = -1;
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
    return r;
}

/* The port: typed on gCache (structs/cache.h) and CMemEntry/CCacheEntry. The ROM reaches the
 * cache through a B12Mgr view of gCache (Rage Wars offsets) and B12Node/B12Msg views at N64 offsets: on the host
 * they missed the cache's own fields (the display-list locks landed in CMemEntry.lockCount, so
 * a model still in the frame's display list could be freed; the cache thread waited on a queue
 * that was never created, so async loads and display-list releases never completed). */
void CCache__LockForDisplayList(s32 unused, B12Node *node) {
    CMemEntry *n = (CMemEntry *)node;
    u32 mask;
    u32 m2;
    s32 idx = gCache.currentDlist;
    u32 bit;
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    if (gCache.dlistValid[idx]) {
        if (idx) {
            bit = 0x400;
        } else {
            bit = 0x200;
        }
        if (!(n->flags & bit)) {
            n->flags |= bit;
            gCache.dlistCount[idx]++;
        }
    }
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&gCache.mutex.queue, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
}

void CCache__PostDisplayListDone(s32 unused, s32 x) {
    osSendMesg(&gCache_queue, x, 1);
}

void CCache__DeallocAndReplaceCompressedData(s32 arg0, CMemEntry **arg1, CMemEntry *arg2) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    CMemEntry *temp_s0;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_s0 = *arg1;
    temp_s0->flags = (temp_s0->flags & ~2);
    if (temp_s0->lockCount != 0) {
loop_4:
        do {
            temp_v0 = temp_s0->lockCount - 1;
            temp_s0->lockCount = temp_v0;
            if (temp_v0 != 0) {
                goto loop_4;
            }
            temp_s0->flags = (temp_s0->flags & ~0x100);
        } while (temp_s0->lockCount != 0);
    }
    if (!(temp_s0->flags & 0x702)) {
        CCache__DetachCacheEntry(0, temp_s0);
        CHeap__Free(&gHeap, temp_s0->data);
        CCache__FreeMemEntry(0, temp_s0);
    }
    *arg1 = arg2;
    temp_v0_2 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_2);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_2);
}

/* Two cache references. */
typedef struct {
    /* 0x00 */ CMemEntry *first;
    /* 0x04 */ CMemEntry *second;
} CacheRefs;

void CCache__ReleaseAndReplaceData(s32 arg0, void *refs, CMemEntry *arg2) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 temp_v1_2;
    CMemEntry *temp_a0_2;
    CMemEntry *temp_a0_3;

    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_a0_2 = ((CacheRefs *)refs)->first;
    if (temp_a0_2 != NULL) {
        temp_v0 = temp_a0_2->lockCount - 1;
        temp_a0_2->lockCount = temp_v0;
        if (temp_v0 == 0) {
            temp_a0_2->flags &= ~0x100;
        }
    }
    temp_a0_3 = ((CacheRefs *)refs)->second;
    if (temp_a0_3 != NULL) {
        temp_v0_2 = temp_a0_3->lockCount - 1;
        temp_a0_3->lockCount = temp_v0_2;
        if (temp_v0_2 == 0) {
            temp_a0_3->flags &= ~0x100;
        }
    }
    ((CacheRefs *)refs)->first = arg2;
    temp_v0_3 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_3);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_3);
}

void CCache__Touch(s32 arg0, s32 arg1) {
    s32 temp_v1;
    s32 temp_v1_2;
    u32 temp_a0;
    u32 temp_v0;
    HashNode *node;
    void *value;
    void **out;
    temp_a0 = __osDisableInt();
    temp_v1 = gCache_mutex_count + 1;
    gCache_mutex_count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    out = &value;
    node = (HashNode *)((u8 *)gHashTable +
        ((((arg1 << 5) ^ ((u32)arg1 >> 1) ^ ((u32)arg1 >> 9) ^
           ((u32)arg1 >> 17)) & gHashMask) * HASH_STRIDE));
    if (node->key != arg1) {
        goto not_initial;
    }
    value = node->value;
    goto found;
loop_found:
    *out = node->value;
    goto found;
not_initial:
    value = 0;
    if (node == 0) {
        goto found;
    }
loop:
    if (node->key == arg1) {
        goto loop_found;
    }
    node = node->next;
    if (node != 0) {
        goto loop;
    }
found:
    if (value != 0) {
        void *object = *(void **)value;
        ((CachedResource *)object)->stamp = gCache_ageCount;
        CList__MoveToHead(&gCache_memAgeList, object);
    }
    temp_v0 = __osDisableInt();
    temp_v1_2 = gCache_mutex_count - 1;
    gCache_mutex_count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&gCache_mutex.queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}

#include "globals/strings.h"

/* Loads a four-byte probe to read a count, frees it under the resource lock, then loads count*4+15
 * rounded to eight bytes and stores its first word through arg1, returning the allocation. */
void **CCache__LoadPersistantIndex(s32 arg0, void **arg1, sptr arg2, void *arg3) {
    void **resource;
    s32 size;
    s32 counter;
    s32 counter2;
    u32 token;
    u32 token2;
    s32 one = 1;

    resource = CCache__Request(0, arg2, 4, 0x1B, 0, 0, sCCacheDesc_Header, one);
    size = **(s32 **)resource;
    token = __osDisableInt();
    counter = gCache_mutex_count + one;
    gCache_mutex_count = counter;
    if (counter != one) {
        __osRestoreInt(token);
        osRecvMesg(&gCache_mutex.queue, 0, one);
    } else {
        __osRestoreInt(token);
    }
    CCache__DeallocateInternal(0, resource);
    token2 = __osDisableInt();
    counter2 = gCache_mutex_count - 1;
    gCache_mutex_count = counter2;
    if (counter2 != 0) {
        __osRestoreInt(token2);
        osSendMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token2);
    }
    resource = CCache__Request(0, arg2, ((size * 4) + 0xF) & ~7, 0x1B, 0, 0, arg3, 1);
    *arg1 = *resource;
    return resource;
}

#include "m2c_macros.h"
#include "protos/iset.h"
#ifdef TARGET_PORT
/* The N64 function is void, but returns CCache__Request's result in v0 and every caller
 * reads it (the handle of the sub-block). arg5 is the decompression callback. */
void *CCache__LoadPersistantSubBlock(s32 arg0, sptr arg1, s32 arg2, s32 arg3, sptr arg4, sptr arg5, sptr arg6) {
    s32 sp20;

    return CCache__Request(0, CISet__GetROMAddressAndSize(arg1, arg3, arg2, &sp20), sp20, 0x1B, arg4, (void *)arg5, (char *)arg6, 1);
}
#else
void CCache__LoadPersistantSubBlock(s32 arg0, sptr arg1, s32 arg2, s32 arg3, sptr arg4, sptr arg5, sptr arg6) {
    s32 sp20;

    CCache__Request(0, CISet__GetROMAddressAndSize(arg1, arg3, arg2, &sp20), sp20, 0x1B, arg4, arg5, arg6, 1);
}
#endif
#ifndef TARGET_PORT
sptr CCache__AllocateMemoryForDecompression(s32 arg0, void *arg1, s32 arg2)
{
  s32 temp_a0;
  s32 temp_s1;
  sptr temp_v0;
  s32 temp_v0_2;
  s32 temp_v0_3;
  s32 temp_v1;
  s32 temp_v1_2;
  sptr var_s0;
  temp_a0 = __osDisableInt();
  temp_v1 = gCache_mutex_count + 1;
  gCache_mutex_count = temp_v1;
  if (temp_v1 != 1)
  {
    __osRestoreInt(temp_a0);
    osRecvMesg((&gCache_mutex.queue), 0, 1);
  }
  else
  {
    __osRestoreInt(temp_a0);
  }
  temp_s1 = *((s32 *) (((s8 *) arg1) + 0x1C));
  var_s0 = CCache__AllocMemEntry(0, temp_s1);
  if (var_s0 != 0)
  {
    *((s32 *) (((s8 *) var_s0) + 8)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 8))) + 1);
    *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x100);
    temp_v0 = CCache__HeapAlloc(0, arg2, (((u32) temp_s1) >> 5) & 1, temp_s1);
    *((s32 *) (((s8 *) var_s0) + 0)) = temp_v0;
    if (temp_v0 != 0)
    {
      *((s32 *) (((s8 *) var_s0) + 4)) = arg2;
      *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | temp_s1);
      CCache__AddToAddressList(0, var_s0);
    }
    else
    {
      temp_v0_2 = (*((s32 *) (((s8 *) var_s0) + 8))) - 1;
      *((s32 *) (((s8 *) var_s0) + 8)) = temp_v0_2;
      if (temp_v0_2 == 0)
      {
        *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) & (~0x100));
      }
      CCache__FreeMemEntry(0, var_s0);
      var_s0 = 0;
    }
  }
  temp_v0_3 = __osDisableInt();
  temp_v1_2 = gCache_mutex_count - 1;
  gCache_mutex_count = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    __osRestoreInt(temp_v0_3);
    osSendMesg((&gCache_mutex.queue), 0, 1);
  }
  else
  {
    __osRestoreInt(temp_v0_3);
  }
  return var_s0;
}
#endif
#ifdef TARGET_PORT
#include "structs/cache.h"

/* The host form of the function above, typed (its m2c body writes CMemEntry fields at N64
 * offsets and returns the entry as s32). ProPackDecompress (func_00200B00.c) calls it for the
 * block a load unit is copied into: a fresh entry with the request's memFlags, locked, `size`
 * bytes from the heap (from the top when memFlags has 0x20), on the address list. */
CMemEntry *CCache__AllocDecompressFlags(u32 flags, u32 size) {
    CMemEntry *node;
    s32 token = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(token);
        osRecvMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
    node = CCache__AllocMemEntry(0, flags);
    if (node != NULL) {
        node->lockCount++;
        node->flags |= 0x100;
        node->data = (void *)CCache__HeapAlloc(0, size, (flags >> 5) & 1, flags);
        if (node->data != NULL) {
            /* round 19 h1: the block starts zeroed. The game reads some of it before writing it
             * (CGameObjectInstance__Construct's ground tilt tests m_AI.m_dwFlags & 0xC0000 before
             * CAI__Setup; FIDELITY.md 9.2): on the host the old heap bytes there hold host
             * pointers, so the tilt (and the bone matrices built from it) changed with ASLR
             * (the level-135 two paths, STABILITY.md 13). The console's heap starts zeroed
             * too (InitializeSystemResources). */
            __builtin_memset(node->data, 0, size);
            node->size = size;
            node->flags |= flags;
            CCache__AddToAddressList(0, node);
        } else {
            if (--node->lockCount == 0) {
                node->flags &= ~0x100;
            }
            CCache__FreeMemEntry(0, node);
            node = NULL;
        }
    }
    token = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(token);
        osSendMesg(&gCache_mutex.queue, 0, 1);
    } else {
        __osRestoreInt(token);
    }
    return node;
}

CMemEntry *CCache__AllocDecompressBlock(CCacheEntry *request, u32 size) {
    return CCache__AllocDecompressFlags(request->memFlags, size);
}

/* The game's entry point (protos/cache.h still returns the entry as s32: 64BIT.md part 2). */
sptr CCache__AllocateMemoryForDecompression(s32 arg0, void *arg1, s32 arg2) {
    return (sptr)CCache__AllocDecompressBlock((CCacheEntry *)arg1, (u32)arg2);
}
#endif
#ifdef TARGET_PORT
/* The port: the typed allocation of func_00205D7C.c with memFlags 0x22 (the m2c body below writes
 * CMemEntry fields at N64 offsets). */
struct CMemEntry *CCache__AllocDecompressFlags(u32 flags, u32 size);
sptr CCache__AllocateMemoryForDecompressionTop(s32 arg0, sptr arg1, s32 arg2) {
    return (sptr)CCache__AllocDecompressFlags(0x22, (u32)arg2);
}
#else
sptr CCache__AllocateMemoryForDecompressionTop(s32 arg0, sptr arg1, s32 arg2)
{
  s32 temp_a0;
  sptr temp_v0;
  s32 temp_v0_2;
  s32 temp_v0_3;
  s32 temp_v1;
  s32 temp_v1_2;
  sptr var_s0;
  temp_a0 = __osDisableInt();
  temp_v1 = gCache_mutex_count + 1;
  gCache_mutex_count = temp_v1;
  if (temp_v1 != 1)
  {
    __osRestoreInt(temp_a0);
    osRecvMesg((&gCache_mutex.queue), 0, 1);
  }
  else
  {
    __osRestoreInt(temp_a0);
  }
  var_s0 = CCache__AllocMemEntry(0, 0x22);
  if (var_s0 != 0)
  {
    *((s32 *) (((s8 *) var_s0) + 8)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 8))) + 1);
    *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x100);
    temp_v0 = CCache__HeapAlloc(0, arg2, 1, 0x22);
    *((s32 *) (((s8 *) var_s0) + 0)) = temp_v0;
    if (temp_v0 != 0)
    {
      *((s32 *) (((s8 *) var_s0) + 4)) = arg2;
      *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) | 0x22);
      CCache__AddToAddressList(0, var_s0);
    }
    else
    {
      temp_v0_2 = (*((s32 *) (((s8 *) var_s0) + 8))) - 1;
      *((s32 *) (((s8 *) var_s0) + 8)) = temp_v0_2;
      if (temp_v0_2 == 0)
      {
        *((s32 *) (((s8 *) var_s0) + 0xC)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xC))) & (~0x100));
      }
      CCache__FreeMemEntry(0, var_s0);
      var_s0 = 0;
    }
  }
  temp_v0_3 = __osDisableInt();
  temp_v1_2 = gCache_mutex_count - 1;
  gCache_mutex_count = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    __osRestoreInt(temp_v0_3);
    osSendMesg((&gCache_mutex.queue), 0, 1);
  }
  else
  {
    __osRestoreInt(temp_v0_3);
  }
  return var_s0;
}
#endif

void CCache__SetFrameDelay(s32 arg0) {
    gCache_frameDelay = arg0;
}

s32 CCache__GetFrameDelay(void) {
    return gCache_frameDelay;
}

s32 CCache__GetFreeCacheEntryCount(void) {
    return gCache.cacheFreeList.m_nCount;
}

s32 CCache__GetFreeMemEntryCount(void) {
    return gCache_freeEntryCount;
}

s32 CCache__GetMemEntryCount(void) {
    return gCache.memAgeList.m_nCount;
}

s32 func_002060B8(void) {
    return gCache.memAgeList.m_nCount;
}

s32 func_002060C8(void) {
    return gCache.memAgeList.m_nCount;
}

s32 func_002060D8(void) {
    return 0;
}

CMemEntry *CCache__AllocateInternal(s32 arg0, s32 arg1, s32 arg2) {
    sptr temp_v0;
    s32 temp_v0_2;
    CMemEntry *var_s0;

    var_s0 = CCache__AllocMemEntry(0, arg2);
    if (var_s0 != 0) {
        var_s0->lockCount += 1;
        var_s0->flags |= 0x100;
        temp_v0 = CCache__HeapAlloc(0, arg1, ((u32) arg2 >> 5) & 1, arg2);
        var_s0->data = (void *)temp_v0;
        if (temp_v0 != 0) {
            var_s0->size = arg1;
            var_s0->flags |= arg2;
            CCache__AddToAddressList(0, var_s0);
            return var_s0;
        }
        temp_v0_2 = var_s0->lockCount - 1;
        var_s0->lockCount = temp_v0_2;
        if (temp_v0_2 == 0) {
            var_s0->flags &= ~0x100;
        }
        CCache__FreeMemEntry(0, var_s0);
        var_s0 = NULL;
        goto block_6;
    }
block_6:
    return var_s0;
}

/* Drops every lock of the entry (one at a time, as the ROM's loop does) and frees it when nothing
 * else holds it (flags 0x702). The host build used to keep a second, natural form of this; the N64
 * code was a Rage Wars CAI view of the CMemEntry until round 28. */
void CCache__DeallocateInternal(s32 arg0, void *entry) {
    CMemEntry *e = entry;
    s32 n;

    e->flags &= ~2;
    if (e->lockCount != 0) {
loop_1:
        do {
            n = e->lockCount - 1;
            e->lockCount = n;
            if (n != 0) {
                goto loop_1;
            }
            e->flags &= ~MEMENTRY_FLAG_LOCKED;
        } while (e->lockCount != 0);
    }
    if (!(e->flags & 0x702)) {
        CCache__DetachCacheEntry(0, e);
        CHeap__Free(&gHeap, e->data);
        CCache__FreeMemEntry(0, e);
    }
}

/* Takes a CMemEntry off the free stack and puts it at the head of the age list. */
CMemEntry *CCache__PopFreeMemEntry(s32 unused0, s32 flags) {
    CMemEntry *entry;

    if (gCache.freeEntryCount == 0) {
        return NULL;
    }
    gCache.freeEntryCount -= 1;
    entry = gCache.memEntriesFree[gCache.freeEntryCount];
    entry->flags = MEMENTRY_FLAG_ALLOCATED;
    entry->lockCount = 0;
    entry->data = NULL;
    entry->lockedNext = NULL;
    entry->lockedLast = NULL;
    entry->cacheEntry = NULL;
    entry->age = gCache.ageCount;
    CList__AddHead(&gCache.memAgeList, entry);
    entry->flags |= (flags & 0xC);
    return entry;
}

#include "structs/camera.h"

/* Takes a CMemEntry off the age and address lists and returns it to the free stack. */
void CCache__FreeMemEntry(s32 unused, CMemEntry *entry) {
    CList__Remove(&gCache.memAgeList, entry);
    if (entry->flags & MEMENTRY_FLAG_ON_LOCKED) {
        CList__Remove(&gCache.memLockedList, entry);
    }
    if (gCache.currentCompressPos == entry) {
        gCache.currentCompressPos = NULL;
    }
    entry->flags = 0;
    gCache.memEntriesFree[gCache.freeEntryCount] = entry;
    gCache.freeEntryCount++;
}

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 CCache__FreeOldestMemEntryWithFlags(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = arg1;
    if (arg2 & 0x10) {
        var_v0 = 0;
    }
    return CCache__FreeOldestMemEntry(0, 2, var_v0);
}
#else
void CCache__FreeOldestMemEntryWithFlags(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = arg1;
    if (arg2 & 0x10) {
        var_v0 = 0;
    }
    CCache__FreeOldestMemEntry(0, 2, var_v0);
}
#endif

extern void *CHeap__AllocTop(CHeap *arg0, s32 arg1);

void *CCache__HeapAlloc(s32 arg0, u32 arg1, s32 arg2, u32 arg3)
{
  void *result;
  s32 value;
  if (arg2 != 0)
  {
    do { result = CHeap__AllocTop(&gHeap, arg1); } while (0);
  }
  else
  {
    result = CHeap__Alloc(&gHeap, arg1);
  }
  if (result == 0)
  {
    arg3 &= 0x10;
    do
    {
      value = CCache__FreeOldestMemEntry(0, 2, arg3 == 0);
    }
    while ((value != 0) && (value < arg1));
    if (value >= arg1)
    {
      if (arg2 != 0)
      {
        result = CHeap__AllocTop(&gHeap, arg1);
      }
      else
      {
        result = CHeap__Alloc(&gHeap, arg1);
      }
    }
  }
  return result;
}

/* Files a CMemEntry in the locked list, which is sorted by address. */
void CCache__AddToAddressList(s32 unused, CMemEntry *entry) {
    CMemEntry *scan;

    scan = gCache_memLockedList.m_pHead;
    if (scan != NULL) {
loop_1:
        if (scan->data > entry->data) {
            CList__InsertBefore(&gCache_memLockedList, scan, entry);
        } else {
            scan = scan->lockedNext;
            if (scan != NULL) {
                goto loop_1;
            }
        }
        if (scan == NULL) {
            goto block_5;
        }
    } else {
block_5:
        CList__AddTail(&gCache_memLockedList, entry);
    }
    entry->flags = entry->flags | MEMENTRY_FLAG_ON_LOCKED;
}

void CCache__ClearCurrentDisplayList(void) {
    s32 *p = &gCache_currentDlist;
    CCache__ClearDisplayListLocks(0, *p);
    gCache_dlistValid[*p] = 0;
}

/* Clears display list dlist's lock bit on every CMemEntry. */
void CCache__ClearDisplayListLocks(s32 unused, s32 dlist) {
    s32 mask;
    u32 i;
    u32 count;

    mask = ~MEMENTRY_FLAG_DLISTA_LOCKED;
    if (dlist != 0) {
        mask = ~MEMENTRY_FLAG_DLISTB_LOCKED;
    }
    do { i = 0; count = gCacheEntryCount; } while (0);   /* changes only the asm label numbers */
    if (count != 0) {
        do {
            gCache.memEntries[i].flags &= mask;
            i++;
        } while (i < count);
    }
    gCache_dlistCount[dlist] = 0;
}

/* Stamps a CMemEntry with this frame and moves it to the head of the age list. */
void CCache__TouchMemEntry(s32 unused, CMemEntry *entry) {
    entry->age = gCache.ageCount;
    CList__MoveToHead(&gCache.memAgeList, entry);
}

/* Unhooks a memory entry's request (if any) from the hash table and returns it to the free list. */
void CCache__DetachCacheEntry(s32 arg0, void *arg1) {
    CMemEntry *mem = arg1;
    CCacheEntry *found;
    CCacheEntry *entry;

    found = mem->cacheEntry;
    if (found != NULL) {
        CHashTable__Remove(found->requestId);
        entry = mem->cacheEntry;
        CList__Remove(&gCache_cacheUsedList, entry);
        entry->flags = 0;
        CList__AddHead(&gCache_cacheFreeList, entry);
    }
}

/* Takes the head of the free request list (flags 1) onto the used list, the list after it. */
void *CCache__TakeFreeCacheEntry(void) {
    CList *p = &gCache_cacheFreeList;
    CCacheEntry *e = p->m_pHead;

    if (e != 0) {
        CList__Remove(p, e);
        e->flags = 1;
        CList__AddHead(p + 1, e);
    }
    return e;
}

/* Returns a request entry to the free list. */
#ifdef TARGET_PORT
void CCache__FreeCacheEntry(s32 arg0, void *entry) {
    CCacheEntry *e = entry;

    CList__Remove(&gCache_cacheUsedList, e);
    e->flags = 0;
    CList__AddHead(&gCache_cacheFreeList, e);
}
#else
/* gCache_cacheFreeList is reached from gCache_cacheUsedList (the list before it): one %hi for both. */
void CCache__FreeCacheEntry(s32 arg0, void *entry) {
    CCacheEntry *e = entry;

    CList__Remove(&gCache_cacheUsedList, e);
    e->flags = 0;
    CList__AddHead((CList *)((s8 *)&gCache_cacheUsedList - sizeof(CList)), e);
}
#endif

static __inline__ CCacheEntry *take(s32 active) {
 CCacheEntry *node=gCache_cacheFreeList.m_pHead;
 if(node) {CList__Remove(&gCache_cacheFreeList,node); node->flags=active;CList__AddHead(&gCache_cacheUsedList,node);}
 return node;
}

void *CCache__AllocCacheEntry(s32 unused,s32 flags) {
 CCacheEntry *node=take(1);
 if (!node) {
  s32 active=1;
  flags &= 0x10;
  while(CCache__FreeOldestMemEntry(0,2,flags==0)) {node=take(active);if(node)break;}
 }
 return node;
}

/* Display list dlist is done: wakes CCache__WaitForDisplayLists if it waits
 * for it and clears the list's lock bit on every CMemEntry. */
void CCache__ReleaseDisplayList(s32 unused, sptr dlist) {
    s32 mask;
    u32 i;
    u32 count;

    if (gCache_dlistBlocking[dlist] != 0) {
        gCache_dlistBlocking[dlist] = 0;
        osSendMesg(&gCache.blocker.queue, NULL, 1);
    }
    mask = ~MEMENTRY_FLAG_DLISTA_LOCKED;
    if (dlist != 0) {
        mask = ~MEMENTRY_FLAG_DLISTB_LOCKED;
    }
    do { i = 0; count = gCacheEntryCount; } while (0);   /* changes only the asm label numbers */
    if (count != 0) {
        do {
            gCache.memEntries[i].flags &= mask;
            i++;
        } while (i < count);
    }
    do { gCache_dlistCount[dlist] = 0; } while (0);     /* the same */
}

EntryRecAt18V3 *CCache__FindOldestMemEntry(s32 unused, EntryList *list, EntryRecAt18V3 **recent)
{
    EntryRecAt18V3 *e = list->first;
    if (*recent == 0) {
        for (; e != 0; e = e->next) {
            if (!(e->flags & 0x702)) {
                if ((u32)(gCache.ageCount - e->stamp) >= 5) {
                    return e;
                }
                *recent = e;
                break;
            }
        }
    }
    for (; e != 0; e = e->next) {
        if (!(e->flags & 0x702) && (u32)(gCache.ageCount - e->stamp) >= 5) {
            return e;
        }
    }
    return 0;
}
