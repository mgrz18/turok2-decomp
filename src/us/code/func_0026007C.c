#include "common.h"
#include "structs/game_object.h"

s32 AI_StopSound(s32, s32);
s32 EnemyAI_NukedFrozenRemove(sptr, sptr);

void EnemyAI_NukedFrozenUpdate(sptr arg0, CAI *arg1) {
    if (arg1->m_OnFireTimer <= 0.0f) {
        AI_StopSound(arg0, (sptr) arg1);
    }
    if (arg1->m_ModeTime >= 150.0f) {
        EnemyAI_NukedFrozenRemove(arg0, (sptr) arg1);
    }
}
