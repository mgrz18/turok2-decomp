#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Combat_AFE88(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((s32) obj, arg1, (s32) sEnemyAnimCombat_AFE88, obj->header.m_pIntelligence->m_Enemy.m_wCombatAnimFlags);
}
