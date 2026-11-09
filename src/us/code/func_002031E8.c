/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00251448.c, func_802514A8_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "protos/list.h"

extern s32 CCache__FreeOldestMemEntry(s32 unused0, s32 unused1, s32 keepRecent);

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
