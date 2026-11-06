#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 sEnemyAnimMovingDeath_B01B0[];

void EnemyAnim_MovingDeath_B01B0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimMovingDeath_B01B0, obj->header.m_pIntelligence->m_Enemy.m_wMovingDeathAnimFlags);
}
