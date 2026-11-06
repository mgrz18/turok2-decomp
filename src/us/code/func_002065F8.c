#include "common.h"
#include "m2c_macros.h"

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);
s32 CHashTable__Remove(s32);

extern M2C_UNK gCache_cacheFreeList;
extern M2C_UNK gCache_cacheUsedList;

void CCache__DetachCacheEntry(s32 arg0, void *arg1) {
    void *temp_s0;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg1, void **, 0x14);
    if (temp_v0 != NULL) {
        CHashTable__Remove(M2C_FIELD(temp_v0, s32 *, 8));
        temp_s0 = M2C_FIELD(arg1, void **, 0x14);
        CList__Remove((s32) &gCache_cacheUsedList, (s32) temp_s0);
        M2C_FIELD(temp_s0, s32 *, 0x10) = 0;
        CList__AddHead((s32) &gCache_cacheFreeList, (s32) temp_s0);
    }
}
