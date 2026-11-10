#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

void AI_UpdateTargets(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void AI_ComputeFrameMotion(CGameObjectInstance *self, CAI *ai, CAIFrameMotion *out);
void EnemyFly_SteerToPathScaled(CGameObjectInstance *self, CAI *ai, CAITargets *t, CAIFrameMotion *steer, f32 speed, f32 arg5);
#ifdef TARGET_PORT
s16 EnemyAI_PickAttack(CGameObjectInstance *self, CAI *ai, CAITargets *t); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_PickAttack(CGameObjectInstance *self, CAI *ai, CAITargets *t);
#endif

void EnemyRam_MoveUpdate(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    s32 next;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    t.m_Path.m_vDelta.y = t.m_Sight.m_vDelta.y = ai->m_vLeashPos.y - self->header.m_vPos.y;
    if (ai->m_Action == 1) {
        EnemyFly_SteerToPathScaled(self, ai, &t, &steer, 2.0f, 2.0f);
    } else {
        EnemyFly_SteerToPathScaled(self, ai, &t, &steer, 1.0f, 1.0f);
    }
    if (t.m_Sight.m_pTarget != 0 && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) <= 1.5707965f) {
        next = EnemyAI_PickAttack(self, ai, &t);
        if (next != 0) {
            ai->m_ModeLastFrame = -1;
            CAI__SetMode(self, ai, next);
        }
    }
}
