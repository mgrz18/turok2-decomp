#include "common.h"
#include "structs/cache.h"

s32 CHeap__Free(s32, s32);
s32 CCache__FreeMemEntry(s32, s32);
s32 CCache__DetachCacheEntry(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 gHeap;
extern s32 D_800D8DB0;
extern s32 D_800D8DCC;

void CCache__DeallocAndReplaceCompressedData(s32 arg0, CMemEntry **arg1, CMemEntry *arg2) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    CMemEntry *temp_s0;

    temp_a0 = __osDisableInt();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg((s32) &D_800D8DB0, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_s0 = *arg1;
    temp_s0->flags = (temp_s0->flags & ~2);
    if (temp_s0->lockCount != 0) {
loop_4:
        do {
            temp_v0 = temp_s0->lockCount - 1;
            temp_s0->lockCount = temp_v0;
            if (temp_v0 != 0) {
                goto loop_4;
            }
            temp_s0->flags = (temp_s0->flags & ~0x100);
        } while (temp_s0->lockCount != 0);
    }
    if (!(temp_s0->flags & 0x702)) {
        CCache__DetachCacheEntry(0, (s32) temp_s0);
        CHeap__Free((s32) &gHeap, temp_s0->data);
        CCache__FreeMemEntry(0, (s32) temp_s0);
    }
    *arg1 = arg2;
    temp_v0_2 = __osDisableInt();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_2);
        osSendMesg((s32) &D_800D8DB0, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_2);
}
