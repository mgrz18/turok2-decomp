#include "common.h"
#include "structs/game_object.h"

extern CListedInstance *gScene_m_InstancePool_m_ActiveList;

s32 TotemDefense_AnyEnemyAlive_2(void) {
    CListedInstance *actor;

    for (actor = gScene_m_InstancePool_m_ActiveList; actor != 0; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}
