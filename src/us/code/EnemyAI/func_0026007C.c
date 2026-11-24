#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"


void EnemyAI_NukedFrozenUpdate(sptr arg0, CAI *arg1) {
    if (arg1->m_OnFireTimer <= 0.0f) {
        AI_StopSound(arg0, (sptr) arg1);
    }
    if (arg1->m_ModeTime >= 150.0f) {
        EnemyAI_NukedFrozenRemove(arg0, (sptr) arg1);
    }
}
