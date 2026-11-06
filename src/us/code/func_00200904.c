#include "common.h"
#include "structs/camera.h"

/* CList: unlinks element arg1 and appends it at the tail. */
void CList__MoveToTail(CList *arg0, s32 arg1) {
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;

    temp_a2 = *(s32 *)(arg1 + arg0->m_LastOffset);
    if (temp_a2 != 0) {
        temp_v0 = arg0->m_NextOffset;
        *(s32 *)(temp_a2 + temp_v0) = *(s32 *)(arg1 + temp_v0);
    }
    temp_a2_2 = *(s32 *)(arg1 + arg0->m_NextOffset);
    if (temp_a2_2 != 0) {
        temp_v0_2 = arg0->m_LastOffset;
        *(s32 *)(temp_a2_2 + temp_v0_2) = *(s32 *)(arg1 + temp_v0_2);
    }
    if ((s32) arg0->m_pHead == arg1) {
        arg0->m_pHead = (void *) *(s32 *)(arg1 + arg0->m_NextOffset);
    }
    if ((s32) arg0->m_pTail == arg1) {
        arg0->m_pTail = (void *) *(s32 *)(arg1 + arg0->m_LastOffset);
    }
    temp_v1 = (s32) arg0->m_pTail;
    arg0->m_nCount = arg0->m_nCount - 1;
    if (temp_v1 != 0) {
        *(s32 *)(arg1 + arg0->m_LastOffset) = temp_v1;
        *(s32 *)((s32) arg0->m_pTail + arg0->m_NextOffset) = arg1;
    } else {
        *(s32 *)(arg1 + arg0->m_LastOffset) = 0;
        arg0->m_pHead = (void *) arg1;
    }
    *(s32 *)(arg1 + arg0->m_NextOffset) = 0;
    arg0->m_pTail = (void *) arg1;
    arg0->m_nCount = arg0->m_nCount + 1;
}
