#include "common.h"
#include "structs/camera.h"

/* Appends arg1 to the list; m_LastOffset and m_NextOffset say where in an
 * element its links are. */
void CList__AddTail(CList *arg0, void *arg1) {
    void *temp_v1;

    temp_v1 = arg0->m_pTail;
    if (temp_v1 != 0) {
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = temp_v1;
        *(void **)((u8 *)arg0->m_pTail + arg0->m_NextOffset) = arg1;
    } else {
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = 0;
        arg0->m_pHead = arg1;
    }
    *(void **)((u8 *)arg1 + arg0->m_NextOffset) = 0;
    arg0->m_pTail = arg1;
    arg0->m_nCount = (s32) (arg0->m_nCount + 1);
}
