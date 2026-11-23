#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

void AI_UpdateTargets(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void AI_ComputeFrameMotion(void *self, CAI *ai, CAIFrameMotion *out);
void EnemyFly_SteerToPathScaled(void *self, CAI *ai, CAITargets *t, CAIFrameMotion *steer, f32 speed, f32 arg5);

void EnemyRam_CombatUpdate(void *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    f32 dist;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    dist = t.m_Path.m_FlatDist;
    if (!(dist >= 1024.0f)) {
        dist = 1024.0f;
    }
    EnemyFly_SteerToPathScaled(self, ai, &t, &steer, 6.0f - dist * (1.0f / 1024) * 3.0f, 2.0f);
    if ((t.m_Sight.m_pTarget != 0 && (t.m_Sight.m_vDelta.y < 0.0f ? -t.m_Sight.m_vDelta.y : t.m_Sight.m_vDelta.y) < 51.199997f) || ai->m_ModeTime > 90.0f) {
        CAI__SetMode(self, ai, 4);
    }
}
