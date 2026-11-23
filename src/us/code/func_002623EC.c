#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "protos/ai.h"

void Angle_Wrap(f32 *angle);
void AI_UpdateTargets(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
f32 GameObject_GetCollisionHeight(void *actor);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
f32 CVector3__GetPitch(CVector3 *v);
void Turret_UpdateHinge(CGameObjectInstance *self, CAI *ai, s32 mode, CGameObjectInstance *target);
f32 Math_Approach(f32 cur, f32 target, f32 rate);

s32 Turret_Aim(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CVector3 from;
    CVector3 to;
    CVector3 delta;
    f32 yaw;
    CTurretIntelligence *params;
    CGameObjectInstance *target;
    s32 fire;
    f32 pitch;
    f32 turn;
    f32 sum;
    f32 limit;

    yaw = self->m_RotY - ai->m_InitialRotY;
    params = &self->header.m_pIntelligence->m_Turret;
    Angle_Wrap(&yaw);
    AI_UpdateTargets(self, ai, &t, 0, 1);
    target = ai->m_pSightTarget;
    fire = 0;
    if (target != NULL && t.m_Sight.m_FlatDist <= params->m_AimRange
        && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) < params->m_AimFov * 0.5f) {
        if (t.m_Sight.m_FlatDist <= params->m_FireRange
            && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) < params->m_FireFov * 0.5f) {
            fire = 1;
        }
        from = self->header.m_vPos;
        from.y += GameObject_GetCollisionHeight(self) * 0.5f;
        to = target->header.m_vPos;
        to.y += GameObject_GetCollisionHeight(target) * 0.5f;
        CVector3__Subtract(&delta, &to, &from);
        pitch = CVector3__GetPitch(&delta);
        turn = t.m_Sight.m_Angle;
        if (params->m_TurnArc < 6.283186f) {
            limit = params->m_TurnArc * 0.5f;
            sum = yaw + turn;
            if (limit < sum) {
                turn = limit - yaw;
            } else if (sum < -limit) {
                turn = -limit - yaw;
            }
        }
        Turret_UpdateHinge(self, ai, params->m_AimMode, target);
    } else {
        pitch = 0.0f;
        turn = -yaw;
        Turret_UpdateHinge(self, ai, params->m_IdleMode, target);
    }
    AI_TurnClamped(self, ai, params->m_RotSpeed, turn);
    ai->m_Turret.m_Pitch = Math_Approach(ai->m_Turret.m_Pitch, pitch, params->m_RotSpeed);
    return fire;
}
