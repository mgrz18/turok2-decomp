#include "common.h"
#include "structs/camera.h"

/* Appends arg1 to the list; m_LastOffset and m_NextOffset say where in an
 * element its links are. */
void CList__AddTail(CList *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = (s32) arg0->m_pTail;
    if (temp_v1 != 0) {
        *(s32 *)(arg1 + arg0->m_LastOffset) = temp_v1;
        *(s32 *)((s32) arg0->m_pTail + arg0->m_NextOffset) = arg1;
    } else {
        *(s32 *)(arg1 + arg0->m_LastOffset) = 0;
        arg0->m_pHead = (void *) arg1;
    }
    *(s32 *)(arg1 + arg0->m_NextOffset) = 0;
    arg0->m_pTail = (void *) arg1;
    arg0->m_nCount = (s32) (arg0->m_nCount + 1);
}
