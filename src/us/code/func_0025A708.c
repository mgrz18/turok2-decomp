#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 D_800AFF18[];

void EnemyAnim_DartFallback_AFF18(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, D_800AFF18, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}
