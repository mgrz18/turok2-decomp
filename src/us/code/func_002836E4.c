#include "common.h"
#include "structs/camera.h"

s32 CList__AddTail(void *, s32);                 /* extern */
s32 CList__Remove(void *, s32);                 /* extern */

typedef struct {
    /* 0x000 */ u8 pad0[0x8A0];
    /* 0x8A0 */ CList freeList;
    /* 0x8B4 */ CList activeList;
} EmitterOwner;

/* Takes the first free emitter and moves it to the active list. */
s32 func_002836E4(EmitterOwner *arg0) {
    s32 temp_s0;

    temp_s0 = (s32) arg0->freeList.m_pHead;
    if (temp_s0 != 0) {
        CList__Remove(&arg0->freeList, temp_s0);
        CList__AddTail(&arg0->activeList, temp_s0);
    }
    return temp_s0;
}
