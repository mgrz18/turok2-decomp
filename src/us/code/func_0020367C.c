/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_4.CPP, func_8025193C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/list.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/list.h"
#include "protos/cache.h"
#include "protos/ultra.h"

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern void CDecompressor__DecompressSync(CDecompressor *, CCacheEntry *);
extern void CLoader__LoadSync(CLoader *, uptr, u32, void *);
extern void CLoader__LoadAsync(CLoader *, u32, u32, void *, void *, CCacheEntry *, s32);

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
