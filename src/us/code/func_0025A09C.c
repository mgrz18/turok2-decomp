#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 sEnemyAnimRightArmWound_AF9F0[];

void EnemyAnim_RightArmWound_AF9F0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimRightArmWound_AF9F0, obj->header.m_pIntelligence->m_Enemy.m_wRightArmWoundAnimFlags);
}
