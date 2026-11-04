#include "common.h"
#include "structs/camera.h"

/* Links element arg1 in front of the list head; the links live at the list offsets inside it. */
void CList__AddHead(CList *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = (s32) arg0->m_pHead;
    if (temp_v1 != 0) {
        *(s32 *)(arg1 + arg0->m_NextOffset) = temp_v1;
        *(s32 *)((s32) arg0->m_pHead + arg0->m_LastOffset) = arg1;
    } else {
        *(s32 *)(arg1 + arg0->m_NextOffset) = 0;
        arg0->m_pTail = (void *) arg1;
    }
    *(s32 *)(arg1 + arg0->m_LastOffset) = 0;
    arg0->m_pHead = (void *) arg1;
    arg0->m_nCount = (s32) (arg0->m_nCount + 1);
}
