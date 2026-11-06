#include "common.h"

/* An element of the lists at gCache_cacheUsedList - 0x14 and gCache_cacheUsedList. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
} ListElem;

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);

extern s32 gCache_cacheUsedList;

void CCache__FreeCacheEntry(s32 arg0, ListElem *arg1) {
    CList__Remove((s32) &gCache_cacheUsedList, (s32) arg1);
    arg1->unk10 = 0;
    CList__AddHead((s32) (((__typeof__(&gCache_cacheUsedList))((s8 *)&gCache_cacheUsedList - 0x14))), (s32) arg1);
}
