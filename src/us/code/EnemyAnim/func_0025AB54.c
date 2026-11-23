#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_Evade_B0480(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimEvade_B0480, obj->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}
