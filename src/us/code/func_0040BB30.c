#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* AI state: turn toward the target, fire (MotherBoss_TeleportTarget) every third step, and go to anim state 0x16 once
 * the anim is done. */


extern void AI_UpdateTargets(void *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnClamped(void *obj, CAI *ai, f32 angle, f32 dist);
extern void MotherBoss_TeleportTarget(void *obj, s32 first);
extern void CAI__SetMode(void *obj, CAI *ai, s32 state);

void MotherBoss_Mode28_TeleportTarget(void *obj, CAI *ai) {
    CAITargets t;
    MotherBossState *state;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    state = &D_8012FB70;
    if (state->counter % 3 == 0) {
        MotherBoss_TeleportTarget(obj, state->counter == 0);
    }
    state->counter++;
    if (ai->m_AnimRepeatedCount) {
        state->counter = 0;
        CAI__SetMode(obj, ai, 0x16);
    }
}
