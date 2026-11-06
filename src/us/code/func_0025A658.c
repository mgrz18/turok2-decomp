#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

extern s32 D_800AFEE8;

void EnemyAnim_Leap_AFEE8(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) &D_800AFEE8, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}
