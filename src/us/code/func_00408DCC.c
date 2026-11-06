#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


extern u8 *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;          /* current player */

void TotemDefense_StopEnemies(void) {
    CListedInstance *actor;

    *(f32 *) (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead + 0xAAC) = 450.0f;
    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            actor->m_Instance.m_AI.m_pPathTarget = NULL;
            actor->m_Instance.m_AI.m_pSightTarget = NULL;
            actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
}
