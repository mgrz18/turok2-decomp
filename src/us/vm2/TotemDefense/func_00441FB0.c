/* The second VM module's copy of src/us/vm1/TotemDefense/func_00408FB0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


/* Looks for a live player actor; the result is not used. */
void TotemDefense_FindLiveEnemy_2(void) {
    CListedInstance *actor;

    for (actor = gScene.m_InstancePool.m_ActiveList.m_pHead; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            break;
        }
    }
}
