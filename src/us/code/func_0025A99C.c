#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_ViolentDeath_B02A0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimViolentDeath_B02A0, obj->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}
