/* The second VM module's copy of src/us/vm1/TotemDefense/func_00408DCC.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"

void TotemDefense_StopEnemies(void) {
    CListedInstance *actor;

#ifdef TARGET_PORT
    gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead->m_FleeTime = 450.0f; /* N64 0xAAC */
#else
    *(f32 *) ((u8 *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead + 0xAAC) = 450.0f; /* the current player */
#endif
    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            actor->m_Instance.m_AI.m_pPathTarget = NULL;
            actor->m_Instance.m_AI.m_pSightTarget = NULL;
            actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
}
