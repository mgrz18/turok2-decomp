#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, AIAnimPick *table, s32 anim);

void EnemyAnim_MovingDeath_B01B0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimMovingDeath_B01B0, obj->header.m_pIntelligence->m_Enemy.m_wMovingDeathAnimFlags);
}
