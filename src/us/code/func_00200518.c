#include "common.h"
#include "structs/camera.h"
#include "protos/list.h"

/* Links element arg1 in front of the list head; the links live at the list offsets inside it. */
void CList__AddHead(CList *arg0, void *arg1) {
    void *temp_v1;

    temp_v1 = arg0->m_pHead;
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
