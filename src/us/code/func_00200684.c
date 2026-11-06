#include "common.h"
#include "structs/camera.h"

/* CList: insert arg2 after arg1, or at the tail when arg1 has no next. */
void CList__InsertAfter(CList *arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_v0;

    temp_v1 = *(s32 *)(arg1 + arg0->m_NextOffset);
    if (temp_v1 != 0) {
        *(s32 *)(temp_v1 + arg0->m_LastOffset) = arg2;
        temp_v0 = arg0->m_NextOffset;
        *(s32 *)(arg2 + temp_v0) = *(s32 *)(arg1 + temp_v0);
        *(s32 *)(arg1 + arg0->m_NextOffset) = arg2;
        *(s32 *)(arg2 + arg0->m_LastOffset) = arg1;
        var_v0 = arg0->m_nCount + 1;
    } else {
        temp_v1_2 = (s32) arg0->m_pTail;
        if (temp_v1_2 != 0) {
            *(s32 *)(arg2 + arg0->m_LastOffset) = temp_v1_2;
            *(s32 *)((s32) arg0->m_pTail + arg0->m_NextOffset) = arg2;
        } else {
            *(s32 *)(arg2 + arg0->m_LastOffset) = 0;
            arg0->m_pHead = (void *) arg2;
        }
        *(s32 *)(arg2 + arg0->m_NextOffset) = 0;
        arg0->m_pTail = (void *) arg2;
        var_v0 = arg0->m_nCount + 1;
    }
    arg0->m_nCount = var_v0;
}
