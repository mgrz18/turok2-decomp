#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, CEnemyAnimPick *table, s32 anim);

void EnemyAnim_NormalDeath_B0390(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimNormalDeath_B0390, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
