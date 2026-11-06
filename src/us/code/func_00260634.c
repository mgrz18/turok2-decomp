#include "common.h"
#include "structs/game_object.h"

void AI_UpdateTargets(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void AI_ComputeFrameMotion(void *self, CAI *ai, CAIFrameMotion *out);
f32 EnemyAI_GetModeTurnRate(void *self, CAI *ai);
void AI_TurnClamped(void *self, CAI *ai, f32 angle, f32 arg3);
f32 EnemyAI_GetTypeTurnRate(void *self, CAI *ai);
s32 EnemyAI_PickAttack(void *self, CAI *ai, CAITargets *t);
void CAI__SetMode(void *self, CAI *ai, s32 mode);

void EnemyStationary_Update(void *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    f32 range;
    s32 mode;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    AI_TurnClamped(self, ai, EnemyAI_GetModeTurnRate(self, ai), t.m_Sight.m_Angle);
    range = EnemyAI_GetTypeTurnRate(self, ai);
    if ((t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) <= range) {
        mode = EnemyAI_PickAttack(self, ai, &t);
        if (mode != 0) {
            ai->m_ModeLastFrame = -1;
            CAI__SetMode(self, ai, mode);
        } else {
            CAI__SetMode(self, ai, 10);
        }
    }
}
