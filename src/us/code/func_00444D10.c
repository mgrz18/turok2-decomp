#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern CListedInstance *D_8010FEB4;

void MotherBoss_KillMinions_2(void) {
    CListedInstance *var_s0;

    var_s0 = D_8010FEB4;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_Instance.m_AI.m_Health > 0) {
                var_s0->m_Instance.m_AI.m_Health = 0;
                CAI__SetMode(&var_s0->m_Instance, &var_s0->m_Instance.m_AI, 0x34);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
