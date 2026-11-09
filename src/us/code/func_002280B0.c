#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/list.h"



void CListedInstance__Free(CListedInstance *arg0) {
    s32 *temp_v1;

    temp_v1 = arg0->m_pOwnerCount;
    arg0->m_Instance.m_AI.m_Health = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    CList__Remove(&gScene.m_InstancePool.m_ActiveList, arg0);
    CList__AddTail(&gScene_m_InstancePool_m_FreeList, arg0);
}
