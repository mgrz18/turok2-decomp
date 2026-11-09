#include "common.h"
#include "structs/camera.h"

/* CList: inserts element arg2 before element arg1 (at the head when arg1 has no previous). */
void CList__InsertBefore(CList *arg0, void *arg1, void *arg2) {
    s32 temp_v0;
    void *temp_v1;
    void *temp_v1_2;
    s32 var_v0;

    temp_v1 = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    if (temp_v1 != 0) {
        *(void **)((u8 *)temp_v1 + arg0->m_NextOffset) = arg2;
        temp_v0 = arg0->m_LastOffset;
        *(void **)((u8 *)arg2 + temp_v0) = *(void **)((u8 *)arg1 + temp_v0);
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = arg2;
        *(void **)((u8 *)arg2 + arg0->m_NextOffset) = arg1;
        var_v0 = arg0->m_nCount + 1;
    } else {
        temp_v1_2 = arg0->m_pHead;
        if (temp_v1_2 != 0) {
            *(void **)((u8 *)arg2 + arg0->m_NextOffset) = temp_v1_2;
            *(void **)((u8 *)arg0->m_pHead + arg0->m_LastOffset) = arg2;
        } else {
            *(void **)((u8 *)arg2 + arg0->m_NextOffset) = 0;
            arg0->m_pTail = arg2;
        }
        *(void **)((u8 *)arg2 + arg0->m_LastOffset) = 0;
        arg0->m_pHead = arg2;
        var_v0 = arg0->m_nCount + 1;
    }
    arg0->m_nCount = var_v0;
}
