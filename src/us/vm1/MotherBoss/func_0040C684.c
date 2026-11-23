#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

/* AI state: turn toward the target (15 degrees per step), then go to anim state 0x16. */

extern void AI_UpdateTargets(void *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);

void MotherBoss_TurnThenPhase2Choose(void *obj, CAI *ai) {
    CAITargets t;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0x16);
    }
}
