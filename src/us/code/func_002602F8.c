#include "common.h"
#include "structs/game_object.h"

s32 EnemyAI_HasComboEndAnim(CGameObjectInstance *arg0, CAI *arg1) {
    CEnemyIntelligence *temp_a0;

    temp_a0 = &(arg0->header.m_pIntelligence)->m_Enemy;
    if (((arg1->m_Enemy.m_Behavior[0] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x1F)) || ((arg1->m_Enemy.m_Behavior[1] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x3E0)) || ((arg1->m_Enemy.m_Behavior[2] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x7C00))) {
        return 1;
    }
    return 0;
}
