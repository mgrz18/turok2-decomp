#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);
extern f32 D_800B6D28;

s32 EnemyAI_TickTimers(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    s8 temp_v1;

    if (arg0->header.m_pIntelligence->m_dwTypeFlags & 0x100000) {
        temp_f0 = arg1->m_Enemy.m_Timer - D_800B6D28;
        arg1->m_Enemy.m_Timer = temp_f0;
        if (temp_f0 < 0.0f) {
            temp_f0_2 = arg1->m_Enemy.m_Timer2 - D_800B6D28;
            arg1->m_Enemy.m_Timer = 0.0f;
            arg1->m_Enemy.m_Timer2 = temp_f0_2;
            if (temp_f0_2 < 0.0f) {
                arg1->m_Enemy.m_Timer2 = 0.0f;
            }
        }
        if ((arg1->m_Health != 0) && (arg1->m_Enemy.m_Timer <= 0.0f)) {
            if (arg1->m_Enemy.m_Timer2 <= 0.0f) {
                temp_v1 = arg1->m_Mode;
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    CAI__SetMode((s32) arg0, (s32) arg1, 0x19);
                    return 1;
                }
                return 0;
            }
            goto block_15;
        }
        return 0;
    }
block_15:
    return 0;
}
