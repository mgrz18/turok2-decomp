#include "common.h"
#include "globals/cache.h"
#include "protos/list.h"

/* An element of the lists at gCache_cacheUsedList - 0x14 and gCache_cacheUsedList. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
} ListElem;



void CCache__FreeCacheEntry(s32 arg0, ListElem *arg1) {
    CList__Remove(&gCache_cacheUsedList, arg1);
    arg1->unk10 = 0;
    CList__AddHead((sptr) (((__typeof__(&gCache_cacheUsedList))((s8 *)&gCache_cacheUsedList - 0x14))), arg1);
}
