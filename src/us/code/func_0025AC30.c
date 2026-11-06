#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 D_800B0570[];

void EnemyAnim_Move_B0570(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, D_800B0570, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
