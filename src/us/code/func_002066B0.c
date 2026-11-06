#include "common.h"

/* An element of the lists at D_800D8D7C - 0x14 and D_800D8D7C. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
} ListElem;

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);

extern s32 D_800D8D7C;

void CCache__FreeCacheEntry(s32 arg0, ListElem *arg1) {
    CList__Remove((s32) &D_800D8D7C, (s32) arg1);
    arg1->unk10 = 0;
    CList__AddHead((s32) (((__typeof__(&D_800D8D7C))((s8 *)&D_800D8D7C - 0x14))), (s32) arg1);
}
