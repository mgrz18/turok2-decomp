#include "common.h"
#include "globals/cache.h"
#include "protos/list.h"
#include "protos/cache.h"
#include "protos/hash.h"

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
