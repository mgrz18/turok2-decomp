#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_NormalDeath_B0390(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimNormalDeath_B0390, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
