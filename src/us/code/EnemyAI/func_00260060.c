#include "common.h"
#include "structs/game_object.h"

s32 EnemyAI_DrawWithBlend(s32, s32);

void EnemyAI_NukedFrozenEnter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = EnemyAI_DrawWithBlend;
    ai->m_OnFireTimer = 75.0f;
}
