/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_6.CPP, func_80252774_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/cache.h"

typedef struct HashEntry {
    /* 0x00 */ u32 key;
    /* 0x04 */ CCacheEntry *value;
    /* 0x08 */ s32 index;
    /* 0x0C */ struct HashEntry *next;
} HashEntry;

extern char gCache_cachePendingList[];           /* pending request list */
extern u32 gHashMask;              /* hash mask */
extern HashEntry *gHashTable;       /* hash table */
extern char gHeap[];           /* heap */
extern s32 gCache_memAgeList;              /* cached node list; the current node sits 8 bytes before it, the free node count 0xBCC after */
extern char gCache_memLockedList[];           /* locked node list */
extern CMemEntry **gCache_memEntriesFree; /* free node stack */

void CList__Remove(void *list, void *item);
void CList__AddHead(void *list, void *item);
void CHashTable__Remove(u32 key);
void CHeap__Free(void *heap, s32 data);
void CCache__FreeCacheEntry(s32 unused, CCacheEntry *request);
void CHashTable__Insert(u32 key, CCacheEntry *value);

static __inline__ void find(u32 key, CCacheEntry **out) {
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

static __inline__ void release(CMemEntry *node) {
    CList__Remove(&gCache_memAgeList, node);
    if (node->flags & 0x1000) {
        CList__Remove(gCache_memLockedList, node);
    }
    if (*(&gCache_memAgeList - 2) == (s32) node) {
        *(&gCache_memAgeList - 2) = 0;
    }
    node->flags = 0;
    gCache_memEntriesFree[*(&gCache_memAgeList + 0x2F3)] = node;
    *(&gCache_memAgeList + 0x2F3) += 1;
}

CCacheEntry *CCache__CompleteRequest(s32 unused, CCacheEntry *request, s32 dropReference) {
    CCacheEntry *existing;
    CMemEntry *node;

    CList__Remove(gCache_cachePendingList, request);
    CList__AddHead(gCache_cachePendingList - 0x14, request);
    if (request->memEntry != 0) {
        find(request->requestId, &existing);
        if (existing != 0) {
            if ((s32) existing->requestLength < (s32) request->requestLength) {
                CHashTable__Remove(existing->requestId);
                existing->memEntry->cacheEntry = 0;
                if (!(existing->memEntry->flags & 0x702)) {
                    CHeap__Free(gHeap, existing->memEntry->data);
                    release(existing->memEntry);
                }
                CCache__FreeCacheEntry(0, existing);
            } else {
                CHeap__Free(gHeap, request->memEntry->data);
                release(request->memEntry);
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
