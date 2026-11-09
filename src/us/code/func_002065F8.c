#include "common.h"
#include "m2c_macros.h"
#include "globals/cache.h"
#include "protos/list.h"
#include "protos/cache.h"

s32 CHashTable__Remove(s32);


#ifdef TARGET_PORT
/* The port: the same through CMemEntry / CCacheEntry (structs/cache.h), whose host offsets are
 * not the N64 0x14 / 8 / 0x10. */
#include "structs/cache.h"

void CCache__DetachCacheEntry(s32 arg0, void *arg1) {
    CMemEntry *mem = arg1;
    CCacheEntry *entry = mem->cacheEntry;

    if (entry != NULL) {
        CHashTable__Remove(entry->requestId);
        entry = mem->cacheEntry;
        CList__Remove(&gCache_cacheUsedList, entry);
        entry->flags = 0;
        CList__AddHead(&gCache_cacheFreeList, entry);
    }
}
#else
void CCache__DetachCacheEntry(s32 arg0, void *arg1) {
    void *temp_s0;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg1, void **, 0x14);
    if (temp_v0 != NULL) {
        CHashTable__Remove(M2C_FIELD(temp_v0, s32 *, 8));
        temp_s0 = M2C_FIELD(arg1, void **, 0x14);
        CList__Remove(&gCache_cacheUsedList, temp_s0);
        M2C_FIELD(temp_s0, s32 *, 0x10) = 0;
        CList__AddHead(&gCache_cacheFreeList, temp_s0);
    }
}
#endif
