/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/802508E0_5.CPP, func_80251F6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/list.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/list.h"

extern void __osRestoreInt(OSIntMask mask);
extern OSIntMask __osDisableInt(void);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
extern void CCache__FreeCacheEntry(s32 arg0, void *arg1);
extern CCacheEntry *CCache__AllocCacheEntry(s32, s32);
extern void CDecompressor__DecompressSync(CDecompressor *, CCacheEntry *);
extern CCacheEntry *CCache__CompleteRequest(s32 unused, CCacheEntry *request, s32 dropReference);
static __inline__ void addref(CMemEntry *node) {
    node->lockCount += 1;
    node->flags |= 0x100;
}
static __inline__ void stamp(CMemEntry *node) {
    node->age = gCache_ageCount;
    CList__MoveToHead(&gCache_memAgeList, node);
}
static __inline__ void unref(CMemEntry *node) {
    if (--node->lockCount == 0) {
        node->flags &= ~0x100;
    }
}
static __inline__ void acquire(void) {
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
static __inline__ void release(void) {
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
extern s32 CDecompressor__DecompressAsync(CDecompressor *, CCacheEntry *, OSMesgQueue *);
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
    acquire();
    out = &found;
    node = (HashEntry *)((u8 *)gHashTable +
        ((((key << 5) ^ ((u32)key >> 1) ^ ((u32)key >> 9) ^
           ((u32)key >> 17)) & gHashMask) * 0x10));
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
        addref(found->memEntry);
        stamp(found->memEntry);
        release();
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
            found->notifyId = (s32)owner;
            found->decompressCallback = tag;
            found->memFlags = mode;
            found->memEntry = first;
            found->referencedMemEntry = second;
            if (first != 0) {
                addref(first);
            }
            if (second != 0) {
                addref(second);
            }
            CList__Remove(&gCache.cacheUsedList, found);
            CList__AddTail(&gCache.cachePendingList, found);
            release();
            if (async != 0) {
                found->flags &= ~0xE;
                CDecompressor__DecompressSync(&gDecompressor, found);
                acquire();
                found = CCache__CompleteRequest(0, found, 0);
                release();
                if (found != 0) {
                    return found->memEntry;
                }
                return 0;
            }
            if (CDecompressor__DecompressAsync(&gDecompressor, found, &gCache.queue) == 0) {
                acquire();
                if (first != 0) {
                    unref(first);
                }
                if (second != 0) {
                    unref(second);
                }
                CList__Remove(&gCache.cachePendingList, found);
                CList__AddHead(&gCache.cacheUsedList, found);
                CCache__FreeCacheEntry(0, found);
                release();
            }
            return 0;
        }
    }
    release();
    return 0;
}
