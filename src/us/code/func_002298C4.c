#include "common.h"
#include "structs/vector.h"
#include "structs/particle.h"

extern s32 LoopSounds_Get(void);
extern void *LoopSounds_Start(s32 bank, s32 sound, CVector3 *pos, CVector3 *pos2, s32 loops);  /* starts a sound */
extern void LoopSounds_Stop(s32 bank, void *handle);                                  /* stops a sound */
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

typedef struct {
    /* 0x000 */ u8 pad0[0x214];
    /* 0x214 */ CVector3 pos;
} Target;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 sound;
} SoundHandle;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x18 - 0x10];
    /* 0x018 */ CVector3 vel;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ CROMParticleEffect *parent;
    /* 0x0FC */ u8 padFC[0x114 - 0xFC];
    /* 0x114 */ Target *target;
    /* 0x118 */ u8 pad118[0x11C - 0x118];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x150 - 0x120];
    /* 0x150 */ CVector3 dir;
    /* 0x15C */ u8 pad15C[0x1B8 - 0x15C];
    /* 0x1B8 */ SoundHandle *loop;
} Missile;

/* Same as CParticle__Accelerate: accelerate along dir, clamp to the max speed. */
static __inline__ void Missile_Accelerate(Missile *obj) {
    CVector3 delta;
    CVector3 dir;
    CVector3 *vel;
    f32 accel;
    f32 speed;
    f32 maxSpeed;

    accel = HalfToFloat(obj->parent->m_pMotion->m_Accel);
    if (accel != 0.0f) {
        dir = obj->dir;
        CVector3__Scale(&delta, &dir, accel);
        vel = &obj->vel;
        CVector3__Add(vel, vel, &delta);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        maxSpeed = HalfToFloat(obj->parent->m_pMotion->m_MaxSpeed);
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
void CParticle__UpdateHomingMissile(Missile *obj) {
    CVector3 targetPos;
    CVector3 toTarget;
    CVector3 forward;
    CQuaternion cur;
    CQuaternion want;
    CQuaternion rot;
    CMtxF mtx;
    Target *target;
    f32 speed;
    f32 dist;
    f32 t;
    f32 far;
    SoundHandle *loop;
    s32 sound;

    if (obj->flags & 0x10000) {
        if (obj->loop != NULL) {
            LoopSounds_Stop(LoopSounds_Get(), obj->loop);
            obj->loop = NULL;
        }
        return;
    }
    loop = obj->loop;
    sound = 0x1B2;
    if (loop != NULL) {
        if (loop->sound == sound && loop->state != -1) {
            goto playing;
        }
        CParticle__StopLoopSound(obj);
    }
    obj->loop = LoopSounds_Start(LoopSounds_Get(), 0x1B2, &obj->pos, &obj->pos, -1);
playing:

    target = obj->target;
    if (target != NULL && obj->speed > 7.0f) {
        CQuaternion__FromDirection(&cur, &obj->dir);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        targetPos = target->pos;
        CVector3__Subtract(&toTarget, &targetPos, &obj->pos);
        dist = func_00298470(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z);
        if (dist < 102.399994f) {
            CQuaternion__FromDirection(&rot, &toTarget);
            CQuaternion__ToMatrix(&rot, mtx);
            forward.x = 0.0f;
            forward.y = 0.0f;
            forward.z = 1.0f;
            CMtxF__TransformPoint(mtx, &forward, &obj->dir);
            CVector3__Scale(&obj->vel, &obj->dir, speed);
            return;
        }
        if (dist > 153.59999f) {
            targetPos.y += 102.399994f;
            CVector3__Subtract(&toTarget, &targetPos, &obj->pos);
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
        CMtxF__TransformPoint(mtx, &forward, &obj->dir);
        CVector3__Scale(&obj->vel, &obj->dir, speed);
    }
    Missile_Accelerate(obj);
    CParticle__Wobble(obj, 1.0f);
}
