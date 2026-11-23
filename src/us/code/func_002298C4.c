#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/particle.h"
#include "structs/sound.h"
#include "protos/game.h"

extern void *LoopSounds_Start(sptr bank, s32 sound, CVector3 *pos, CVector3 *pos2, sptr loops);  /* starts a sound */
extern void LoopSounds_Stop(sptr bank, void *handle);                                  /* stops a sound */
extern void CParticle__StopLoopSound(void *obj);

extern f32 HalfToFloat(s32 half);                         /* half float -> f32 */
extern f32 func_00298470(f32);                              /* sqrtf */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a - b */
extern void CVector3__Normalize(CVector3 *v);                        /* normalize */
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);          /* rotation facing dir */
extern void CQuaternion__Normalize(CQuaternion *q);                        /* normalize */
extern void CQuaternion__SlerpFast(CQuaternion *out, f32 t, CQuaternion *a, CQuaternion *b); /* slerp */
extern void CQuaternion__ToMatrix(CQuaternion *q, CMtxF m);               /* to matrix */
extern void CMtxF__TransformPoint(CMtxF m, CVector3 *in, CVector3 *out);  /* transform */
extern void CParticle__Wobble(void *obj, f32 scale);



/* Same as CParticle__Accelerate: accelerate along dir, clamp to the max speed. */
static __inline__ void Missile_Accelerate(CParticle *obj) {
    CVector3 delta;
    CVector3 dir;
    CVector3 *vel;
    f32 accel;
    f32 speed;
    f32 maxSpeed;

    accel = HalfToFloat(obj->m_pEffect->m_pMotion->m_Accel);
    if (accel != 0.0f) {
        dir = obj->m_vDirection;
        CVector3__Scale(&delta, &dir, accel);
        vel = &obj->m_vVelocity;
        CVector3__Add(vel, vel, &delta);
        speed = func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z);
        maxSpeed = HalfToFloat(obj->m_pEffect->m_pMotion->m_MaxSpeed);
        if (accel > 0.0f) {
            if (maxSpeed < speed) {
                CVector3__Scale(vel, &dir, maxSpeed);
            }
        } else {
            if (speed < maxSpeed) {
                CVector3__Scale(vel, &dir, maxSpeed);
            }
        }
    }
}

/* A homing missile: keeps its flight sound 0x1B2 playing, turns toward its
 * target (aiming above it when far), then accelerates. */
void CParticle__UpdateHomingMissile(CParticle *obj) {
    CVector3 targetPos;
    CVector3 toTarget;
    CVector3 forward;
    CQuaternion cur;
    CQuaternion want;
    CQuaternion rot;
    CMtxF mtx;
    CGameObjectInstance *target;
    f32 speed;
    f32 dist;
    f32 t;
    f32 far;
    SndSource *loop;
    s32 sound;

    if (obj->m_dwFlags & 0x10000) {
        if (obj->m_pLoopSound != NULL) {
            LoopSounds_Stop(LoopSounds_Get(), obj->m_pLoopSound);
            obj->m_pLoopSound = NULL;
        }
        return;
    }
    loop = obj->m_pLoopSound;
    sound = 0x1B2;
    if (loop != NULL) {
        if (loop->sound.w == sound && loop->handle != -1) {
            goto playing;
        }
        CParticle__StopLoopSound(obj);
    }
    obj->m_pLoopSound = LoopSounds_Start(LoopSounds_Get(), 0x1B2, &obj->header.m_vPos, &obj->header.m_vPos, -1);
playing:

    target = obj->m_pTarget;
    if (target != NULL && obj->m_cFrame > 7.0f) {
        CQuaternion__FromDirection(&cur, &obj->m_vDirection);
        speed = func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z);
        targetPos = target->m_AI.m_vBoreTarget;
        CVector3__Subtract(&toTarget, &targetPos, &obj->header.m_vPos);
        dist = func_00298470(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z);
        if (dist < 102.399994f) {
            CQuaternion__FromDirection(&rot, &toTarget);
            CQuaternion__ToMatrix(&rot, mtx);
            forward.x = 0.0f;
            forward.y = 0.0f;
            forward.z = 1.0f;
            CMtxF__TransformPoint(mtx, &forward, &obj->m_vDirection);
            CVector3__Scale(&obj->m_vVelocity, &obj->m_vDirection, speed);
            return;
        }
        if (dist > 153.59999f) {
            targetPos.y += 102.399994f;
            CVector3__Subtract(&toTarget, &targetPos, &obj->header.m_vPos);
            dist = func_00298470(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z);
        }
        CVector3__Normalize(&toTarget);
        CQuaternion__FromDirection(&want, &toTarget);
        if (dist < 1024.0f) {
            far = dist - 204.79999f;
            if (far < 0.0f) {
                t = 0.0f;
            } else {
                t = far * 0.0012207031f;
            }
            t = 1.0f - t * 0.85999995f;
        } else {
            t = 0.04f;
        }
        CQuaternion__Normalize(&want);
        CQuaternion__Normalize(&cur);
        CQuaternion__SlerpFast(&rot, t, &cur, &want);
        CQuaternion__ToMatrix(&rot, mtx);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        CMtxF__TransformPoint(mtx, &forward, &obj->m_vDirection);
        CVector3__Scale(&obj->m_vVelocity, &obj->m_vDirection, speed);
    }
    Missile_Accelerate(obj);
    CParticle__Wobble(obj, 1.0f);
}
