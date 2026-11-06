#include "common.h"
#include "structs/game_object.h"

extern CListedInstance *gScene_m_InstancePool_m_ActiveList;

/* Is any actor whose definition kind is 1 still alive? */
s32 TotemDefense_AnyEnemyAlive(void) {
    CListedInstance *actor;

    for (actor = gScene_m_InstancePool_m_ActiveList; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}
