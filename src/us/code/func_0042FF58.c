#include "common.h"
#include "structs/camera.h"

/* A pool of elements on a free list and an active list (CList__*), laid out
 * like the head of CCameraPool. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ CList m_FreeList;
    /* 0x20 */ CList m_ActiveList;
} CElementPool;

s32 CList__AddTail(CList *, s32);
s32 CList__Remove(CList *, s32);
s32 func_0042F9B0(s32);

/* Takes the first free element, resets it and moves it to the active list. */
s32 func_0042FF58(CElementPool *arg0) {
    s32 temp_s0;

    temp_s0 = (s32) arg0->m_FreeList.m_pHead;
    if (temp_s0 != 0) {
        func_0042F9B0(temp_s0);
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddTail(&arg0->m_ActiveList, temp_s0);
    }
    return temp_s0;
}
