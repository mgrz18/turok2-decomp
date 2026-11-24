#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

void AI_UpdateTargets(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);

void BlindOne_BossMode3Update(CGameObjectInstance *actor, CAI *ai) {
    CAITargets targets;

    AI_UpdateTargets(actor, ai, &targets, 0, 1);
    if (ai->m_ModeTime > ai->m_ConditionTimer) {
        CAI__SetMode(actor, ai, 5);
    }
}
