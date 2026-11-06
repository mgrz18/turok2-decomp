#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 sEnemyAnimNormalDeath_B0390[];

void EnemyAnim_NormalDeath_B0390(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimNormalDeath_B0390, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
