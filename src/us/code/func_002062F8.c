#include "common.h"
#include "structs/camera.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "protos/list.h"
#include "protos/cache.h"


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
