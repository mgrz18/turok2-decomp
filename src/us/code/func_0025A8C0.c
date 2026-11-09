#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_MovingDeath_B01B0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimMovingDeath_B01B0, obj->header.m_pIntelligence->m_Enemy.m_wMovingDeathAnimFlags);
}
