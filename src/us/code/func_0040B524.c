#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

/* AI state: turn toward the target (15 degrees per step), resetting the step counter, then go to anim state 0xC. */

extern void AI_UpdateTargets(void *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnClamped(void *obj, CAI *ai, f32 angle, f32 dist);
extern void CAI__SetMode(void *obj, CAI *ai, s32 state);

void MotherBoss_Mode17_TrackTarget(void *obj, CAI *ai) {
    CAITargets t;

    gMotherBoss.counter = 0;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(obj, ai, 0xC);
    }
}
