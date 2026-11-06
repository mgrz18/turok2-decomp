#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

s32 CAI__SetMode(s32, s32, s32);


void MotherBoss_KillMinions(void) {
    CListedInstance *var_s0;

    var_s0 = gScene.m_InstancePool.m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_Instance.m_AI.m_Health > 0) {
                var_s0->m_Instance.m_AI.m_Health = 0;
                CAI__SetMode((s32) var_s0, (s32) &var_s0->m_Instance.m_AI, 0x34);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
