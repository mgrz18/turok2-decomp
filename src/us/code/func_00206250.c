/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254A28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *CCache__PopFreeMemEntry(s32 unused0, s32 arg1);
extern s32 CList__AddHead(void *arg0, s32 arg1);
void *CCache__PopFreeMemEntry(s32 unused0, s32 arg1) {
    extern Pool gCache_freeEntryCount;
    extern void **gCache_memEntriesFree;
    struct ResourceCacheEntry *temp_s0;
    if (gCache_freeEntryCount.count == 0) {
        return 0;
    }
    gCache_freeEntryCount.count -= 1;
    temp_s0 = gCache_memEntriesFree[gCache_freeEntryCount.count];
    temp_s0->flags = 0x800;
    temp_s0->references = 0;
    temp_s0->resource = 0;
    temp_s0->callback = 0;
    temp_s0->pending = 0;
    temp_s0->owner = 0;
    temp_s0->lastUsed = gCache_freeEntryCount.field44;
    CList__AddHead((char *)&gCache_freeEntryCount - 0xBCC, (s32)temp_s0);
    temp_s0->flags |= (arg1 & 0xC);
    return temp_s0;
}
