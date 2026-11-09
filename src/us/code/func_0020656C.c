/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254D44_de.
 * The same code in Turok 2. */

#include "common.h"
#include "globals/cache.h"
#include "protos/cache.h"

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
