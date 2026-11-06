#include "common.h"
#include "structs/list.h"

/* A pool with a free list and a used list. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ CList freeList;
    /* 0x20 */ CList usedList;
} Pool;

s32 CList__AddTail(s32, s32);
s32 CList__Remove(s32, s32);

s32 CCamera__Construct_2(s32);                         /* extern */

s32 CCameraPool__AllocCamera_2(Pool *arg0) {
    s32 temp_s0;

    temp_s0 = (s32) arg0->freeList.m_pHead;
    if (temp_s0 != 0) {
        CCamera__Construct_2(temp_s0);
        CList__Remove((s32) &arg0->freeList, temp_s0);
        CList__AddTail((s32) &arg0->usedList, temp_s0);
    }
    return temp_s0;
}
