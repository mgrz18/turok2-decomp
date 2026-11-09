#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "protos/ai.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 turnRate;
    /* 0x44 */ f32 turnArc;
    /* 0x48 */ f32 aimRange;
    /* 0x4C */ f32 aimFov;
    /* 0x50 */ f32 fireRange;
    /* 0x54 */ f32 fireFov;
    /* 0x58 */ u8 pad58[0x6C - 0x58];
    /* 0x6C */ u8 aimMode;
    /* 0x6D */ u8 idleMode;
} TurretParams;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ TurretParams params;
} TurretDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ TurretDef *def;
    /* 0x18 */ u8 pad18[0x50 - 0x18];
    /* 0x50 */ f32 rotY;
} Turret;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 pos;
} TargetActor;

void Angle_Wrap(f32 *angle);
void AI_UpdateTargets(Turret *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
f32 GameObject_GetCollisionHeight(void *actor);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
f32 CVector3__GetPitch(CVector3 *v);
void Turret_UpdateHinge(Turret *self, CAI *ai, s32 mode, TargetActor *target);
f32 Math_Approach(f32 cur, f32 target, f32 rate);

s32 Turret_Aim(Turret *self, CAI *ai) {
    CAITargets t;
    CVector3 from;
    CVector3 to;
    CVector3 delta;
    f32 yaw;
    TurretParams *params;
    TargetActor *target;
    s32 fire;
    f32 pitch;
    f32 turn;
    f32 sum;
    f32 limit;

    yaw = self->rotY - ai->m_InitialRotY;
    params = &self->def->params;
    Angle_Wrap(&yaw);
    AI_UpdateTargets(self, ai, &t, 0, 1);
    target = ai->m_pSightTarget;
    fire = 0;
    if (target != NULL && t.m_Sight.m_FlatDist <= params->aimRange
        && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) < params->aimFov * 0.5f) {
        if (t.m_Sight.m_FlatDist <= params->fireRange
            && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) < params->fireFov * 0.5f) {
            fire = 1;
        }
        from = self->pos;
        from.y += GameObject_GetCollisionHeight(self) * 0.5f;
        to = target->pos;
        to.y += GameObject_GetCollisionHeight(target) * 0.5f;
        CVector3__Subtract(&delta, &to, &from);
        pitch = CVector3__GetPitch(&delta);
        turn = t.m_Sight.m_Angle;
        if (params->turnArc < 6.283186f) {
            limit = params->turnArc * 0.5f;
            sum = yaw + turn;
            if (limit < sum) {
                turn = limit - yaw;
            } else if (sum < -limit) {
                turn = -limit - yaw;
            }
        }
        Turret_UpdateHinge(self, ai, params->aimMode, target);
    } else {
        pitch = 0.0f;
        turn = -yaw;
        Turret_UpdateHinge(self, ai, params->idleMode, target);
    }
    AI_TurnClamped(self, ai, params->turnRate, turn);
    ai->m_Turret.m_Pitch = Math_Approach(ai->m_Turret.m_Pitch, pitch, params->turnRate);
    return fire;
}
