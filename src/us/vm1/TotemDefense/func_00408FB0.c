#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


/* Looks for a live player actor; the result is not used. */
void TotemDefense_FindLiveEnemy(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            break;
        }
    }
}
