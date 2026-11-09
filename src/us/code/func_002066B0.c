#include "common.h"
#include "globals/cache.h"
#include "protos/list.h"

/* An element of the lists at gCache_cacheUsedList - 0x14 and gCache_cacheUsedList. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
} ListElem;



#ifdef TARGET_PORT
/* The port: CCacheEntry.flags and gCache_cacheFreeList by name (structs/cache.h); the N64 0x10
 * and the list 0x14 bytes before the used list are not the host's. */
#include "structs/cache.h"

void CCache__FreeCacheEntry(s32 arg0, CCacheEntry *arg1) {
    CList__Remove(&gCache_cacheUsedList, arg1);
    arg1->flags = 0;
    CList__AddHead(&gCache_cacheFreeList, arg1);
}
#else
void CCache__FreeCacheEntry(s32 arg0, ListElem *arg1) {
    CList__Remove((s32) &gCache_cacheUsedList, (s32) arg1);
    arg1->unk10 = 0;
    CList__AddHead((s32) (((__typeof__(&gCache_cacheUsedList))((s8 *)&gCache_cacheUsedList - 0x14))), (s32) arg1);
}
#endif
