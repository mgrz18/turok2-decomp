#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


s32 TotemDefense_AnyEnemyAlive_2(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != 0; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            return 1;
        }
    }
    return 0;
}
