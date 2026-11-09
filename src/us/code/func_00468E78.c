#include "common.h"
#include "structs/list.h"
#include "protos/list.h"

/* A pool with a free list and a used list. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ CList freeList;
    /* 0x20 */ CList usedList;
} Pool;


s32 CCamera__Construct_2(s32);                         /* extern */

sptr CCameraPool__AllocCamera_2(Pool *arg0) {
    sptr temp_s0;

    temp_s0 = (sptr) arg0->freeList.m_pHead;
    if (temp_s0 != 0) {
        CCamera__Construct_2(temp_s0);
        CList__Remove(&arg0->freeList, temp_s0);
        CList__AddTail(&arg0->usedList, temp_s0);
    }
    return temp_s0;
}
