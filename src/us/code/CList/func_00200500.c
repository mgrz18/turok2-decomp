#include "common.h"
#include "structs/camera.h"
#include "protos/list.h"

void CList__Construct(CList *arg0, s32 arg1, s32 arg2) {
    arg0->m_LastOffset = arg1;
    arg0->m_NextOffset = arg2;
    arg0->m_pTail = 0;
    arg0->m_pHead = 0;
    arg0->m_nCount = 0;
}
