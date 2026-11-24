#include "common.h"
#include "globals/cache.h"
#include "protos/list.h"
#include "protos/cache.h"

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
