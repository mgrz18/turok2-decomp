#include "common.h"
#include "structs/emitter.h"

s32 CList__AddTail(void *, s32);                 /* extern */
s32 CList__Remove(void *, s32);                 /* extern */

/* Takes the first free emitter and moves it to the active list. */
s32 CEmitterPool__TakeEmitter(CEmitterPool *arg0) {
    s32 temp_s0;

    temp_s0 = (s32) arg0->m_FreeList.m_pHead;
    if (temp_s0 != 0) {
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddTail(&arg0->m_ActiveList, temp_s0);
    }
    return temp_s0;
}
