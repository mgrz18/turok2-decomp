#include "common.h"
#include "structs/camera.h"

/* Moves element arg1 to the head of the list; m_LastOffset and
 * m_NextOffset say where in an element its links are. */
void CList__MoveToHead(CList *arg0, void *arg1) {
    void *temp_a2;
    void *temp_a2_2;
    s32 temp_v0;
    s32 temp_v0_2;
    void *temp_v1;

    temp_a2 = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    if (temp_a2 != 0) {
        temp_v0 = arg0->m_NextOffset;
        *(void **)((u8 *)temp_a2 + temp_v0) = *(void **)((u8 *)arg1 + temp_v0);
    }
    temp_a2_2 = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    if (temp_a2_2 != 0) {
        temp_v0_2 = arg0->m_LastOffset;
        *(void **)((u8 *)temp_a2_2 + temp_v0_2) = *(void **)((u8 *)arg1 + temp_v0_2);
    }
    if (arg0->m_pHead == arg1) {
        arg0->m_pHead = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    }
    if (arg0->m_pTail == arg1) {
        arg0->m_pTail = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    }
    temp_v1 = arg0->m_pHead;
    arg0->m_nCount = (s32) (arg0->m_nCount - 1);
    if (temp_v1 != 0) {
        *(void **)((u8 *)arg1 + arg0->m_NextOffset) = temp_v1;
        *(void **)((u8 *)arg0->m_pHead + arg0->m_LastOffset) = arg1;
    } else {
        *(void **)((u8 *)arg1 + arg0->m_NextOffset) = 0;
        arg0->m_pTail = arg1;
    }
    *(void **)((u8 *)arg1 + arg0->m_LastOffset) = 0;
    arg0->m_pHead = arg1;
    arg0->m_nCount = (s32) (arg0->m_nCount + 1);
}
