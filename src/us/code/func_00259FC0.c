#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 sEnemyAnimTakeCover_AF900[];

void EnemyAnim_TakeCover_AF900(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimTakeCover_AF900, obj->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}
