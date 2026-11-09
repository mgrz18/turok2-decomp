/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254A28_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "protos/list.h"


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
