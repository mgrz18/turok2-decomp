#include "common.h"

typedef struct IntrusiveList {
    /* 0x00 */ void *head;
    /* 0x04 */ void *tail;
    /* 0x08 */ s32 prevOffset;
    /* 0x0C */ s32 nextOffset;
    /* 0x10 */ s32 count;
} IntrusiveList;

/* A pool with a free list and a used list. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ IntrusiveList freeList;
    /* 0x20 */ IntrusiveList usedList;
} Pool;

s32 CList__AddTail(s32, s32);
s32 CList__Remove(s32, s32);

s32 func_0042F8D0(s32);                         /* extern */

s32 func_00468E78(Pool *arg0) {
    s32 temp_s0;

    temp_s0 = (s32) arg0->freeList.head;
    if (temp_s0 != 0) {
        func_0042F8D0(temp_s0);
        CList__Remove((s32) &arg0->freeList, temp_s0);
        CList__AddTail((s32) &arg0->usedList, temp_s0);
    }
    return temp_s0;
}
