#include "common.h"
#include "structs/game_object.h"

s32 AI_StopSound(s32, s32);
s32 EnemyAI_NukedFrozenRemove(s32, s32);

void EnemyAI_NukedFrozenUpdate(s32 arg0, CAI *arg1) {
    if (arg1->m_OnFireTimer <= 0.0f) {
        AI_StopSound(arg0, (s32) arg1);
    }
    if (arg1->m_ModeTime >= 150.0f) {
        EnemyAI_NukedFrozenRemove(arg0, (s32) arg1);
    }
}
