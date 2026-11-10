#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "protos/ai.h"

/* Flying AI state: steer towards the target, attack when possible (resetting
 * the attack delay), and fall back to state 10 once the timer runs out. */

void AI_UpdateTargets(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void AI_ComputeFrameMotion(void *self, CAI *ai, CAIFrameMotion *out);
void EnemyFly_SteerToPath(void *self, CAI *ai, CAITargets *t, CAIFrameMotion *steer);
#ifdef TARGET_PORT
s16 EnemyAI_PickAttack(void *self, CAI *ai, CAITargets *t); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_PickAttack(void *self, CAI *ai, CAITargets *t);
#endif
f32 AI_LerpByAggression(void *self, CAI *ai, f32 lo, f32 hi);

void EnemyHover_MoveUpdate(void *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    s32 mode;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    EnemyFly_SteerToPath(self, ai, &t, &steer);
    mode = EnemyAI_PickAttack(self, ai, &t);
    if (mode != 0) {
        ai->m_ModeLastFrame = -1;
        CAI__SetMode(self, ai, mode);
        ai->m_Action = 0;
        ai->m_ActionTime = AI_LerpByAggression(self, ai, 90.0f, 30.0f);
    }
    ai->m_ConditionTimer += gFrameIncrement;
    if (AI_LerpByAggression(self, ai, 60.0f, 120.0f) < ai->m_ConditionTimer) {
        ai->m_ConditionTimer = 0.0f;
        CAI__SetMode(self, ai, 10);
    }
}
