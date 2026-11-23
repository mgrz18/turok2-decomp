#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_RightArmWound_AF9F0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimRightArmWound_AF9F0, obj->header.m_pIntelligence->m_Enemy.m_wRightArmWoundAnimFlags);
}
