#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 sEnemyAnimEvade_B0480[];

void EnemyAnim_Evade_B0480(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimEvade_B0480, obj->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}
