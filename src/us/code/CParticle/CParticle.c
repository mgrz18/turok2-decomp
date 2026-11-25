/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/80279208_11.CPP, func_80279B40_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"
#include "globals/scene.h"
#include "globals/collision.h"
#include "protos/game.h"

void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
/* The angle passed as its word (the float's bits in a1): the port's shim (port/game/t2abi.c,
 * ABI.md float_int), the N64 definition under its own name. */
#ifdef TARGET_PORT
void CMtxF__SetRotateZ_i(void *m, s32 angle);
#else
void CMtxF__SetRotateZ_i(void *m, s32 angle) __asm__("CMtxF__SetRotateZ");
#endif

void CMtxF__TransformPoint(void *m, CVector3 *in, CVector3 *out);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */
CMtxF *CParticle__GetParentTransform(CParticle *p, u8 *view);
void GameObject_TraceMove(void *actor, CVector3 to, CVector3 *outPos, void *outRegion, s32 flagA, s32 flagB);

static __inline__ f32 offset_x(CParticle *p) {
    if (p->m_pEffect->m_pModel != NULL) {
        return 0.0f;
    }
    return p->m_vRot.x;
}

static __inline__ f32 offset_y(CParticle *p) {
    if (p->m_pEffect->m_pModel != NULL) {
        return 0.0f;
    }
    return p->m_vRot.y;
}

void CParticle__SpawnChild(CParticle *p, s32 model, s32 arg2, s32 flags) {
    CQuaternion rotation;
    CVector3 direction;
    CVector3 offset;
    CVector3 turned;
    CVector3 carried;
    CVector3 point;
    CVector3 moved;
    CMtxF matrix;
    CCollisionResults scratch;
    void *region;
    CMtxF *parent;

    if (p->m_pEffect->m_dwFlags & 0x10) {
        direction = gCollisionResults.m_vVelocity;
    } else {
        direction = p->m_vVelocity;
    }
    CQuaternion__FromDirection(&rotation, &direction);
    if (offset_x(p) == 0.0f && offset_y(p) == 0.0f) {
        point = p->header.m_vPos;
        region = p->header.m_pCurrentRegion;
    } else {
        offset.x = -offset_x(p);
        offset.y = -offset_y(p);
        offset.z = 0.0f;
        CMtxF__SetRotateZ_i(matrix, *(s32 *)&p->m_vRot.z);
        CMtxF__TransformPoint(matrix, &offset, &turned);
        parent = CParticle__GetParentTransform(p, 0);
        if (parent != NULL) {
            CMtxF__TransformPoint(*parent, &turned, &carried);
        } else {
            carried = turned;
        }
        CVector3__Add(&moved, &carried, &p->header.m_vPos);
        gCollisionResultsP = &scratch;
        if (p->header.m_pCurrentRegion != NULL) {
            GameObject_TraceMove(p, moved, &point, &region, 0, 0);
        } else {
            point = moved;
            region = NULL;
        }
        gCollisionResultsP = &gCollisionResults;
    }
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, p, p->m_pOwner, p->m_pOwnerCount, p->m_pTarget, model, direction, rotation, point, region,
                  arg2, flags | (p->m_dwFlags & 0x200006));
}

#include "structs/vector.h"
#include "globals/engine.h"

/* Particle wobble: swings the velocity sideways (and then up/down) along
 * sine waves whose amplitude ramps up over the template's first m_Ramp frames
 * of the particle's age. */

extern f32 func_002982F0(f32);                              /* sinf */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a x b */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a - b */

void CParticle__Wobble(CParticle *p, f32 scale) {
    CVector3 up;
    CVector3 axis;
    CVector3 dir;
    CVector3 before;
    CVector3 after;
    CVector3 side;
    f32 ramp;
    f32 k;

    k = 1.0f;
    ramp = p->m_pEffect->m_pOscillation->m_Ramp;
    if (p->m_cFrame < ramp) {
        k = p->m_cFrame / ramp;
    }
    k *= scale;
    dir = p->m_vDirection;
    if (p->m_WobbleAmplitude[0] > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3__Cross(&axis, &up, &dir);
        CVector3__Scale(&before, &axis, func_002982F0(p->m_WobblePhase[0]) * p->m_WobbleAmplitude[0] * k);
        p->m_WobblePhase[0] += p->m_WobbleFreq[0] * k * gFrameIncrement;
        CVector3__Scale(&after, &axis, func_002982F0(p->m_WobblePhase[0]) * p->m_WobbleAmplitude[0] * k);
        CVector3__Subtract(&p->m_vVelocity, &p->m_vVelocity, &before);
        CVector3__Add(&p->m_vVelocity, &p->m_vVelocity, &after);
    } else {
        p->m_WobblePhase[0] += p->m_WobbleFreq[0] * k * gFrameIncrement;
    }
    if (p->m_WobbleAmplitude[1] > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3__Cross(&side, &up, &dir);
        CVector3__Cross(&axis, &side, &dir);
        CVector3__Scale(&before, &axis, func_002982F0(p->m_WobblePhase[1]) * p->m_WobbleAmplitude[1] * k);
        p->m_WobblePhase[1] += p->m_WobbleFreq[1] * k * gFrameIncrement;
        CVector3__Scale(&after, &axis, func_002982F0(p->m_WobblePhase[1]) * p->m_WobbleAmplitude[1] * k);
        CVector3__Subtract(&p->m_vVelocity, &p->m_vVelocity, &before);
        CVector3__Add(&p->m_vVelocity, &p->m_vVelocity, &after);
    } else {
        p->m_WobblePhase[1] += p->m_WobbleFreq[1] * k * gFrameIncrement;
    }
}

#include "structs/game_object.h"
#include "structs/sound.h"

void *LoopSounds_Start(void *pool, s32 id, CVector3 *pos, void *pos2, sptr loops);
void LoopSounds_Stop(void *bank, void *handle);
f32 HalfToFloat(s32 half);
extern f32 func_00298470(f32);                              /* sqrtf */
extern void CVector3__Normalize(CVector3 *v);                        /* normalize */
extern void CQuaternion__Normalize(CQuaternion *q);                        /* normalize */
void CQuaternion__SlerpFast(CQuaternion *out, f32 t, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, void *m);

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
typedef struct {
    /* 0x000 */ u8 pad0[0x1B8];
    /* 0x1B8 */ s32 loop;               /* looping sound handle */
} SoundObject;
void CParticle__StopLoopSound(SoundObject *arg0);

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

#include "structs/player.h"

extern f32 Math_AcosLookup(f32);                              /* acosf */
extern void CPlayer__TransformByView(CPlayer *player, CVector3 *pos);      /* player-relative point to world */
f32 GameObject_GetCollisionHeight(CGameObjectInstance *a);
#include "globals/particle.h"   /* sReturningHomeFrame, sReturningCatchRadius */

void CParticle__UpdateReturning(CParticle *obj) {
    CVector3 aim;
    CVector3 toHome;
    CVector3 facing;
    CVector3 forward;
    CVector3 home;
    CVector3 toCatch;
    CQuaternion cur;
    CQuaternion want;
    CQuaternion rot;
    CMtxF mtx;
    CPlayer *player;
    SndSource *loop;
    f32 speed;
    f32 dist;
    f32 t;
    f32 angle;
    s32 sound;

    if (obj->m_cFrame > sReturningHomeFrame) {
        CQuaternion__FromDirection(&cur, &obj->m_vDirection);
        speed = func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z);
        player = (CPlayer *) obj->m_pOwner->m_AI.m_pParent;
        home.x = -26.623999f;
        home.y = -20.48f;
        home.z = -143.36f;
        CPlayer__TransformByView(player, &home);
        CVector3__Subtract(&toCatch, &home, &obj->header.m_vPos);
        dist = func_00298470(toCatch.x * toCatch.x + toCatch.y * toCatch.y + toCatch.z * toCatch.z);
        if (dist <= sReturningCatchRadius + 30.72f) {
            if (dist < sReturningCatchRadius * (1.0f / 3) || obj->m_cFrame >= 65.0f) {
                obj->header.m_vPos = home;
                obj->m_nFrames = 1;
                if (player->m_Data.m_CurrentWeapon == 0x15) {
                    player->m_WeaponActor.m_AI.m_Weapon.m_BarrelSpeed = 0.73303837f;
                }
                return;
            }
            toHome = toCatch;
        } else {
            aim.x = -102.399994f;
            aim.y = GameObject_GetCollisionHeight(obj->m_pOwner) * 0.5f;
            aim.z = -sReturningCatchRadius;
            CPlayer__TransformByView(player, &aim);
            CVector3__Subtract(&toHome, &aim, &obj->header.m_vPos);
            func_00298470(toHome.x * toHome.x + toHome.y * toHome.y + toHome.z * toHome.z);
        }
        CVector3__Normalize(&toHome);
        CQuaternion__FromDirection(&want, &toHome);
        CQuaternion__Normalize(&want);
        CQuaternion__Normalize(&cur);
        CVector3__Normalize(&toHome);
        facing = obj->m_vDirection;
        CVector3__Normalize(&facing);
        angle = Math_AcosLookup(toHome.x * facing.x + toHome.y * facing.y + toHome.z * facing.z);
        t = (angle < 0.26179942f) ? 1.0f : 0.5f;
        CQuaternion__SlerpFast(&rot, t, &cur, &want);
        CQuaternion__ToMatrix(&rot, mtx);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        CMtxF__TransformPoint(mtx, &forward, &obj->m_vDirection);
        CVector3__Scale(&obj->m_vVelocity, &obj->m_vDirection, speed);
    } else {
        loop = obj->m_pLoopSound;
        sound = 0x1B3;
        if (loop != NULL) {
            if (loop->sound.w == sound && loop->handle != -1) {
                goto playing;
            }
            CParticle__StopLoopSound(obj);
        }
        obj->m_pLoopSound = LoopSounds_Start(LoopSounds_Get(), 0x1B3, &obj->header.m_vPos, &obj->header.m_vPos, -1);
    }
playing:
    Missile_Accelerate(obj);
    CParticle__Wobble(obj, 1.0f);
}

f32 GameObject_GetCollisionYOffset(CGameObjectInstance *item);
/* The quaternion facing dir, returned by value: the N64 definition's (out, dir) seen as a struct return
 * (the hidden pointer is its first argument); the port calls its by-value shim (ABI.md, struct_scalar). */
#ifdef TARGET_PORT
extern CQuaternion CQuaternion__FromDirection_byval(CVector3 *dir);
#define CQuaternion__FromDirection_r CQuaternion__FromDirection_byval
#else
extern CQuaternion CQuaternion__FromDirection_r(CVector3 *dir) __asm__("CQuaternion__FromDirection");
#endif

/* Speeds up along the heading, stopping at the definition's limit. */
static __inline__ void accelerate(CParticle *p, f32 accel) {
    CVector3 step;
    CVector3 dir;
    CVector3 *vel;
    f32 speed;
    f32 limit;

    dir = p->m_vDirection;
    CVector3__Scale(&step, &dir, accel);
    vel = &p->m_vVelocity;
    CVector3__Add(vel, vel, &step);
    speed = func_00298470(p->m_vVelocity.x * p->m_vVelocity.x + p->m_vVelocity.y * p->m_vVelocity.y + p->m_vVelocity.z * p->m_vVelocity.z);
    limit = HalfToFloat(p->m_pEffect->m_pMotion->m_MaxSpeed);
    if (accel > 0.0f) {
        if (limit < speed) {
            CVector3__Scale(vel, &dir, limit);
        }
    } else {
        if (speed < limit) {
            CVector3__Scale(vel, &dir, limit);
        }
    }
}

/* Homing projectile: turns toward the middle of its target, harder the
 * closer it is, then accelerates along its heading up to (or down to) the
 * definition's speed limit, then wobbles. */
void CParticle__UpdateHoming(CParticle *p) {
    CVector3 aim;
    CVector3 delta;
    CVector3 forward;
    CQuaternion from;
    CQuaternion to;
    CQuaternion turn;
    CMtxF matrix;
    struct CGameObjectInstance *target;
    f32 dist;
    f32 speed;
    f32 amount;
    f32 accel;

    target = p->m_pTarget;
    if (target != NULL && p->m_cFrame > 1.0f) {
        f32 height = GameObject_GetCollisionYOffset(target);

        aim = target->header.m_vPos;
        aim.y += height;
        aim.y += GameObject_GetCollisionHeight(target) * 0.5f;
        CVector3__Subtract(&delta, &aim, &p->header.m_vPos);
        dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
        if (dist <= 51.199997f) {
            p->header.m_vPos = target->header.m_vPos;
            return;
        }
        CVector3__Normalize(&delta);
        to = CQuaternion__FromDirection_r(&delta);
        from = CQuaternion__FromDirection_r(&p->m_vDirection);
        speed = func_00298470(p->m_vVelocity.x * p->m_vVelocity.x + p->m_vVelocity.y * p->m_vVelocity.y + p->m_vVelocity.z * p->m_vVelocity.z);
        if (dist < 1024.0f) {
            amount = dist - 204.79999f;
            if (amount < 0.0f) {
                amount = 0.0f;
            } else {
                amount *= 0.0012207031f;
            }
            amount = 0.9f - amount * 0.85999995f;
        } else {
            amount = 0.04f;
        }
        CQuaternion__SlerpFast(&turn, amount, &from, &to);
        CQuaternion__ToMatrix(&turn, &matrix);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        CMtxF__TransformPoint(&matrix, &forward, &p->m_vDirection);
        CVector3__Scale(&p->m_vVelocity, &p->m_vDirection, speed);
    }
    accel = HalfToFloat(p->m_pEffect->m_pMotion->m_Accel);
    if (accel != 0.0f) {
        accelerate(p, accel);
    }
    CParticle__Wobble(p, 1.0f);
}

#include "globals/game.h"

typedef struct ProjLimits {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 thrust;                 /* high half of an f32 */
    /* 0x12 */ u16 maxSpeed;               /* high half of an f32 */
} ProjLimits;
#ifdef TARGET_PORT
typedef struct ProjDef {
/* the view at the host offsets of CROMParticleEffect (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ProjDef(m) __builtin_offsetof(CROMParticleEffect, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_ProjDef(m_dwFlags)]; u32 flags; }; /* N64 0x0 */
        struct { u8 _hp_turnRate[T2_HOFF_ProjDef(m_TurnRate)]; s16 turnRate; }; /* N64 0xE */
        struct { u8 _hp_limits[T2_HOFF_ProjDef(m_pMotion)]; ProjLimits *limits; }; /* N64 0x30 */
    };
} ProjDef;
#else
typedef struct ProjDef {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0xA];
    /* 0x0E */ s16 turnRate;               /* degrees */
    /* 0x10 */ u8 pad10[0x20];
    /* 0x30 */ ProjLimits *limits;
} ProjDef;
#endif
#ifdef TARGET_PORT
typedef struct Projectile {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Projectile(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_type[T2_HOFF_Projectile(header.m_nObjType)]; u16 type; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_Projectile(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_velocity[T2_HOFF_Projectile(m_vVelocity)]; CVector3 velocity; }; /* N64 0x18 */
        struct { u8 _hp_flags[T2_HOFF_Projectile(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_def[T2_HOFF_Projectile(m_pEffect)]; ProjDef *def; }; /* N64 0xF8 */
        struct { u8 _hp_target[T2_HOFF_Projectile(m_pTarget)]; CPlayer *target; }; /* N64 0x114 */
        struct { u8 _hp_heading[T2_HOFF_Projectile(m_vDirection)]; CVector3 heading; }; /* N64 0x150 */
        struct { u8 _hp_sound[T2_HOFF_Projectile(m_pLoopSound)]; SndSource *sound; }; /* N64 0x1B8 */
    };
} Projectile;
#else
typedef struct Projectile {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 type;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[8];
    /* 0x018 */ CVector3 velocity;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ProjDef *def;
    /* 0x0FC */ u8 padFC[0x114 - 0xFC];
    /* 0x114 */ CPlayer *target;
    /* 0x118 */ u8 pad118[0x150 - 0x118];
    /* 0x150 */ CVector3 heading;
    /* 0x15C */ u8 pad15C[0x1B8 - 0x15C];
    /* 0x1B8 */ SndSource *sound;
} Projectile;
#endif

extern f32 Random_Blend(f32 lo, f32 hi);   /* random in range */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CVector3__LimitLength(CVector3 *v, f32 max);

/* CParticle__StartLoopSound, inlined: (re)start the projectile's looping sound unless it is already playing. */
static __inline__ void PlayLoopingSound(Projectile *o, s32 id) {
    if (o->sound != NULL) {
        if (o->sound->sound.w == id && o->sound->handle != -1) {
            return;
        }
        CParticle__StopLoopSound(o);
    }
    o->sound = LoopSounds_Start(LoopSounds_Get(), id, &o->pos, &o->pos, -1);
}

static __inline__ void Steer(Projectile *o, f32 a) {
    CVector3 push;
    CVector3 vel;
    f32 len, lim;

    if (a != 0.0f) {
        vel = o->heading;
        CVector3__Scale(&push, &vel, a);
        CVector3__Add(&o->velocity, &o->velocity, &push);
        len = func_00298470(o->velocity.x * o->velocity.x + o->velocity.y * o->velocity.y
                            + o->velocity.z * o->velocity.z);
        lim = HalfToFloat(o->def->limits->maxSpeed);
        if (a > 0.0f) {
            if (lim < len) CVector3__Scale(&o->velocity, &vel, lim);
        } else {
            if (len < lim) CVector3__Scale(&o->velocity, &vel, lim);
        }
    }
}

void CParticle__Steer(Projectile *o) {
    CVector3 d;
    CVector3 tp;
    CVector3 vv;
    CVector3 dir;
    CVector3 ax;
    CQuaternion q0, q1, q2;
    CMtxF mt;
    CPlayer *p;
    CPlayer *p2;
    CVector3 *tpp;
    CVector3 *velp;
    CPlayer *t;
    ProjDef *def;
    f32 h, fa, fb, fc, fd, k, speed, ang, w, tt;

    switch (o->type) {
        case 0x2E:
            p2 = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
            if (p2 != NULL) {
                tpp = &tp;
                velp = &o->velocity;
                do { fa = 0.5f; } while (0);
                fb = 2.0f;
                p = p2;
                do {
                    h = p->m_CharacterActor.m_CollisionYOffset;
                    tp = p->m_CharacterActor.header.m_vPos;
                    tp.y += h;
                    tp.y += GameObject_GetCollisionHeight((CGameObjectInstance *) p2) * fa;
                    CVector3__Subtract(&d, tpp, &o->pos);
                    k = func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
                    if (k < 512.0f) {
                        k = 1.0f - k * (1.0f / 512);
                        d.x *= k * Random_Blend(fa, fb);
                        d.y *= k * Random_Blend(fa, fb);
                        d.z *= k * Random_Blend(fa, fb);
                        CVector3__Subtract(velp, velp, &d);
                    }
                    p2 = p->m_pNext;
                    p = p2;
                } while (p2 != NULL);
            }
            t = o->target;
            tt = GameObject_GetCollisionYOffset((CGameObjectInstance *) t);
            tp = t->m_CharacterActor.header.m_vPos;
            tp.y += tt;
            CVector3__Subtract(&d, &tp, &o->pos);
            fc = 0.3f;
            fd = 0.7f;
            d.x *= Random_Blend(fc, fd);
            d.y *= Random_Blend(fc, fd);
            d.z *= Random_Blend(fc, fd);
            CVector3__Add(&o->velocity, &o->velocity, &d);
            CVector3__LimitLength(&o->velocity, HalfToFloat(o->def->limits->maxSpeed));
            return;
        case 0xE:
            CParticle__UpdateHoming(o);
            return;
        case 0xB:
        case 0x129:
            CParticle__UpdateHomingMissile(o);
            return;
        case 0x68:
            CParticle__UpdateReturning(o);
            return;
        case 0x5F:
        case 0x60:
            PlayLoopingSound(o, 0x228);
            break;
    }

    t = o->target;
    if (t != NULL) {
        def = o->def;
        if (def->flags & 0x40000) {
            if (def->turnRate != 0) {
                tt = GameObject_GetCollisionYOffset((CGameObjectInstance *) t);
                tp = t->m_CharacterActor.header.m_vPos;
                tp.y += tt;
                tp.y += GameObject_GetCollisionHeight((CGameObjectInstance *) t) * 0.5f;
                CVector3__Subtract(&dir, &tp, &o->pos);
                CVector3__Normalize(&dir);
                CQuaternion__FromDirection(&q1, &dir);
                CQuaternion__FromDirection(&q0, &o->heading);
                vv = o->heading;
                CVector3__Normalize(&vv);
                speed = func_00298470(o->velocity.x * o->velocity.x + o->velocity.y * o->velocity.y
                                      + o->velocity.z * o->velocity.z);
                ang = Math_AcosLookup(vv.x * dir.x + vv.y * dir.y + vv.z * dir.z);
                tt = (f32)o->def->turnRate * 0.017453294f;
                w = 1.0f;
                if (tt < ang) {
                    w = tt / ang;
                }
                CQuaternion__SlerpFast(&q2, w, &q0, &q1);
                CQuaternion__ToMatrix(&q2, &mt);
                *(s32 *)&ax.x = 0;
                *(s32 *)&ax.y = 0;
                ax.z = 1.0f;
                CMtxF__TransformPoint(&mt, &ax, &o->heading);
                CVector3__Scale(&o->velocity, &o->heading, speed);
            }
        }
    }
    if (!(o->flags & 0x70000)) {
        Steer(o, HalfToFloat(o->def->limits->thrust));
    }
    if (!(o->flags & 0x30000)) {
        CParticle__Wobble(o, 1.0f);
    }
}

void CGameRegion__FireTriggers(sptr state, s32 bits, CParticle *p);

static __inline__ s32 isGroupA(CParticle *p) {
    switch (p->header.m_nObjType) {
        case 2:
        case 0x56:
        case 0x111:
        case 0x126:
            return 1;
    }
    return 0;
}

static __inline__ s32 isGroupB(CParticle *p) {
    switch (p->header.m_nObjType) {
        case 5:
        case 6:
        case 7:
        case 9:
        case 0x11:
        case 0x13:
        case 0x1A:
        case 0x101:
        case 0x110:
            return 1;
    }
    return 0;
}

static __inline__ s32 isGroupC(CParticle *p) {
    switch (p->header.m_nObjType) {
        case 8:
        case 0xD:
        case 0x12:
            return 1;
    }
    return 0;
}

/* Reports a particle to the two collision states by the type group it belongs to: with
 * one set of bits when its owner is a kind 1 object flagged 0x300000, otherwise with
 * another when its descriptor is of kind 1 or 4. */
void CParticle__ReportCollision(CParticle *p) {
    if (gCollisionResults.m_pWallRegion != 0) {
        CGameObjectInstance *owner = p->m_pOwner;

        if (owner != NULL && owner->header.m_Type == 1 && (owner->m_dwFlags & 0x300000)) {
            if (isGroupA(p)) {
                CGameRegion__FireTriggers((sptr)gCollisionResults.m_pWallRegion, 0x10, p);
            }
            if (isGroupB(p)) {
                CGameRegion__FireTriggers((sptr)gCollisionResults.m_pWallRegion, 8, p);
            }
            if (isGroupC(p)) {
                CGameRegion__FireTriggers((sptr)gCollisionResults.m_pWallRegion, 0x20, p);
            }
        } else if (p->header.m_pIntelligence->m_Type == 1 || p->header.m_pIntelligence->m_Type == 4) {
            if (isGroupC(p)) {
                CGameRegion__FireTriggers((sptr)gCollisionResults.m_pWallRegion, 0x200, p);
            }
        }
    }
    if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 1) {
        CGameObjectInstance *owner = p->m_pOwner;

        if (owner != NULL && owner->header.m_Type == 1 && (owner->m_dwFlags & 0x300000)) {
            if (isGroupA(p)) {
                GameObject_FireTriggers(gCollisionResults.m_pInstance, 8, p);
            }
            if (isGroupB(p)) {
                GameObject_FireTriggers(gCollisionResults.m_pInstance, 4, p);
            }
            if (isGroupC(p)) {
                GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x10, p);
            }
        } else if (p->header.m_pIntelligence->m_Type == 1 || p->header.m_pIntelligence->m_Type == 4) {
            if (isGroupA(p)) {
                GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x80, p);
            }
            if (isGroupB(p)) {
                GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x40, p);
            }
            if (isGroupC(p)) {
                GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x100, p);
            }
        }
    }
}

#include "globals/pickup.h"

s32 Random_Next(void);
s32 Collision_MoveInstance(void *obj, CVector3 to, void *info);
f32 GameObject_GetCollisionRadius(CPlayer *player);
f32 func_00298470(f32 x);
void CParticle__FireExpireEvent(CParticle *o);
void CParticle__FireUpdateEvent(CParticle *o);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Normalize(CVector3 *v);
void CParticle__HitWater(CParticle *arg0);
void CParticle__HitWall(CParticle *arg0);
void CParticle__HitInstanceBody(CParticle *p);
void CParticle__HitTarget(CParticle *p);
void CParticle__HitFloor(CParticle *arg0);
void CParticle__HitCeiling(CParticle *arg0);
void CParticle__SpawnAlongPath(CParticle *actor);
#define MIN(a, b) ((a) > (b) ? (b) : (a))
/* Engine actor instance (only the fields used here). */
#ifdef TARGET_PORT
typedef struct EffectActor {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_EffectActor(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_kind[T2_HOFF_EffectActor(header.m_Type)]; u8 kind; }; /* N64 0x0 */
        struct { u8 _hp_typeId[T2_HOFF_EffectActor(header.m_nObjType)]; u16 typeId; }; /* N64 0x2 */
        struct { u8 _hp_flags[T2_HOFF_EffectActor(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_child[T2_HOFF_EffectActor(m_pSource)]; struct EffectActor *child; }; /* N64 0x108 */
        struct { u8 _hp_parent[T2_HOFF_EffectActor(m_pOwner)]; u8 *parent; }; /* N64 0x10C */
        struct { u8 _hp_liveCount[T2_HOFF_EffectActor(m_pOwnerCount)]; s32 *liveCount; }; /* N64 0x110 */
        struct { u8 _hp_sound[T2_HOFF_EffectActor(m_pDLight)]; s32 sound; }; /* N64 0x118 */
        struct { u8 _hp_registered[T2_HOFF_EffectActor(m_ActiveSwooshes)]; u8 registered; }; /* N64 0x1B5 */
        struct { u8 _hp_list[T2_HOFF_EffectActor(m_pActiveList)]; void *list; }; /* N64 0x1C0 */
    };
} EffectActor;
#else
typedef struct EffectActor {
    /* 0x000 */ u8 kind;
    u8 pad1;
    /* 0x002 */ u16 typeId;
    u8 pad4[0x3C];
    /* 0x040 */ u32 flags;
    u8 pad44[0xC4];
    /* 0x108 */ struct EffectActor *child;
    /* 0x10C */ u8 *parent;
    /* 0x110 */ s32 *liveCount;
    u8 pad114[0x4];
    /* 0x118 */ s32 sound;
    u8 pad11C[0x99];
    /* 0x1B5 */ u8 registered;
    u8 pad1B6[0xA];
    /* 0x1C0 */ void *list;
} EffectActor;
#endif
void CParticleSystem__Free(u8 *mgr, EffectActor *actor);

/* Updates a particle for one frame: measures its bounding box, kills it or bounces it off its
 * target when its type's rule says so, lets the players it touches react to it, advances its
 * size and spin (dying when a size goes negative) and, when it moves or falls, probes the
 * ground, orients itself to the surface it hit and runs the collision responses its
 * descriptor allows. */
void CParticle__Update(CParticle *p) {
    CBox bounds;
    CGameObjectInstance *target;
    CPlayer *player;
    CPickupRow14 *handler;
    f32 extent;
    f32 scaled;
    f32 half;
    f32 top;
    f32 dist;
    f32 step;
    f32 grow;
    s32 die;
    s32 bounce;
    s32 i;
    s8 fade;

    if (p->m_cFrame < 0.0f) {
        return;
    }
    extent = p->m_vSize.z;
    scaled = p->m_vSize.y * extent;
    if (!(p->m_vSize.x * extent <= scaled)) {
        /* FAKEMATCH: an always-true test (folded only after CSE) keeps CSE from carrying the
         * 0.0f of the time check into the code below, as in the ROM. Natural shapes of the
         * maximum (if/else, ternary, MAX macros, inline helpers with one or two returns) all
         * carry it into `p->m_cFrame = 0.0f` (docs/MATCHING.md, k4 pass). */
        if (p != NULL) {
            scaled = p->m_vSize.x * extent;
        } else {
            scaled = p->m_vSize.x * extent;
        }
    }
    if (!(scaled <= extent)) {
        extent = scaled;
    }
    scaled = extent * 1.4142135f;
    half = scaled * 0.5f;
    bounds.m_vMin.x = p->header.m_vPos.x - half;
    bounds.m_vMax.x = p->header.m_vPos.x + half;
    bounds.m_vMin.y = p->header.m_vPos.y - half;
    bounds.m_vMax.y = p->header.m_vPos.y + scaled;
    bounds.m_vMin.z = p->header.m_vPos.z - half;
    bounds.m_vMax.z = p->header.m_vPos.z + half;
    if (p->m_dwFlags & 0x20000) {
        p->m_cFrame = 0.0f;
        p->m_nFrames = p->m_AnimSpeed * 32.0f;
        half = p->m_Shake - gFrameIncrement * 6.0f;
        fade = (half < 0.0f) ? 0 : (s32) half;
        p->m_Shake = fade;
    }
    if (p->m_dwFlags & 0x10000) {
        target = p->m_pTarget;
        switch (p->header.m_nObjType) {
            default:
                p->m_cFrame = 0.0f;
                p->m_nFrames = p->m_AnimSpeed * 32.0f;
            case 0x2D:
            case 0x4F:
                die = 0;
                break;
            case 0xB:
                die = 0;
                break;
        }
        bounce = die;
        switch (p->header.m_nObjType) {
            case 0x12A:
            case 0x132:
                die = 0;
                if (target->m_AI.m_Health == 0) {
                    bounce = 1;
                }
                break;
            case 2:
            case 0xF:
            case 0x56:
            case 0x126:
                if (p->m_dwFlags & 0x4000000) {
                    die = 1;
                } else if (!(target->m_dwFlags & 0x100)) {
                    die = 1;
                } else if (target->m_dwFlags & 0x300000) {
                    die = (f32) target->m_AI.m_Health <= 0.0f;
                } else {
                    die = target->m_AI.m_dwFlags & 0x20;
                }
                bounce = 0;
                break;
            case 0xB:
                die = 0;
                if (target->m_AI.m_Mode != 0x21 || !(target->m_dwFlags & 0x100)) {
                    bounce = 1;
                }
                break;
            case 4:
                die = 0;
                if (target->m_AI.m_Mode != 0x20 || !(target->m_dwFlags & 0x100)) {
                    bounce = 1;
                }
                break;
            case 0x2D:
            case 0x4F:
                bounce = !(target->m_dwFlags & 0x100);
                break;
        }
        if (die || bounce) {
            p->m_cFrame = 0.0f;
            p->m_Alignment = -7;
            p->m_dwFlags = (p->m_dwFlags & ~0x30000) | 0x40000;
            p->m_vRot.z = (f32) (Random_Next() % 360) * 0.017453294f;
            p->m_vVelocity.y = 0.0f;
            if (p->m_dwFlags & 0x4000000) {
                p->m_vVelocity.x = -p->m_vDirection.x;
                p->m_vVelocity.z = -p->m_vDirection.z;
            } else {
                p->m_vVelocity.x = 0.0f;
                p->m_vVelocity.z = 0.0f;
            }
            p->header.m_pCurrentRegion = target->header.m_pCurrentRegion;
            p->m_dwHitFlags &= ~0x1000000;
            if (bounce) {
                p->m_nFrames = 1;
            }
        }
    }
    if ((p->m_dwFlags & 0x40000) && Collision_MoveInstance(p, p->header.m_vPos, &p->m_dwHitFlags) && gCollisionResults.m_pFloorRegion != 0) {
        p->m_Alignment = -3;
    }
    if (p->m_dwFlags & 0x60000) {
        for (player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; player != NULL; player = player->m_pNext) {
            if (player->m_Data.m_Health != 0 && player->m_Mode != 2) {
                top = player->m_CharacterActor.header.m_vPos.y + player->m_CharacterActor.m_CollisionYOffset;
                if (top - 20.479999f <= p->header.m_vPos.y && p->header.m_vPos.y <= top + GameObject_GetCollisionHeight((CGameObjectInstance *) player) + 20.479999f) {
                    dist = func_00298470((player->m_CharacterActor.header.m_vPos.x - p->header.m_vPos.x) * (player->m_CharacterActor.header.m_vPos.x - p->header.m_vPos.x)
                                         + (player->m_CharacterActor.header.m_vPos.z - p->header.m_vPos.z) * (player->m_CharacterActor.header.m_vPos.z - p->header.m_vPos.z));
                    if (dist < GameObject_GetCollisionRadius(player) + 51.199997f) {
                        handler = sParticleAmmoPickups;
                        for (i = 3; i != -1; handler++, i--) {
                            if (handler->m_Id == p->header.m_nObjType && handler->m_pHandler(player, handler)) {
                                p->m_dwFlags |= 0x1000;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    p->m_vLastPos = p->header.m_vPos;
    p->m_AnimFrame += gFrameIncrement * p->m_AnimSpeed * (gParticleTimeScale * 0.06666667f);
    if (p->m_pEffect->m_dwFlags & 0x1000) {
        step = gFrameIncrement * 10.24f;
        p->m_vSize.x += step * p->m_vGrowth.x;
        p->m_vSize.y += step * p->m_vGrowth.y;
        p->m_vSize.z += gFrameIncrement * p->m_vGrowth.z;
        p->m_vSize.x = (p->m_vSize.x > 4096.0f) ? 4096.0f : p->m_vSize.x;
        p->m_vSize.y = (p->m_vSize.y > 4096.0f) ? 4096.0f : p->m_vSize.y;
        p->m_vSize.z = (p->m_vSize.z > 400.0f) ? 400.0f : p->m_vSize.z;
    } else {
        grow = MIN(p->m_vSize.x * p->m_vGrowth.x, 4096.0f);
        p->m_vSize.x += (grow - p->m_vSize.x) * gFrameIncrement;
        grow = MIN(p->m_vSize.y * p->m_vGrowth.y, 4096.0f);
        p->m_vSize.y += (grow - p->m_vSize.y) * gFrameIncrement;
        grow = MIN(p->m_vSize.z * p->m_vGrowth.z, 400.0f);
        p->m_vSize.z += (grow - p->m_vSize.z) * gFrameIncrement;
        if (p->m_vSize.x < 0.0f || p->m_vSize.y < 0.0f || p->m_vSize.z < 0.0f) {
            CParticle__FireExpireEvent(p);
            CParticleSystem__Free(&gScene.m_ParticleSystem, p);
            CParticle__StopLoopSound(p);
            return;
        }
    }
    p->m_vRot.x += p->m_vRotSpeed.x * gFrameIncrement;
    p->m_vRot.y += p->m_vRotSpeed.y * gFrameIncrement;
    p->m_vRot.z += p->m_vRotSpeed.z * gFrameIncrement;
    gCollisionResultsP->m_vVelocity = p->m_vVelocity;
    CParticle__FireUpdateEvent(p);
    CParticle__Steer(p);
    if ((p->m_Gravity == 0.0f || (p->header.m_vPos.y == 0.0f && p->m_Gravity < 0.0f))
        && p->m_vVelocity.x * p->m_vVelocity.x + p->m_vVelocity.y * p->m_vVelocity.y + p->m_vVelocity.z * p->m_vVelocity.z < 0.001f) {
        return;
    }
    if (!(p->m_dwFlags & 0x72000)) {
        die = Collision_MoveInstance(p, p->header.m_vPos, &p->m_dwHitFlags);
        if (p->header.m_nObjType == 0x68 && gCollisionResults.m_pInstance != NULL) {
            p->m_pTarget = (CGameObjectInstance *)gCollisionResults.m_pInstance;
        }
        if (die) {
            if (gCollisionResults.m_pInstance != NULL) {
                CVector3__Subtract(&p->m_vDirection, &p->header.m_vPos, &gCollisionResults.m_vInstancePos);
            } else if (gCollisionResults.m_pShape != 0) {
                CVector3__Subtract(&p->m_vDirection, &p->header.m_vPos, &gCollisionResults.m_vShapePos);
            } else if (gCollisionResults.m_pWallRegion != 0) {
                CVector3__Subtract(&p->m_vDirection, &p->header.m_vPos, &gCollisionResults.m_vWallPos);
            } else if (gCollisionResults.m_pFloorRegion != 0) {
                CVector3__Subtract(&p->m_vDirection, &p->header.m_vPos, &gCollisionResults.m_vFloorPos);
                if (p->m_Alignment == -7) {
                    p->m_Alignment = -3;
                }
            } else if (gCollisionResults.m_pWaterRegion != 0) {
                CVector3__Subtract(&p->m_vDirection, &p->header.m_vPos, &gCollisionResults.m_vWaterPos);
            }
            CVector3__Normalize(&p->m_vDirection);
        }
        CParticle__ReportCollision(p);
        if (gCollisionResults.m_pWaterRegion != 0) {
            CParticle__HitWater(p);
        }
        if (p->m_dwFlags & 0x100) {
            if (gCollisionResults.m_pWallRegion != 0) {
                if (!(p->m_pEffect->m_dwFlags & 0x10000000)) {
                    CParticle__HitWall(p);
                }
                if (p->m_pEffect->m_dwFlags & 0x800) {
                    p->m_bDieOnImpact = 1;
                }
            }
            if (p->m_dwFlags & 0x100) {
                if (gCollisionResults.m_pInstance != NULL) {
                    if ((p->m_dwHitFlags & 0x1000000) && gCollisionResults.m_pInstance->header.m_Type == 0) {
                        CParticle__HitInstanceBody(p);
                    } else {
                        CParticle__HitTarget(p);
                    }
                }
                if ((p->m_dwFlags & 0x100) && gCollisionResults.m_nFloorKind != 0 && !(p->m_pEffect->m_dwFlags & 0x20000000)) {
                    if (gCollisionResults.m_nFloorKind == 1) {
                        CParticle__HitFloor(p);
                    } else {
                        CParticle__HitCeiling(p);
                    }
                }
            }
        }
    }
    if ((p->m_dwFlags & 0x8100) == 0x100) {
        CParticle__SpawnAlongPath(p);
    }
}

#ifdef TARGET_PORT
extern u8 CGameRegion__GetWallSurface(void *region); /* ABI: it returns u8 (as s32 the host read garbage upper bits) */
#else
extern s32 CGameRegion__GetWallSurface(void *region);                     /* region's surface type */
#endif

void ImpactEvent_Send(void *src, void *dst, s32 effect, s32 id, CVector3 pos, CEventValue params);
extern void Sound__Play(s16 sound, CVector3 pos, sptr, sptr);

void CParticle__HitWall(CParticle *arg0) {
    CEventValue pair;
    CQuaternion rotation;
    CVector3 position;
    s32 surface;
    s32 index;
    CROMParticleImpact *events;
    s32 particle;
    s32 effect;
    s32 sound;
    CROMParticleEffect *desc;

    if ((arg0->m_dwFlags & 0x400) != 0) {
        if ((arg0->m_pEffect->m_dwFlags & 0x800) != 0) {
            goto skip_effects;
        }
    }
    {
        surface = CGameRegion__GetWallSurface(arg0->header.m_pCurrentRegion);
        if (surface == 10) {
            surface = 0;
        }
        desc = arg0->m_pEffect;
        index = surface;
        events = desc->m_pImpact;
        particle = events->m_ImpactParticleType[index];
        effect = events->m_ImpactEventType[index];
        pair = events->m_ImpactEventValue[surface];
        sound = desc->m_pImpact->m_ImpactSoundType[index];
        if (effect != 0xFFFF) {
            ImpactEvent_Send(arg0, arg0, effect, -1, gCollisionResults_m_vWallPos, pair);
        }
        if (particle != 0xFFFF) {
            if ((arg0->m_pEffect->m_dwFlags & 0x10) != 0) {
                position = gCollisionResults_m_vVelocity;
            } else {
                position = arg0->m_vVelocity;
            }
            CQuaternion__FromDirection(&rotation, &position);
            CParticleSystem__Spawn(&gScene.m_ParticleSystem, arg0, arg0->m_pOwner, arg0->m_pOwnerCount, arg0->m_pTarget, particle, position,
                          rotation, gCollisionResults_m_vWallPos, gCollisionResults_m_pWallRegion, gCollisionResults_m_nWallEdge, (arg0->m_dwFlags & 0x200006) | 1);
        }
        if (sound != 0xFFFF) {
            Sound__Play((s16)sound, gCollisionResults_m_vWallPos, 0, -1);
        }
    }
skip_effects:
    CVector3__Scale(&arg0->m_vRotSpeed, &arg0->m_vRotSpeed, arg0->m_RotationScale);
    arg0->m_dwFlags |= 0x400;
    if (arg0->m_bDieOnImpact == 1) {
        CParticleSystem__Free(&gScene.m_ParticleSystem, arg0);
        CParticle__StopLoopSound(arg0);
    }
}

void CParticle__HitWater(CParticle *arg0) {
    CEventValue pair;
    CQuaternion rotation;
    CVector3 position;
    s32 surface;
    s32 index;
    CROMParticleImpact *events;
    s32 particle;
    s32 effect;
    s32 sound;
    CROMParticleEffect *desc;

    switch (gCollisionResults_m_nWaterKind) {
    case 1:
    default:
        surface = 1;
        break;
    case 7:
        surface = 7;
        break;
    case 8:
        surface = 8;
        break;
    }
    arg0->m_dwFlags ^= 4;
    desc = arg0->m_pEffect;
    index = surface;
    events = desc->m_pImpact;
    particle = events->m_ImpactParticleType[index];
    effect = events->m_ImpactEventType[index];
    pair = events->m_ImpactEventValue[surface];
    sound = desc->m_pImpact->m_ImpactSoundType[index];
    if (effect != 0xFFFF) {
        ImpactEvent_Send(arg0, arg0, effect, -1, gCollisionResults_m_vWaterPos, pair);
    }
    if (particle != 0xFFFF) {
        if ((arg0->m_pEffect->m_dwFlags & 0x10) != 0) {
            position = gCollisionResults_m_vVelocity;
        } else {
            position = arg0->m_vVelocity;
        }
        CQuaternion__FromDirection(&rotation, &position);
        CParticleSystem__Spawn(&gScene.m_ParticleSystem, arg0, arg0->m_pOwner, arg0->m_pOwnerCount, arg0->m_pTarget, particle, position,
                      rotation, gCollisionResults_m_vWaterPos, (arg0->m_dwHitFlags & 0x1000000) ? 0 : gCollisionResults_m_pWaterRegion, -5, (arg0->m_dwFlags & 0x200006) | 1);
    }
    if (sound != 0xFFFF) {
        Sound__Play((s16)sound, gCollisionResults_m_vWaterPos, 0, -1);
    }
    arg0->m_dwFlags |= 0x200;
    if ((arg0->m_pEffect->m_dwFlags & 0x20000) != 0) {
        CParticleSystem__Free(&gScene.m_ParticleSystem, arg0);
        CParticle__StopLoopSound(arg0);
    }
}

extern s32 CGameRegion__IsSteep(void *region);
#ifdef TARGET_PORT
/* ABI: both return u8 (func_0026C114.c, func_0026C0E0.c): as s32 the host read eax's garbage upper bits */
extern u8 CGameRegion__GetWallSurface(void *region);
extern u8 CGameRegion__GetFloorSurface(void *region);
#else
extern s32 CGameRegion__GetWallSurface(void *region);
extern s32 CGameRegion__GetFloorSurface(void *region);
#endif

void CParticle__HitFloor(CParticle *arg0) {
    CEventValue pair;
    CQuaternion rotation;
    CVector3 position;
    s32 surface;
    s32 index;
    CROMParticleImpact *events;
    s32 particle;
    s32 effect;
    s32 sound;
    CROMParticleEffect *desc;

    if (CGameRegion__IsSteep(arg0->header.m_pCurrentRegion) != 0) {
        surface = CGameRegion__GetWallSurface(arg0->header.m_pCurrentRegion);
    } else {
        surface = CGameRegion__GetFloorSurface(arg0->header.m_pCurrentRegion);
    }
    if (surface == 10) {
        surface = 0;
    }
    desc = arg0->m_pEffect;
    index = surface;
    events = desc->m_pImpact;
    particle = events->m_ImpactParticleType[index];
    effect = events->m_ImpactEventType[index];
    pair = events->m_ImpactEventValue[surface];
    sound = desc->m_pImpact->m_ImpactSoundType[index];
    if (effect != 0xFFFF) {
        ImpactEvent_Send(arg0, arg0, effect, -1, gCollisionResults_m_vFloorPos, pair);
    }
    if (particle != 0xFFFF) {
        if ((arg0->m_pEffect->m_dwFlags & 0x10) != 0) {
            position = gCollisionResults_m_vVelocity;
        } else {
            position = arg0->m_vVelocity;
        }
        CQuaternion__FromDirection(&rotation, &position);
        CParticleSystem__Spawn(&gScene.m_ParticleSystem, arg0, arg0->m_pOwner, arg0->m_pOwnerCount, arg0->m_pTarget, particle, position,
                      rotation, gCollisionResults_m_vFloorPos, gCollisionResults_m_pFloorRegion, -3, (arg0->m_dwFlags & 0x200006) | 1);
    }
    if (sound != 0xFFFF) {
        Sound__Play((s16)sound, gCollisionResults_m_vFloorPos, 0, -1);
    }
    CVector3__Scale(&arg0->m_vRotSpeed, &arg0->m_vRotSpeed, arg0->m_RotationScale);
    arg0->m_dwFlags |= 0x200;
    if (arg0->m_bDieOnBounce == 1) {
        CParticleSystem__Free(&gScene.m_ParticleSystem, arg0);
        CParticle__StopLoopSound(arg0);
    }
}

void CParticle__HitCeiling(CParticle *arg0) {
    CEventValue pair;
    CQuaternion rotation;
    CVector3 position;
    s32 surface;
    s32 index;
    CROMParticleImpact *events;
    s32 particle;
    s32 effect;
    s32 sound;
    CROMParticleEffect *desc;

    surface = CGameRegion__GetWallSurface(arg0->header.m_pCurrentRegion);
    if (surface == 10) {
        surface = 0;
    }
    desc = arg0->m_pEffect;
    index = surface;
    events = desc->m_pImpact;
    particle = events->m_ImpactParticleType[index];
    effect = events->m_ImpactEventType[index];
    pair = events->m_ImpactEventValue[surface];
    sound = desc->m_pImpact->m_ImpactSoundType[index];
    if (effect != 0xFFFF) {
        ImpactEvent_Send(arg0, arg0, effect, -1, gCollisionResults_m_vFloorPos, pair);
    }
    if (particle != 0xFFFF) {
        if ((arg0->m_pEffect->m_dwFlags & 0x10) != 0) {
            position = gCollisionResults_m_vVelocity;
        } else {
            position = arg0->m_vVelocity;
        }
        CQuaternion__FromDirection(&rotation, &position);
        CParticleSystem__Spawn(&gScene.m_ParticleSystem, arg0, arg0->m_pOwner, arg0->m_pOwnerCount, arg0->m_pTarget, particle, position,
                      rotation, gCollisionResults_m_vFloorPos, gCollisionResults_m_pFloorRegion, -2, (arg0->m_dwFlags & 0x200006) | 1);
    }
    if (sound != 0xFFFF) {
        Sound__Play((s16)sound, gCollisionResults_m_vFloorPos, 0, -1);
    }
    CVector3__Scale(&arg0->m_vRotSpeed, &arg0->m_vRotSpeed, arg0->m_RotationScale);
    arg0->m_dwFlags |= 0x200;
    if (arg0->m_bDieOnBounce == 1) {
        CParticleSystem__Free(&gScene.m_ParticleSystem, arg0);
        CParticle__StopLoopSound(arg0);
    }
}

#include "globals/swoosh.h"
#include "structs/particle_event.h"
#include "globals/particle.h"
#ifdef TARGET_PORT
#include "structs/player.h"
#endif

typedef struct Bone {
    /* 0x00 */ u8 pad0[0x40];
} Bone;
typedef struct Target Target;
#ifdef TARGET_PORT
struct Target {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Target(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_kind[T2_HOFF_Target(m_CharacterActor.header.m_Type)]; u8 kind; }; /* N64 0x0 */
        struct { u8 _hp_pos[T2_HOFF_Target(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_unk10[T2_HOFF_Target(m_CharacterActor.header.m_pCurrentRegion)]; sptr unk10; }; /* N64 0x10: a region pointer */
        struct { u8 _hp_def[T2_HOFF_Target(m_CharacterActor.header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_bones[T2_HOFF_Target(m_CharacterActor.m_pmtDrawMtxs)]; Bone *bones; }; /* N64 0x98 */
        struct { u8 _hp_flags[T2_HOFF_Target(m_CharacterActor.m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_aiFlags[T2_HOFF_Target(m_CharacterActor.m_AI.m_dwFlags)]; s32 aiFlags; }; /* N64 0x140 */
        struct { u8 _hp_callback[T2_HOFF_Target(m_CharacterActor.m_AI.m_pfHit)]; void (*callback)(Target *target, s32 *ai, void *particle); }; /* N64 0x240 */
        struct { u8 _hp_extraUses[T2_HOFF_Target(m_nNumArrowsStuckInMe)]; s32 extraUses; }; /* N64 0x12B0 */
    };
};
#else
struct Target {
    /* 0x0000 */ u8 kind;
    /* 0x0001 */ u8 pad1[3];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ s32 unk10;
    /* 0x0014 */ CIntelligence *def;
    /* 0x0018 */ u8 pad18[0x98 - 0x18];
    /* 0x0098 */ Bone *bones;
    /* 0x009C */ u8 pad9C[0xD4 - 0x9C];
    /* 0x00D4 */ u32 flags;
    /* 0x00D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x0140 */ s32 aiFlags;
    /* 0x0144 */ u8 pad144[0x240 - 0x144];
    /* 0x0240 */ void (*callback)(Target *target, s32 *ai, void *particle);
    /* 0x0244 */ u8 pad244[0x12B0 - 0x244];
    /* 0x12B0 */ s32 extraUses;
};
#endif

s32 GameObject_IsInanimate(Target *target);
s32 Actor_GetTypeClass(Target *target);
void CMtxF__FromMtxAffine(void *out, void *in);
void func_0021B39C(Target *target, s32 *ai, f32 *matrix, CVector3 origin, CVector3 direction, s32 frame);
void CMtxF__InvertAffine(void *out, void *in);
void CMtxF__RotateVector(void *m, CVector3 *in, CVector3 *out);
void AI_HearNoise(void *target, void *ai, CVector3 *from, s32 arg3);
void CParticle__BuildCameraMatrix(void *arg0);
void Trail_DetachOwner(void *list, void *owner);
void CDLightSet__Free(void *list, CDLight *light);
s32 CParticle__IsUnderWater(CParticle *obj);
s32 func_00268148(Target *target);
void Sound__Play(s16 sound, CVector3 pos, sptr arg2, sptr arg3);

/* Sticks the particle to the bone it hit: its position and velocity in the bone's frame. */
static __inline__ void attachParticle(CParticle *p, f32 *transform, s32 action) {
    f32 localTransform[16];
    f32 inverse[16];
    CVector3 direction;
    Target *attached;
    CCollisionResults *scene;

    attached = (Target *)gCollisionResults.m_pInstance;
    if (transform) {
        CMtxF__InvertAffine(inverse, transform);
    } else {
        CMtxF__FromMtxAffine(localTransform, &attached->bones[gCollisionResults.m_nInstanceNode]);
        CMtxF__InvertAffine(inverse, localTransform);
    }
    scene = &gCollisionResults;
    CMtxF__TransformPoint(inverse, &scene->m_vInstancePos, &p->m_vAttachPos);
    direction = scene->m_vVelocity;
    CVector3__Normalize(&direction);
    CMtxF__RotateVector(inverse, &direction, &p->m_vVelocity);
    p->m_dwFlags |= 0x10000;
    p->m_pTarget = (CGameObjectInstance *)attached;
    p->m_nBone = gCollisionResults.m_nInstanceNode;
    if (attached->def->m_Type == 1) {
        if (p->m_pOwner != NULL) {
            AI_HearNoise(attached, &attached->aiFlags, &p->m_pOwner->header.m_vPos, 3);
        } else {
            AI_HearNoise(attached, &attached->aiFlags, &gCollisionResults.m_vInstancePos, 3);
        }
    }
    if (p->header.m_nObjType == 0x56 && action == 2) {
        p->m_dwFlags |= 0x4000000;
    }
}

/* A particle hit the current collision target: attaches to it when its type sticks, then
 * plays the impact's model, effects and sound for the hit's action and removes the particle
 * when its descriptor says so. */
void CParticle__HitTarget(CParticle *p) {
    CEventValue primary;
    CEventValue secondary;
    f32 boneMatrix[16];
    Target *source;
    Target *owner;
    CROMParticleEffect *def;
    CROMParticleImpact *actions;
    CROMParticleImpact *extraActions;
    void *other;
    s32 mode;
    s32 enabled;
    s32 action;
    s32 special;
    s32 draw;
    s32 model;
    s32 effect;
    s32 sound;
    s32 extraEffect;
    u16 type;

    source = (Target *)gCollisionResults.m_pInstance;
    enabled = 1;
    special = 0;
    def = p->m_pEffect;
    if (GameObject_IsInanimate(source)) {
        mode = p->m_bDieOnImpact;
    } else {
        mode = p->m_ObjectImpact;
    }
    if (p->header.m_pCurrentRegion == NULL) {
        p->header.m_pCurrentRegion = (struct CGameRegion *)source->unk10;
    }
    if (gCollisionResults.m_nInstanceNode != -1) {
        action = gCollisionResults.m_Frame.action;
        if (p->header.m_nObjType == 13) {
            action = 4;
        }
        owner = (Target *)gCollisionResults.m_pInstance;
        if (owner->flags & 0x3000000) {
            CMtxF__FromMtxAffine(boneMatrix, &owner->bones[gCollisionResults.m_nInstanceNode]);
            if (owner->flags & 0x1000000) {
                func_0021B39C(owner, &owner->aiFlags, boneMatrix, gCollisionResults.m_vInstancePos, gCollisionResults.m_vVelocity,
                              gCollisionResults.m_nInstanceNode);
            }
            if (owner->flags & 0x2000000) {
                type = p->header.m_nObjType;
                if (type == 0x12A || type == 0x132) {
                    if (owner->def->m_Type == 11) {
                        special = owner->extraUses < 1;
                        if (special) {
                            owner->extraUses++;
                        }
                    }
                }
                if (p->header.m_nObjType == 0x56 || p->header.m_nObjType == 2 || p->header.m_nObjType == 0x126 || p->header.m_nObjType == 15 || special
                    || (p->header.m_nObjType == 11 && (gCollisionResults.m_Frame.flags & 3) == 1)) {
                    attachParticle(p, boneMatrix, action);
                    if (p->m_ActiveSwooshes) {
                        CParticle__BuildCameraMatrix(p);
                        Trail_DetachOwner(&gEngineApp_m_SwooshSystem, p);
                    }
                    if (p->m_pDLight) {
                        CDLightSet__Free((u8 *) &gScene.m_DLightSet, p->m_pDLight);
                        p->m_pDLight = NULL;
                    }
                }
            }
        }
    } else {
        action = Actor_GetTypeClass(source);
    }
    if (action == 5) {
        action = 4;
        p->m_dwFlags |= 2;
    }
    if (CParticle__IsUnderWater(p) && action < 6 && action >= 4) {
        action = 6;
    }
    actions = def->m_pImpact;
    model = actions->m_ImpactParticleType[action];
    effect = actions->m_ImpactEventType[action];
    primary = actions->m_ImpactEventValue[action];
    sound = def->m_pImpact->m_ImpactSoundType[action];
    if (source->kind == 1) {
        if (source->callback) {
            source->callback(source, &source->aiFlags, p);
        }
        if (func_00268148(source)) {
            enabled = 0;
        }
        if (source->aiFlags & 2) {
            enabled = 0;
        }
    }
    if (effect != 0xFFFF) {
        if (gImpactEventHandlers[effect].flags & 4) {
            if (p->header.m_nObjType == 0x56) {
                primary.lower = primary.lower * p->m_Scale;
            }
        }
        other = source;
        if (gImpactEventHandlers[effect].flags & 2) {
            other = p;
        }
        ImpactEvent_Send(other, p, effect, -1, gCollisionResults.m_vInstancePos, primary);
    }
    if (action != 4) {
        draw = 1;
        if (gEngineApp_m_Settings.multiplayer) {
            draw = source->def->m_Type != 11;
            if (mode == p->m_bDieOnImpact) {
                draw = 0;
            }
        }
        if (draw) {
            extraActions = def->m_pImpact;
            extraEffect = extraActions->m_ImpactEventType[4];
            if (extraEffect != 0xFFFF) {
                secondary = extraActions->m_ImpactEventValue[4];
                if (gImpactEventHandlers[extraEffect].flags & 4) {
                    other = source;
                    if (gImpactEventHandlers[extraEffect].flags & 2) {
                        other = p;
                    }
                    ImpactEvent_Send(other, p, extraEffect, -1, gCollisionResults.m_vInstancePos, secondary);
                }
            }
        }
    }
    if (model != 0xFFFF) {
        CParticle__SpawnChild(p, model, -8, enabled);
    }
    if (sound != 0xFFFF) {
        Sound__Play((s16) sound, gCollisionResults.m_vInstancePos, 0, -1);
    }
    if (!(p->m_dwFlags & 0x30000)) {
        if (mode == 1 || (effect != 0xFFFF && (gImpactEventHandlers[effect].flags & 1))) {
            CParticleSystem__Free(&gScene.m_ParticleSystem, p);
            CParticle__StopLoopSound(p);
        }
    }
}

void SwooshSystem_PlaceDecal(void *list, CVector3 pos, CVector3 normal, s32 kind, sptr arg4);
f32 Random_Blend(f32 lo, f32 hi);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);

/* Puts the particle at the hit point, flying along the hit direction, and pulls it back
 * along that direction by a random distance. */
static __inline__ void placeAtHit(CParticle *p) {
    CVector3 dir;

    p->header.m_vPos = gCollisionResults.m_vInstancePos;
    p->m_vVelocity = gCollisionResults.m_vVelocity;
    dir = gCollisionResults.m_vVelocity;
    CVector3__Normalize(&dir);
    p->m_vDirection = dir;
    CVector3__Scale(&dir, &dir, Random_Blend(-49.92f, 0.0f));
    CVector3__Add(&p->header.m_vPos, &p->header.m_vPos, &dir);
}

/* A particle hit the world (no target): leaves a decal, sticks or bounces off the surface
 * by type, plays the impact's effect, model and sound, and dies when its descriptor says so.
 * A particle under player control plays its own impact instead and always dies. */
void CParticle__HitInstanceBody(CParticle *p) {
    CEventValue params;
    CVector3 normal;
    CEventValue controlledParams;
    Target *source;
    CROMParticleEffect *definition;
    s32 kind;

    source = (Target *)gCollisionResults.m_pInstance;
    definition = p->m_pEffect;
    kind = Actor_GetTypeClass(source);
    if (kind == 5) {
        kind = 4;
        p->m_dwFlags |= 2;
    }
    if (gEngineApp_m_Settings.multiplayer != 0 && CParticle__IsUnderWater(p)) {
        CROMParticleEffect *controlledDefinition = p->m_pEffect;
        s32 controlledKind, model, sound, effect;

        if (CParticle__IsUnderWater(p)) {
            controlledKind = 12;
        } else {
            controlledKind = 10;
        }
        model = controlledDefinition->m_pImpact->m_ImpactParticleType[controlledKind];
        effect = controlledDefinition->m_pImpact->m_ImpactEventType[controlledKind];
        controlledParams = controlledDefinition->m_pImpact->m_ImpactEventValue[controlledKind];
        sound = controlledDefinition->m_pImpact->m_ImpactSoundType[controlledKind];
        if (effect != 0xFFFF) {
            ImpactEvent_Send(p, p, effect, -1, p->header.m_vPos, controlledParams);
        }
        if (model != 0xFFFF) {
            CParticle__SpawnChild(p, model, p->m_Alignment, 1);
        }
        if (sound != 0xFFFF) {
            Sound__Play(sound, p->header.m_vPos, 0, -1);
        }
        goto remove;
    }
    switch (kind) {
        case 0:
        case 2:
        case 3:
            if (definition->m_dwFlags & 0x800000) {
                SwooshSystem_PlaceDecal(&gEngineApp_m_SwooshSystem, gCollisionResults.m_vInstancePos, gCollisionResults.m_vNormal, kind, 0);
            }
            break;
    }
    if (p->header.m_nObjType == 0x56 || p->header.m_nObjType == 0x126 || p->header.m_nObjType == 0xCD || p->header.m_nObjType == 0xF) {
        p->m_dwFlags |= 0x20000;
        placeAtHit(p);
        p->m_Shake = 0x7F;
        if (p->header.m_nObjType == 0x56 && kind == 2) {
            p->m_dwFlags |= 0x4000000;
        }
        if (p->m_ActiveSwooshes) {
            CParticle__BuildCameraMatrix(p);
            Trail_DetachOwner(&gEngineApp_m_SwooshSystem, p);
        }
        if (p->m_pDLight) {
            CDLightSet__Free(&gScene.m_DLightSet, p->m_pDLight);
            p->m_pDLight = NULL;
        }
    }
    normal = gCollisionResults.m_vNormal;
    CVector3__Normalize(&normal);
    CVector3__Scale(&normal, &normal, 1.024f);
    CVector3__Add(&p->header.m_vPos, &p->header.m_vPos, &normal);
    {
        s32 model, sound, effect;

        model = definition->m_pImpact->m_ImpactParticleType[kind];
        effect = definition->m_pImpact->m_ImpactEventType[kind];
        params = definition->m_pImpact->m_ImpactEventValue[kind];
        sound = definition->m_pImpact->m_ImpactSoundType[kind];
        if (effect != 0xFFFF) {
            void *eventSource = source;

            if (gImpactEventHandlers[effect].flags & 2) {
                eventSource = p;
            }
            ImpactEvent_Send(eventSource, p, effect, -1, gCollisionResults.m_vInstancePos, params);
        }
        if (model != 0xFFFF) {
            CParticle__SpawnChild(p, model, -8, 1);
        }
        if (sound != 0xFFFF) {
            Sound__Play(sound, gCollisionResults.m_vInstancePos, 0, -1);
        }
    }
    if (!(p->m_dwFlags & 0x30000) && p->m_bDieOnImpact == 1) {
remove:
        CParticleSystem__Free(&gScene.m_ParticleSystem, p);
        CParticle__StopLoopSound(p);
    }
}

#ifdef TARGET_PORT
#include "structs/camera.h"
#endif

/* The transform a particle's position is relative to, picked by its
 * attachment mode: the particle's own facing, the owner's effect transforms,
 * the region's ground or wall orientation, or a fixed one. */

extern void CMtxF__SetLookDirection(CMtxF m, CVector3 *dir);             /* matrix facing dir */
extern f32 CGameRegion__GetHeight(void *region, f32 x, f32 z);        /* ground height */
extern void CGameRegion__GetTiltRotation(CQuaternion *out, void *region);         /* ground orientation */
extern void CGameRegion__GetCeilingTiltRotation(CQuaternion *out, void *region);         /* wall orientation */
extern void CMtxF__MultiplyAffine(CMtxF out, CMtxF a, CMtxF b);      /* out = a * b */
extern f32 CGameRegion__GetEdgeYaw(void *region, s32 attach);
extern void CMtxF__SetRotateY(CMtxF m, f32 angle);

CMtxF *CParticle__GetParentTransform(CParticle *p, u8 *view) {
    CMtxF output;
    CQuaternion state;

    switch (p->m_Alignment) {
    case -8:
        CMtxF__SetLookDirection(sParticleParentMtx, &p->m_vDirection);
        return &sParticleParentMtx;
    case -6:
        if (view == NULL) {
            return NULL;
        }
#ifdef TARGET_PORT
        return &((CCamera *) view)->m_mfFaceScreenYAxisUp;
#else
        return (CMtxF *)(view + 0xE7C);
#endif
    case -4:
    case -1:
        if (view == NULL) {
            return NULL;
        }
#ifdef TARGET_PORT
        return &((CCamera *) view)->m_mfFaceScreen;
#else
        return (CMtxF *)(view + 0xE3C);
#endif
    case -3:
        if (p->header.m_vPos.y - CGameRegion__GetHeight(p->header.m_pCurrentRegion, p->header.m_vPos.x, p->header.m_vPos.z) <= 0.01f) {
            CGameRegion__GetTiltRotation(&state, p->header.m_pCurrentRegion);
            CQuaternion__ToMatrix(&state, output);
            CMtxF__MultiplyAffine(sParticleParentMtx, sParticleRotXMtx, output);
            return &sParticleParentMtx;
        }
    case -5:
        return &sParticleRotXMtx;
    case -2:
        CGameRegion__GetCeilingTiltRotation(&state, p->header.m_pCurrentRegion);
        CQuaternion__ToMatrix(&state, output);
        CMtxF__MultiplyAffine(sParticleParentMtx, sParticleRotXMtx, output);
        return &sParticleParentMtx;
    case 0:
    case 1:
    case 2:
        CMtxF__SetRotateY(sParticleParentMtx, CGameRegion__GetEdgeYaw(p->header.m_pCurrentRegion, p->m_Alignment));
        return &sParticleParentMtx;
    }
    return NULL;
}

/* Moves a particle attached to an actor along with it: onto a bone of its
 * parent's skeleton (or the parent's position) and/or through its owner's
 * transform; returns the resulting position and heading. */

typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ CMtxF transform;
} PlayerView;

void CParticle__Attach(CParticle *actor, CVector3 *outPosition, CVector3 *outHeading) {
    CMtxF matrix;
    CVector3 position;
    CVector3 heading;
    CGameObjectInstance *link;
    u8 *skeleton;

    position = actor->header.m_vPos;
    heading = actor->m_vVelocity;
    if (actor->m_dwFlags & 0x10000) {
        link = actor->m_pTarget;
        switch (actor->header.m_nObjType) {
        case 2:
        case 0xB:
        case 0xF:
        case 0x2D:
        case 0x4F:
        case 0x56:
        case 0x126:
        case 0x12A:
        case 0x132:
            skeleton = (u8 *)link->m_pmtDrawMtxs;
            if (skeleton != NULL) {
                CMtxF__FromMtxAffine(matrix, skeleton + actor->m_nBone * 64);
                CMtxF__TransformPoint(matrix, &actor->m_vAttachPos, &position);
                CMtxF__RotateVector(matrix, &actor->m_vVelocity, &heading);
                actor->header.m_vPos = position;
                actor->m_vDirection = heading;
            } else {
                position = link->header.m_vPos;
                actor->header.m_vPos = position;
            }
            break;
        }
        actor->header.m_pCurrentRegion = link->header.m_pCurrentRegion;
    }
    if (actor->m_dwFlags & 0x400000) {
        link = actor->m_pOwner;
        if (link->m_dwFlags & 0x300000) {
            /* a player: its view's transform */
#ifdef TARGET_PORT
            CMtxF__TransformPoint(((CPlayer *)link->m_AI.m_pParent)->m_pCamera->m_mfViewOrient, &actor->m_vAttachPos, &position);
#else
            CMtxF__TransformPoint(((PlayerView *)((CPlayer *)link->m_AI.m_pParent)->m_pCamera)->transform, &actor->m_vAttachPos, &position);
#endif
        } else {
            CMtxF__TransformPoint(link->m_mfOrient, &actor->m_vAttachPos, &position);
        }
        actor->header.m_vPos = position;
    }
    if (outPosition != NULL) {
        *outPosition = position;
    }
    if (outHeading != NULL) {
        *outHeading = heading;
    }
}

#include "structs/list.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/frame.h"
#include "globals/render.h"
#include "globals/math.h"
#ifdef TARGET_PORT
#include "structs/level_actor.h"
#include "structs/player.h"
struct CROMParticleEffect *T2Port_ParticleEffects(void *data, s32 *count); /* port/game/t2particles.c */
#endif
#include "protos/list.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "protos/scene.h"
#include "structs/camera.h"
#define MAX(a, b) ((a) < (b) ? (b) : (a))
/* MIN as a statement expression: its value lives in its own temporary. */
#define MINF(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
/* Matches with four register pins (the `register ... asm` lines in the inlined
 * depth scale and at its call), no other trick. Why pins: in the ROM the depth
 * argument stays in $f5 and its working copy in $f4 (so depth conflicts with
 * every value of the inline, as if read again after the copy), and the range's
 * maximum/minimum get $f2/$f3 because $f0/$f1 look busy to local-alloc in the
 * join block; no natural source shape tried (RW `size *=` form, lerp/farness/
 * range helpers as inlines, reused temporaries, early returns, depth reused
 * after the copy, permuter) reproduced either. Without the pins the best
 * natural form was 13 of 624 words. The L pass also tried the depth scaled in
 * place (`depth *= ...`), a `range` local, the size as one expression, a lerp
 * inline, and an early `return 1.0f`: none came closer (each misplaces the
 * clamp's result and the range in $f0/$f1). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_4.CPP, Particle_BuildMatrix, with Turok 2's layouts. */

/* Builds a particle's display matrix: its size scaled for camera depth and
 * field of view, its wobble, its orientation (model particles) or roll
 * (sprites), its placement for the space it lives in, and an optional
 * decaying shake, then packs the result into the fixed-point matrix the
 * display list loads. */

#define MIN_LT(a, b) ((a) < (b) ? (a) : (b))
#define MAX_GT(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 minimum;
    /* 0x20 */ f32 maximum;
} DistanceRange;
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 ramp;
} Oscillation;
#ifdef TARGET_PORT
typedef struct MtxParticleDesc {
/* the view at the host offsets of CROMParticleEffect (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_MtxParticleDesc(m) __builtin_offsetof(CROMParticleEffect, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_MtxParticleDesc(m_dwFlags)]; u32 flags; }; /* N64 0x0 */
        struct { u8 _hp_animMode[T2_HOFF_MtxParticleDesc(m_AnimMode)]; s8 animMode; }; /* N64 0x8 */
        struct { u8 _hp_clampToGround[T2_HOFF_MtxParticleDesc(m_bClampToGround)]; s8 clampToGround; }; /* N64 0xA */
        struct { u8 _hp_renderModel[T2_HOFF_MtxParticleDesc(m_pModel)]; s32 renderModel; }; /* N64 0x14 */
        struct { u8 _hp_distanceRange[T2_HOFF_MtxParticleDesc(m_pDistanceRange)]; DistanceRange *distanceRange; }; /* N64 0x24 */
        struct { u8 _hp_oscillation[T2_HOFF_MtxParticleDesc(m_pOscillation)]; Oscillation *oscillation; }; /* N64 0x2C */
    };
} MtxParticleDesc;
#else
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x8 - 0x4];
    /* 0x08 */ s8 animMode;
    /* 0x09 */ u8 pad9;
    /* 0x0A */ s8 clampToGround;
    /* 0x0B */ u8 padB[0x14 - 0xB];
    /* 0x14 */ s32 renderModel;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ DistanceRange *distanceRange;
    /* 0x28 */ u8 pad28[0x2C - 0x28];
    /* 0x2C */ Oscillation *oscillation;
} MtxParticleDesc;
#endif
#ifdef TARGET_PORT
typedef struct MtxParticle {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_MtxParticle(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_region[T2_HOFF_MtxParticle(header.m_pCurrentRegion)]; void *region; }; /* N64 0x10 */
        struct { u8 _hp_flags[T2_HOFF_MtxParticle(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_desc[T2_HOFF_MtxParticle(m_pEffect)]; MtxParticleDesc *desc; }; /* N64 0xF8 */
        struct { u8 _hp_time[T2_HOFF_MtxParticle(m_cFrame)]; f32 time; }; /* N64 0x11C */
        struct { u8 _hp_size[T2_HOFF_MtxParticle(m_vSize)]; CVector3 size; }; /* N64 0x12C */
        struct { u8 _hp_direction[T2_HOFF_MtxParticle(m_vDirection)]; CVector3 direction; }; /* N64 0x150 */
        struct { u8 _hp_rot[T2_HOFF_MtxParticle(m_vRot)]; CVector3 rot; }; /* N64 0x15C */
        struct { u8 _hp_phaseA[T2_HOFF_MtxParticle(m_WobblePhase)]; f32 phaseA; }; /* N64 0x178 */
        struct { u8 _hp_phaseB[T2_HOFF_MtxParticle(m_WobblePhase[1])]; f32 phaseB; }; /* N64 0x17C */
        struct { u8 _hp_amplitudeA[T2_HOFF_MtxParticle(m_WobbleAmplitude)]; f32 amplitudeA; }; /* N64 0x188 */
        struct { u8 _hp_amplitudeB[T2_HOFF_MtxParticle(m_WobbleAmplitude[1])]; f32 amplitudeB; }; /* N64 0x18C */
        struct { u8 _hp_space[T2_HOFF_MtxParticle(m_Alignment)]; s8 space; }; /* N64 0x1AC */
        struct { u8 _hp_shake[T2_HOFF_MtxParticle(m_Shake)]; s8 shake; }; /* N64 0x1AD */
        struct { u8 _hp_opacity[T2_HOFF_MtxParticle(m_Opacity)]; u8 opacity; }; /* N64 0x1B4 */
    };
} MtxParticle;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x40 - 0x14];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ MtxParticleDesc *desc;
    /* 0x0FC */ u8 padFC[0x11C - 0xFC];
    /* 0x11C */ f32 time;
    /* 0x120 */ u8 pad120[0x12C - 0x120];
    /* 0x12C */ CVector3 size;
    /* 0x138 */ u8 pad138[0x150 - 0x138];
    /* 0x150 */ CVector3 direction;
    /* 0x15C */ CVector3 rot;
    /* 0x168 */ u8 pad168[0x178 - 0x168];
    /* 0x178 */ f32 phaseA;
    /* 0x17C */ f32 phaseB;
    /* 0x180 */ u8 pad180[0x188 - 0x180];
    /* 0x188 */ f32 amplitudeA;
    /* 0x18C */ f32 amplitudeB;
    /* 0x190 */ u8 pad190[0x1AC - 0x190];
    /* 0x1AC */ s8 space;
    /* 0x1AD */ s8 shake;
    /* 0x1AE */ u8 pad1AE[0x1B4 - 0x1AE];
    /* 0x1B4 */ u8 opacity;
} MtxParticle;
#endif

/* CParticle__Attach under this part's view (the effect code below declares it as (Effect *, s32, s32)). */
extern void MtxParticle_Place(MtxParticle *p, CVector3 *outPosition, CVector3 *outHeading) asm("CParticle__Attach");
extern f32 func_002982F0(f32 angle);                             /* sin */
extern void CMtxF__SetEulerXYZ(CMtxF m, f32 x, f32 y, f32 z);         /* rotation */
extern void CMtxF__Identity(CMtxF *m);                              /* identity */
void CMtxF__SetRotateZ(void *m, f32 angle);
extern void CMtxF__TranslateLocal(CMtxF m, f32 x, f32 y, f32 z);
extern void CMtxF__Translate(CMtxF *m, f32 x, f32 y, f32 z);         /* translate */
extern f32 CGameRegion__GetCeilingHeight(void *region, f32 x, f32 z);            /* ceiling height */
extern void CVector3__ClampLength(CVector3 *v, f32 limit);
extern void CMtxF__ScaleAxes(CMtxF *m, f32 x, f32 y, f32 z);         /* scale */
extern void CMtxF__SwapYZRows(CMtxF m);
extern void CMtxF__Copy(CMtxF dst, CMtxF src);                 /* copy */
extern void CMtxF__SetTranslate(CMtxF *m, f32 x, f32 y, f32 z);         /* translation */
extern void CMtxF__RotateY(CMtxF m, f32 angle);
extern void CMtxF__RotateX(CMtxF m, f32 angle);
extern void CMtxF__ToMtxAffine(CMtxF *m, void *out);                   /* pack */

static __inline__ f32 particleDepthScale(MtxParticleDesc *desc, f32 depth) {
    f32 farness;
    f32 near;
    f32 size;

    if (desc->flags & 4) {
        register f32 s asm("$f4") = depth;    /* pin, see top */
        near = s - 16.0f;
        farness = MIN_LT(1.0f, MAX_GT(0.0f, near) * (1.0f / 2032.0f));
        {
            /* pins, see top */
            register f32 hi asm("$f2") = desc->distanceRange->maximum;
            register f32 lo asm("$f3") = desc->distanceRange->minimum;
            s *= lo + (hi - lo) * farness;
        }
        size = s * 0.001f;
    } else {
        size = 1.0f;
    }
    return size;
}

/* Written as an inline with an early return: the ROM's register choices for
 * the fade and the wobble's 0.0 come from that shape. */
static __inline__ f32 rampFade(f32 time, s32 ramp) {
    f32 length;

    length = (f32)ramp;
    if (time < length) {
        return time / length;
    }
    return 1.0f;
}

void CParticle__BuildMatrix(MtxParticle *particle, void *out, CCamera *camera, f32 depth) {
    CMtxF rotation, basis, local, world, shakeMtx;
    CVector3 position;
    CVector3 up;
    CVector3 heading;
    CVector3 axisX;
    CVector3 axisY;
    CVector3 axisZ;
    CVector3 across;
    CVector3 side;
    CVector3 direction;
    CVector3 offset;
    MtxParticleDesc *desc;
    f32 *final;
    f32 scale;
    f32 lift;
    f32 depthScale;
    f32 ramp;
    f32 fade;
    f32 ground;
    f32 rest;
    f32 extent;
    f32 jolt;
    f32 joltSq;
    f32 angle;
    f32 tiltX;
    f32 tiltY;
    f32 fov;
    s32 shake;
    s32 model;

    desc = particle->desc;
    if (camera != 0) {
        {
            register f32 d asm("$f5") = depth;     /* pin, see top */
            depthScale = particleDepthScale(particle->desc, d);
        }
        fov = camera->m_Fov;
        scale = depthScale * (1.0f / 47.5f) * (fov + (47.5f - fov) * 0.25f);
    } else {
        scale = 1.0f;
    }
    if (desc->flags & 8) {
        lift = particle->size.y * particle->size.z * scale;
    } else {
        lift = 0.0f;
    }
    MtxParticle_Place(particle, &position, &heading);
    if (!(particle->flags & 0x30000)) {
        ramp = particle->time;
        if (ramp > 0.0f) {
            fade = rampFade(ramp, desc->oscillation->ramp);
            direction = particle->direction;
            if (particle->amplitudeA < 0.0f) {
                up.x = 0;
                up.z = 0;
                up.y = 1.0f;
                CVector3__Cross(&side, &up, &direction);
                CVector3__Scale(&offset, &side, func_002982F0(particle->phaseA) * -particle->amplitudeA * fade);
                CVector3__Add(&position, &position, &offset);
            }
            if (particle->amplitudeB < 0.0f) {
                up.x = 0;
                up.z = 0;
                up.y = 1.0f;
                CVector3__Cross(&across, &up, &direction);
                CVector3__Cross(&side, &across, &direction);
                CVector3__Scale(&offset, &side, func_002982F0(particle->phaseB) * -particle->amplitudeB * fade);
                CVector3__Add(&position, &position, &offset);
            }
        }
    }
    if (particle->desc->renderModel != 0) {
        if (particle->space == -7) {
            CMtxF__SetEulerXYZ(rotation, particle->rot.x + 3.141593f / 2, particle->rot.y, particle->rot.z);
            if ((heading.x == 0.0f) && (heading.y == 0.0f) && (heading.z == 0.0f)) {
                axisZ = particle->direction;
            } else {
                axisZ = heading;
            }
            CVector3__Normalize(&axisZ);
            up.x = 0;
            up.z = 0;
            up.y = 1.0f;
            CVector3__Cross(&axisX, &up, &axisZ);
            CVector3__Cross(&axisY, &axisX, &axisZ);
            CMtxF__Identity((CMtxF *) basis);
            CVector3__Normalize(&axisX);
            CVector3__Normalize(&axisY);
            CVector3__Normalize(&axisZ);
            basis[0][0] = axisX.x;
            basis[0][1] = axisX.y;
            basis[0][2] = axisX.z;
            basis[1][0] = axisY.x;
            basis[1][1] = axisY.y;
            basis[1][2] = axisY.z;
            basis[2][0] = axisZ.x;
            basis[2][1] = axisZ.y;
            basis[2][2] = axisZ.z;
            CMtxF__MultiplyAffine(local, rotation, basis);
        } else {
            CMtxF__SetEulerXYZ(local, particle->rot.x, particle->rot.y + 3.141593f, particle->rot.z);
        }
    } else if ((particle->rot.x == 0.0f) && (particle->rot.y == 0.0f)) {
        CMtxF__SetRotateZ(local, particle->rot.z);
    } else {
        CMtxF__SetRotateZ(local, particle->rot.z);
        model = particle->desc->renderModel;
        tiltX = 0.0f;
        if (model == 0) {
            tiltX = particle->rot.x;
        }
        tiltX = -tiltX;
        tiltY = 0.0f;
        if (model == 0) {
            tiltY = particle->rot.y;
        }
        CMtxF__TranslateLocal(local, tiltX, -tiltY, 0.0f);
    }
    if (lift != 0.0f) {
        CMtxF__TranslateLocal(local, 0.0f, lift, 0.0f);
        CMtxF__Translate((CMtxF *) local, 0.0f, -lift, 0.0f);
    }
    particle->flags &= ~0x800000;
    switch (particle->space) {
    case -8:
        position.x += particle->direction.x * 5.12f;
        position.y += particle->direction.y * 5.12f;
        position.z += particle->direction.z * 5.12f;
        break;
    case -5:
        if (desc->clampToGround != 0) {
            position.y = MAX_GT(position.y, CGameRegion__GetHeight(particle->region, position.x, position.z) + 5.12f);
        }
        break;
    case -3:
        ground = CGameRegion__GetHeight(particle->region, position.x, position.z);
        if (desc->flags & 0x400) {
            if (position.y < ground + 1.024f) {
                position.y = ground;
                particle->flags |= 0x800000;
            }
        } else {
            rest = ground + 5.12f;
            position.y = (position.y <= rest) ? rest : position.y;
        }
        break;
    case -2:
        if (desc->clampToGround != 0) {
            position.y = MIN_LT(position.y, CGameRegion__GetCeilingHeight(particle->region, position.x, position.z) - 5.12f);
        }
        break;
    case -6:
    case -4:
    case -1:
        position.y += lift;
        break;
    case -7:
    case 0:
    case 1:
    case 2:
        break;
    }
    CVector3__ClampLength(&position, 20000.0f);
    if ((desc->flags & 0x80000) && (desc->animMode == 1)) {
        scale = scale * (1.0f / 255.0f) * (f32)particle->opacity;
    }
    if (particle->desc->renderModel != 0) {
        extent = scale * particle->size.z * (35.0f / 1024.0f);
        CMtxF__ScaleAxes((CMtxF *) local, particle->size.x * extent, particle->size.y * extent, extent * 10.24f);
        CMtxF__SwapYZRows(local);
    } else {
        extent = scale * particle->size.z;
        CMtxF__ScaleAxes((CMtxF *) local, particle->size.x * extent, particle->size.y * extent, extent * 10.24f);
    }
    {
        CMtxF *parent = CParticle__GetParentTransform(particle, camera);
        if (parent != 0) {
            CMtxF__MultiplyAffine(world, local, *parent);
        } else {
            CMtxF__Copy(world, local);
        }
    }
    CMtxF__Translate((CMtxF *) world, position.x, position.y, position.z);
    if ((particle->flags & 0x20000) && (shake = particle->shake) != 0) {
        jolt = (f32)shake * (1.0f / 127.0f);
        joltSq = jolt * jolt;
        angle = joltSq * (3.141593f * 5 / 180);
        CMtxF__SetTranslate((CMtxF *) shakeMtx, 0.0f, 0.0f, -66.56f);
        CMtxF__RotateY(shakeMtx, angle * (func_002982F0(joltSq * (6 * 3.141593f)) * 0.25f));
        CMtxF__RotateX(shakeMtx, angle * func_002982F0(joltSq * (14 * 3.141593f)));
        CMtxF__Translate((CMtxF *) shakeMtx, 0.0f, 0.0f, 66.56f);
        CMtxF__MultiplyAffine(rotation, shakeMtx, world);
        final = rotation[0];
    } else {
        final = world[0];
    }
    CMtxF__ToMtxAffine((CMtxF *) final, out);
}

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ f32 shadowScale;
} ShadowParams;
#ifdef TARGET_PORT
typedef struct ShadowDef {
/* the view at the host offsets of CROMParticleEffect (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ShadowDef(m) __builtin_offsetof(CROMParticleEffect, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_ShadowDef(m_dwFlags)]; u32 flags; }; /* N64 0x0 */
        struct { u8 _hp_bigShadow[T2_HOFF_ShadowDef(m_pModel)]; s32 bigShadow; }; /* N64 0x14 */
        struct { u8 _hp_params[T2_HOFF_ShadowDef(m_pDistanceRange)]; ShadowParams *params; }; /* N64 0x24 */
    };
} ShadowDef;
#else
typedef struct {
    /* 0x00 */ u32 flags;
    u8 pad4[0x10];
    /* 0x14 */ s32 bigShadow;
    u8 pad18[0xC];
    /* 0x24 */ ShadowParams *params;
} ShadowDef;
#endif
#ifdef TARGET_PORT
typedef struct ShadowActor {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ShadowActor(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_ShadowActor(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_region[T2_HOFF_ShadowActor(header.m_pCurrentRegion)]; void *region; }; /* N64 0x10 */
        struct { u8 _hp_def[T2_HOFF_ShadowActor(m_pEffect)]; ShadowDef *def; }; /* N64 0xF8 */
        struct { u8 _hp_fade[T2_HOFF_ShadowActor(m_cFrame)]; f32 fade; }; /* N64 0x11C */
        struct { u8 _hp_fadeTime[T2_HOFF_ShadowActor(m_nFrames)]; s16 fadeTime; }; /* N64 0x128 */
        struct { u8 _hp_radius[T2_HOFF_ShadowActor(m_vSize.z)]; f32 radius; }; /* N64 0x134 */
    };
} ShadowActor;
#else
typedef struct {
    u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ void *region;
    u8 pad14[0xE4];
    /* 0x0F8 */ ShadowDef *def;
    u8 padFC[0x20];
    /* 0x11C */ f32 fade;
    u8 pad120[0x8];
    /* 0x128 */ s16 fadeTime;
    u8 pad12A[0xA];
    /* 0x134 */ f32 radius;
} ShadowActor;
#endif

void *MtxPool_Alloc(void *pool, s32 count);
void CGameRegion__GetTiltRotation(CQuaternion *out, void *region);
f32 GameObject_GetGroundHeight(ShadowActor *actor, s32 arg1);    /* ground height */
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CVector3__ClampLength(CVector3 *v, f32 arg1);
void CMtxF__ScaleAxes(CMtxF *m, f32 x, f32 y, f32 z);  /* scale */
void CMtxF__Translate(CMtxF *m, f32 x, f32 y, f32 z);  /* translate */
void CMtxF__ToMtxAffine(CMtxF *m, void *dst);
void Render_DrawShadowQuad(void *mtx, s32 arg1, u32 alpha);

/* Draws the blob shadow under an actor: on the ground of its region, or
 * where a ray cast straight down from it lands, tilted to the surface. */
void CParticle__DrawShadow(ShadowActor *actor, void *view) { /* view: unused (CParticleSystem__Draw passes the camera) */
    CMtxF mtx;
    CVector3 pos;
    CQuaternion rot;
    CQuaternion tilt;
    CQuaternion spin;
    CObjectHead probe;
    void *gmtx;
    f32 ground;
    f32 height;
    f32 alpha;
    f32 a;
    f32 size;

    gmtx = MtxPool_Alloc(gScene.m_Heap, 1);
    if (gmtx == NULL) {
        return;
    }
    if (actor->region != NULL) {
        CGameRegion__GetTiltRotation(&rot, actor->region);
        ground = GameObject_GetGroundHeight(actor, 0);
    } else {
        probe = *(CObjectHead *) actor;
        probe.header.m_vPos.y += 10.24f;
        pos.x = probe.header.m_vPos.x;
        pos.y = probe.header.m_vPos.y - 102400.0f;
        pos.z = probe.header.m_vPos.z;
        probe.m_vVelocity.x = 0;
        probe.m_vVelocity.y = 0;
        probe.m_vVelocity.z = 0;
        Collision_MoveInstance(&probe, pos, &gCollisionInfoLineOfSight);
        if (gCollisionResults.m_pInstance == NULL) {
            return;
        }
        CQuaternion__FromDirection(&tilt, &gCollisionResults.m_vNormal);
        spin.x = gQuaternionSin = func_002982F0(0.785398245f);
        spin.y = 0.0f;
        spin.z = 0.0f;
        spin.w = func_002974C0(0.785398245f);
        CQuaternion__Multiply(&rot, &spin, &tilt);
        ground = gCollisionResults.m_vInstancePos.y;
    }
    height = actor->pos.y - ground;
    a = 200.0f - height * 0.30517578f;
    alpha = MAX(MIN(a, 255.0f), 0.0f);
    size = actor->radius * 7.68f;
    if (actor->def->bigShadow) {
        size *= 8.0f;
    }
    size *= actor->def->params->shadowScale;
    size = MAX(MINF(size - height * 0.1f, size), 0.0f);
    if ((actor->def->flags & 1) && actor->fadeTime > 0.0f) {
        alpha *= (actor->fadeTime - actor->fade) / actor->fadeTime;
    }
    pos.x = actor->pos.x;
    pos.z = actor->pos.z;
    pos.y = ground;
    CVector3__ClampLength(&pos, 20000.0f);
    CQuaternion__ToMatrix(&rot, &mtx);
    CMtxF__ScaleAxes(&mtx, size, 1.0f, size);
    CMtxF__Translate(&mtx, pos.x, pos.y, pos.z);
    CMtxF__ToMtxAffine(&mtx, gmtx);
    { Gfx *_g = (Gfx *) (gDList++); _g->w0 = 0xE7000000; _g->w1 = 0; }
    { Gfx *_g = (Gfx *) (gDList++); _g->w0 = 0xE3000A01; _g->w1 = 0x100000; }
    Render_DrawShadowQuad(gmtx, 1, (u8) alpha);
    { Gfx *_g = (Gfx *) (gDList++); _g->w0 = 0xE7000000; _g->w1 = 0; }
    { Gfx *_g = (Gfx *) (gDList++); _g->w0 = 0xE3000A01; _g->w1 = 0; }
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_6.CPP, Particle_DrawSprite, with Turok 2's layouts. */

/* Draws one sprite particle for a view: fades particles hidden behind
 * geometry in or out, blends its colours towards the descriptor's targets,
 * picks the animation frame, builds its matrix (once per frame for a shared
 * camera, per view otherwise), selects the texture and render mode, works out
 * its alpha from life, fade-in, fade-out and distance from the view, and
 * emits the colour and draw commands. */


#ifndef STRUCTS_GBI_H /* the port includes structs/gbi.h through structs/game.h */
typedef struct { u8 pad0[0x40]; } Mtx;
#endif
/* The leading 0x34 bytes of a game object, as the collision test reads them. */
#ifdef TARGET_PORT
typedef struct ParticleHead {
/* the view at the host offsets of CObjectHead (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ParticleHead(m) __builtin_offsetof(CObjectHead, m)
    union {
        struct { u8 _hp_type[T2_HOFF_ParticleHead(header.m_nObjType)]; u16 type; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_ParticleHead(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        u8 _hp_size[sizeof(CObjectHead)];
    };
} ParticleHead;
#else
typedef struct {
    /* 0x00 */ u16 pad0;
    /* 0x02 */ u16 type;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[0x24];
} ParticleHead;
#endif /* size 0x34 */

typedef struct {
    /* 0x0 */ u8 kind;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ u8 shiftS;
    /* 0x3 */ u8 shiftT;
} AnimFrame;
typedef struct {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ AnimFrame *frame;
} AnimInfo;
typedef struct {
    /* 0x0 */ u8 pad0[0x6];
    /* 0x6 */ u8 prim[3];
    /* 0x9 */ u8 env[3];
} ParticleColors;
typedef struct {
    /* 0x0 */ u8 pad0[0x5];
    /* 0x5 */ s8 rangeMode;
    /* 0x6 */ u8 pad6[0x3];
    /* 0x9 */ s8 fadeIn;
    /* 0xA */ s8 fadeOut;
    /* 0xB */ u8 padB[0x3];
    /* 0xE */ s8 unkE;
} ParticleFade;
#ifdef TARGET_PORT
typedef struct ParticleDesc {
/* the view at the host offsets of CROMParticleEffect (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ParticleDesc(m) __builtin_offsetof(CROMParticleEffect, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_ParticleDesc(m_dwFlags)]; u32 flags; }; /* N64 0x0 */
        struct { u8 _hp_animMode[T2_HOFF_ParticleDesc(m_AnimMode)]; s8 animMode; }; /* N64 0x8 */
        struct { u8 _hp_colors[T2_HOFF_ParticleDesc(m_pColors)]; ParticleColors *colors; }; /* N64 0x34 */
        struct { u8 _hp_fade[T2_HOFF_ParticleDesc(m_pParams)]; ParticleFade *fade; }; /* N64 0x38 */
    };
} ParticleDesc;
#else
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x4];
    /* 0x08 */ s8 animMode;
    /* 0x09 */ u8 pad9[0x34 - 0x9];
    /* 0x34 */ ParticleColors *colors;
    /* 0x38 */ ParticleFade *fade;
} ParticleDesc;
#endif
#ifdef TARGET_PORT
typedef struct Particle {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Particle(m) __builtin_offsetof(CParticle, m)
    union {
        ParticleHead inst; /* N64 0x0: the CObjectHead the particle starts with */
        struct { u8 _hp_flags[T2_HOFF_Particle(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_mtx[T2_HOFF_Particle(m_mtxDraw)]; Mtx mtx[2]; }; /* N64 0x48 */
        struct { u8 _hp_desc[T2_HOFF_Particle(m_pEffect)]; ParticleDesc *desc; }; /* N64 0xF8 */
        struct { u8 _hp_model[T2_HOFF_Particle(unk_0FC)]; s32 model; }; /* N64 0xFC */
        struct { u8 _hp_time[T2_HOFF_Particle(m_cFrame)]; f32 time; }; /* N64 0x11C */
        struct { u8 _hp_frame[T2_HOFF_Particle(m_AnimFrame)]; f32 frame; }; /* N64 0x120 */
        struct { u8 _hp_life[T2_HOFF_Particle(m_nFrames)]; s16 life; }; /* N64 0x128 */
        struct { u8 _hp_builtFrame[T2_HOFF_Particle(m_BuiltFrame)]; s8 builtFrame; }; /* N64 0x12A */
        struct { u8 _hp_randomFrame[T2_HOFF_Particle(m_RandomFrame)]; s8 randomFrame; }; /* N64 0x12B */
        struct { u8 _hp_alpha[T2_HOFF_Particle(m_Alpha)]; f32 alpha; }; /* N64 0x174 */
        struct { u8 _hp_prim[T2_HOFF_Particle(m_PrimColor)]; u8 prim[3]; }; /* N64 0x1AE */
        struct { u8 _hp_env[T2_HOFF_Particle(m_EnvColor)]; u8 env[3]; }; /* N64 0x1B1 */
        struct { u8 _hp_opacity[T2_HOFF_Particle(m_Opacity)]; u8 opacity; }; /* N64 0x1B4 */
    };
} Particle;
#else
typedef struct {
    /* 0x000 */ ParticleHead inst;
    /* 0x034 */ u8 pad34[0x40 - 0x34];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0x48 - 0x44];
    /* 0x048 */ Mtx mtx[2];
    /* 0x0C8 */ u8 padC8[0xF8 - 0xC8];
    /* 0x0F8 */ ParticleDesc *desc;
    /* 0x0FC */ s32 model;
    /* 0x100 */ u8 pad100[0x11C - 0x100];
    /* 0x11C */ f32 time;
    /* 0x120 */ f32 frame;
    /* 0x124 */ u8 pad124[0x128 - 0x124];
    /* 0x128 */ s16 life;
    /* 0x12A */ s8 builtFrame;
    /* 0x12B */ s8 randomFrame;
    /* 0x12C */ u8 pad12C[0x174 - 0x12C];
    /* 0x174 */ f32 alpha;
    /* 0x178 */ u8 pad178[0x1AE - 0x178];
    /* 0x1AE */ u8 prim[3];
    /* 0x1B1 */ u8 env[3];
    /* 0x1B4 */ u8 opacity;
} Particle;
#endif
#ifdef TARGET_PORT
typedef struct ParticleView {
/* the view at the host offsets of CCamera (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ParticleView(m) __builtin_offsetof(CCamera, m)
    union {
        struct { u8 _hp_index[T2_HOFF_ParticleView(m_Index)]; s32 index; }; /* N64 0x8 */
        struct { u8 _hp_unk24[T2_HOFF_ParticleView(m_ViewParams.m_nType)]; s32 unk24; }; /* N64 0x24 */
        struct { u8 _hp_eye[T2_HOFF_ParticleView(m_vPos)]; CVector3 eye; }; /* N64 0x114 */
        struct { u8 _hp_viewMtx[T2_HOFF_ParticleView(m_mfView)]; f32 viewMtx[4][4]; }; /* N64 0x20C */
        struct { u8 _hp_fadeRange[T2_HOFF_ParticleView(m_FarClip)]; f32 fadeRange; }; /* N64 0x510 */
    };
} ParticleView;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ s32 index;
    /* 0x00C */ u8 padC[0x24 - 0xC];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 pad28[0x114 - 0x28];
    /* 0x114 */ CVector3 eye;
    /* 0x120 */ u8 pad120[0x20C - 0x120];
    /* 0x20C */ f32 viewMtx[4][4];
    /* 0x24C */ u8 pad24C[0x510 - 0x24C];
    /* 0x510 */ f32 fadeRange;
} ParticleView;
#endif

extern s32 gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
extern void *gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */ /* the current view */
extern u8 gParticleSpriteVtx[];             /* the sprite's four vertices */
extern void *Texture_Request(s32 *ref);
extern void Texture_GetInfo(void *model, AnimInfo *info);
extern void Texture_Load(s32 *ref, void *model, s32 frame, s32 a3, s32 shiftS, s32 shiftT, s32 a6, s32 a7, s32 a8);
void *CCamera__GetPlayer(void *cam);
extern void *MtxPool_Alloc(void *pool, s32 count);
extern void Render_SetCombineMode(s32 mode);
extern void Render_SetRenderMode(s32 mode);
extern s32 Random_Next(void);

static __inline__ s32 Particle_Occluded(Particle *particle, ParticleView *view) {
    ParticleHead *inst;
    s32 hit;

    inst = CCamera__GetPlayer(view);
    if (inst != 0 && view->unk24 == 0) {
        ParticleHead saved;
        saved = *inst;
        inst->pos = view->eye;
        hit = Collision_MoveInstance(inst, particle->inst.pos, &gCollisionInfoLineOfSight);
        *inst = saved;
    } else {
        ParticleHead probe;
        probe = particle->inst;
        probe.pos = view->eye;
        hit = Collision_MoveInstance(&probe, particle->inst.pos, &gCollisionInfoLineOfSight);
    }
    return hit;
}

static __inline__ s32 Particle_ColorMode(Particle *particle) {
    s32 mode = 0;

    if (particle->desc->flags & 0x2000000) {
        if (particle->flags & 2) {
            mode = 2;
        } else if (gEngineApp_m_Settings.unk17 == 2) {
            mode = 1;
        }
    }
    return mode;
}

static __inline__ s32 Particle_Scale(f32 value, f32 div) {
    return (value < 0.0f) ? 0 : (s32)((s32)value / div);
}

#define CLAMP_U8(v) (u32)(((v) > 255.0f ? 255.0f : (v)) < 0.0f ? 0.0f : ((v) > 255.0f ? 255.0f : (v)))
#define LERP(to, from, t) (((to) - (from)) * (t) + (from))

void CParticle__DrawSprite(Particle *particle, ParticleView *view) {
    AnimInfo info;
    u8 env[3];
    u8 prim[3];
    CVector3 viewPos;
    void *model;
    ParticleDesc *desc;
    void *mtx;
    f32 fade;
    f32 ratio;
    f32 dist;
    s32 hit;
    s32 alpha;
    s8 duration;
    s32 mode;
    s32 frame;
    s32 time;
    s32 textured;
    s32 shiftS;
    s32 shiftT;
    u8 *envTo;
    u8 *primTo;
    u8 *envFrom;
    u8 *primFrom;

    if (gDisplayListSize - ((uptr)gDList - (uptr)gEngineApp_m_pCurrentFrameData->m_pDisplayList) / sizeof(Gfx) < 3000) {
        return;
    }
    if (particle->time < 0.0f) {
        return;
    }
    model = Texture_Request(&particle->model);
    if (model == 0) {
        return;
    }
    desc = particle->desc;
    if ((desc->flags & 0x80000) && desc->animMode == 1) {
        if (particle->inst.type != 0x60) {
            particle->time = 0.0f;
        }
        hit = Particle_Occluded(particle, view);
        if (!hit) {
            fade = particle->opacity + gFrameIncrement * 128.0f;
            alpha = (fade > 255.0f) ? 255 : (s32)fade;
        } else {
            fade = particle->opacity - gFrameIncrement * 128.0f;
            alpha = (fade < 0.0f) ? 0 : (s32)fade;
        }
        particle->opacity = alpha;
        if (alpha == 0.0f) {
            CCache__Unlock(0, model);
            return;
        }
    }

    particle->flags |= 8 << view->index;
    particle->alpha = 255.0f;
    ratio = particle->time / particle->life > 1.0f ? 1.0f : particle->time / particle->life;
    primFrom = particle->prim;
    envFrom = particle->env;
    primTo = desc->colors->prim;
    envTo = desc->colors->env;
    env[0] = CLAMP_U8(LERP(envTo[0], envFrom[0], ratio));
    env[1] = CLAMP_U8(LERP(envTo[1], envFrom[1], ratio));
    env[2] = CLAMP_U8(LERP(envTo[2], envFrom[2], ratio));
    prim[0] = CLAMP_U8(LERP(primTo[0], primFrom[0], ratio));
    prim[1] = CLAMP_U8(LERP(primTo[1], primFrom[1], ratio));
    prim[2] = CLAMP_U8(LERP(primTo[2], primFrom[2], ratio));

    mode = Particle_ColorMode(particle);
    Texture_GetInfo(model, &info);
    frame = particle->frame;
    time = particle->time;
    switch (desc->animMode) {
        case 0:
            if (frame >= info.count - 1) {
                time = particle->life;
                frame = info.count - 1;
                particle->time = time;
            }
            break;
        case 1:
            if (frame >= info.count) {
                frame = info.count - 1;
            }
            break;
        case 2:
            frame %= info.count;
            break;
        case 3:
            if (info.count * 2 - 2 > 0) {
                frame %= info.count * 2 - 2;
            } else {
                frame = 0;
            }
            if (frame >= info.count) {
                frame = (info.count * 2 - 2) - frame;
            }
            break;
        case 4:
            frame = Random_Next() % info.count;
            break;
        case 5:
            if (particle->randomFrame == -1) {
                particle->randomFrame = Random_Next() % info.count;
            }
            frame = particle->randomFrame;
            break;
    }

    if (!(particle->flags & 0x100000)) {
        if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 1) {
            ParticleView *camera = gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
            CVector3 local;
            f32 depth;
            CMtxF__TransformPoint(camera->viewMtx, &particle->inst.pos, &local);
            depth = local.z;
            if (depth < 0.0f) {
                depth = -depth;
            }
            CParticle__BuildMatrix(particle, &particle->mtx[gEvenOdd], camera, depth);
        } else {
            CParticle__BuildMatrix(particle, &particle->mtx[gEvenOdd], 0, 0.0f);
        }
        particle->builtFrame = frame;
        particle->flags |= 0x100000;
    }
    CMtxF__TransformPoint(view->viewMtx, &particle->inst.pos, &viewPos);
    dist = viewPos.z;
    if (dist < 0.0f) {
        dist = -dist;
    }
    if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 1) {
        GFX_CMD(0xDA380003, GBI_ADDR(&particle->mtx[gEvenOdd]))
    } else {
        mtx = MtxPool_Alloc(gScene.m_Heap, 1);
        if (mtx == 0) {
            CCache__Unlock(0, model);
            return;
        }
        CParticle__BuildMatrix(particle, mtx, view, dist);
        GFX_CMD(0xDA380003, GBI_ADDR(mtx))
    }

    textured = particle->flags & 0x800000;
    shiftS = 1 << (((desc->flags & 0x100) ? 7 : 6) + info.frame->shiftS);
    shiftT = 1 << (((desc->flags & 0x200) ? 7 : 6) + info.frame->shiftT);
    Texture_Load(&particle->model, model, frame, 0, shiftS, shiftT, 1, 1, 0);
    switch (info.frame->kind) {
        case 0:
        case 1:
            Render_SetCombineMode(0x18);
            break;
        case 3:
            Render_SetCombineMode(0x1D);
            break;
    }
    if (textured) {
        Render_SetRenderMode(0xF);
        GFX_CMD(0xD7FF0002, (shiftS << 16) | (shiftT & 0xFFFF))
    } else if ((desc->flags & 0x400) && desc->fade->unkE != 4) {
        Render_SetRenderMode(0xF);
    } else if (desc->flags & 0x4000) {
        Render_SetRenderMode(!(gEngineApp_m_Settings.cheatFlags & 0x200) ? 0xD : 0x11);
    } else {
        Render_SetRenderMode((gEngineApp_m_Settings.cheatFlags & 0x200) ? 0x12 : 0x13);
    }

    alpha = 0xFF;
    if ((desc->flags & 1) && particle->life > 0) {
        alpha = (particle->life - time) * 255.0f / particle->life;
    }
    duration = desc->fade->fadeIn;
    if (time < duration) {
        alpha = alpha * (time + 1.0f) / (duration + 1.0f);
    }
    {
        f32 left = particle->life - time;
        if ((left < 0.0f ? 0.0f : left) < desc->fade->fadeOut) {
            alpha = Particle_Scale(alpha * (particle->life - time), desc->fade->fadeOut + 1.0f);
        }
    }
    {
        f32 range = view->fadeRange;
        f32 half;
        f32 delta;
        switch (desc->fade->rangeMode) {
            case 2:
                alpha = alpha * (((range - dist) * 10.0f / range) > 1.0f ? 1.0f : ((range - dist) * 10.0f / range));
                break;
            case 0:
                alpha = alpha * (((range - dist) * 5.0f / range) > 1.0f ? 1.0f : ((range - dist) * 5.0f / range));
                break;
            case 1:
                half = range * 0.5f;
                delta = half - dist;
                alpha = alpha * (delta < 0.0f ? half + delta : half - delta) / half;
                break;
        }
    }

    if (info.frame->kind == 3) {
        switch (mode) {
            case 0:
            default:
                GFX_CMD(0xFB000000, (env[0] << 24) | (env[1] << 16) | (env[2] << 8) | 0xFF)
                break;
            case 2:
                GFX_CMD(0xFB000000, (env[0] << 24) | (env[0] << 16) | (env[2] << 8) | 0xFF)
                break;
            case 1:
                GFX_CMD(0xFB000000, (env[1] << 24) | (env[0] << 16) | (env[2] << 8) | 0xFF)
                break;
        }
    }
    alpha = (alpha > 255 ? 255 : alpha) < 0 ? 0 : (alpha > 255 ? 255 : alpha);
    GFX_CMD(0x01004008, GBI_ADDR(gParticleSpriteVtx))
    if (textured) {
        GFX_CMD(0xD7FF0002, (shiftS << 16) | (shiftT & 0xFFFF))
    }
    particle->alpha = (alpha * particle->opacity) >> 8;
    switch (mode) {
        case 0:
        default:
            GFX_CMD(0xFA000000, (prim[0] << 24) | (prim[1] << 16) | (prim[2] << 8) | ((u32)particle->alpha & 0xFF))
            break;
        case 2:
            GFX_CMD(0xFA000000, (prim[0] << 24) | (prim[0] << 16) | (prim[2] << 8) | ((u32)particle->alpha & 0xFF))
            break;
        case 1:
            GFX_CMD(0xFA000000, (prim[1] << 24) | (prim[0] << 16) | (prim[2] << 8) | ((u32)particle->alpha & 0xFF))
            break;
    }
    GFX_CMD(0x06000204, 0x00040600)
    CCache__Unlock(0, model);
}

s16 *CPlayer__GetFiringAmmo(void *player, s32 slot);

void CParticleSystem__Free(u8 *mgr, EffectActor *actor) {
    EffectActor *child;
    u8 *owner;
    u32 flags;
    u8 kind;

    flags = actor->flags;
    if (!(flags & 0x100)) {
        return;
    }
    if (!(flags & 0x800)) {
        if (actor->typeId == 0x68) {
#ifdef TARGET_PORT
            /* the owner's m_AI.m_pParent is its player: m_Data.m_AmmoList by member */
            owner = (u8 *) ((CGameObjectInstance *) actor->parent)->m_AI.m_pParent;
            ((CPlayer *) owner)->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(owner, 0x15)] = 1;
#else
            owner = *(u8 **) (actor->parent + 0x1A8);
            *(s16 *) (owner + *CPlayer__GetFiringAmmo(owner, 0x15) * 2 + 0x934) = 1;
#endif
        }
        child = actor->child;
        if (child != NULL && (kind = child->kind) == 2 && (child->flags & 0x100) &&
            (child->typeId == kind || child->typeId == 0x111)) {
            CParticleSystem__Free((u8 *) &gScene.m_ParticleSystem, child);
            CParticle__StopLoopSound(child);
        }
        CParticle__StopLoopSound(actor);
        if (actor->registered) {
            CParticle__BuildCameraMatrix(actor);
            Trail_DetachOwner(&gEngineApp_m_SwooshSystem, actor);
        }
        /* the light pointer-wide, in the host CParticle's member (the view has the N64 offset) */
        if (((CParticle *) actor)->m_pDLight != NULL) {
            CDLightSet__Free(&gScene.m_DLightSet, ((CParticle *) actor)->m_pDLight);
            ((CParticle *) actor)->m_pDLight = NULL;
        }
        if (actor->liveCount != NULL) {
            (*actor->liveCount)--;
        }
        actor->flags &= ~0x02000100;
        CList__Remove(actor->list, actor);
        actor->list = NULL;
        CList__AddHead(&((CParticleSystem *) mgr)->m_FreeList, actor);
        if (actor->flags & 0x01000000) {
            CList__Remove(&((CParticleSystem *) mgr)->m_DrawList, actor);
        }
    }
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_8.CPP, func_802800C0_de, with Turok 2's layouts. */

/* Spawns the particles that template block 2 of the particle system lists for
 * a kind: for each template in the kind's range it filters by player mask,
 * existing owner particles, distance to the current player, mode, team flags
 * and a percent chance, builds a spawn matrix (source matrix, mirror,
 * translation or rotation), then for a random count allocates particles, fills
 * their timing, spread, colour, render and direction fields from the
 * template's packed floats, links them into their group, aims shots at the
 * last collision target for kinds 0x2D and 0x4F, and plays their sound.
 * Returns the number spawned. */

/* Two half-floats: base and random range (see RandomInPackedRange). */
typedef struct {
    /* 0x0 */ u16 base;
    /* 0x2 */ u16 range;
} PackedRange;
typedef struct {
    /* 0x00 */ s16 lifeBase;
    /* 0x02 */ s16 lifeRange;
    /* 0x04 */ s8 chance;
    /* 0x05 */ s8 group;
    /* 0x06 */ s8 countBase;
    /* 0x07 */ s8 countRange;
    /* 0x08 */ u8 listIndex;
    /* 0x09 */ u8 pad9[2];
    /* 0x0B */ s8 unkB;
    /* 0x0C */ u8 padC;
    /* 0x0D */ s8 spread;
    /* 0x0E */ s8 mode;
    /* 0x0F */ u8 event;
} EntryParams;
typedef struct {
    /* 0x00 */ u8 rgb0[3];
    /* 0x03 */ u8 rgb1[3];
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ s8 hueRange;
    /* 0x0D */ s8 satRange;
    /* 0x0E */ s8 valRange;
} EntryColor;
typedef struct {
    /* 0x00 */ PackedRange size;
    /* 0x04 */ PackedRange speed;
    /* 0x08 */ u16 unk8[4];
} EntryMotion;
typedef struct {
    /* 0x00 */ u8 pad0[0x94];
    /* 0x94 */ u16 type;
} EntryModel;
#ifdef TARGET_PORT
typedef struct Entry {
/* the view at the host offsets of CROMParticleEffect (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Entry(m) __builtin_offsetof(CROMParticleEffect, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_Entry(m_dwFlags)]; u32 flags; }; /* N64 0x0 */
        struct { u8 _hp_unk4[T2_HOFF_Entry(unk_04)]; s16 unk4; }; /* N64 0x4 */
        struct { u8 _hp_unk6[T2_HOFF_Entry(unk_06)]; s8 unk6; }; /* N64 0x6 */
        struct { u8 _hp_unk7[T2_HOFF_Entry(m_DLightType)]; s8 unk7; }; /* N64 0x7 */
        struct { u8 _hp_unk8[T2_HOFF_Entry(m_AnimMode)]; s8 unk8; }; /* N64 0x8 */
        struct { u8 _hp_unk9[T2_HOFF_Entry(unk_09)]; u8 unk9; }; /* N64 0x9 */
        struct { u8 _hp_unkA[T2_HOFF_Entry(m_bClampToGround)]; u8 unkA; }; /* N64 0xA */
        struct { u8 _hp_unkB[T2_HOFF_Entry(unk_0B)]; u8 unkB; }; /* N64 0xB */
        struct { u8 _hp_sound[T2_HOFF_Entry(m_nSound)]; s16 sound; }; /* N64 0xC */
        struct { u8 _hp_playerMask[T2_HOFF_Entry(m_PlayerMask)]; u8 playerMask; }; /* N64 0x10 */
        struct { u8 _hp_unk14[T2_HOFF_Entry(m_pModel)]; sptr unk14; }; /* N64 0x14 */
        struct { u8 _hp_model[T2_HOFF_Entry(m_pImpact)]; EntryModel *model; }; /* N64 0x18 */
        struct { u8 _hp_offset[T2_HOFF_Entry(m_pOffset)]; PackedRange *offset; }; /* N64 0x1C */
        struct { u8 _hp_unk20[T2_HOFF_Entry(m_pRotation)]; PackedRange *unk20; }; /* N64 0x20 */
        struct { u8 _hp_unk24[T2_HOFF_Entry(m_pDistanceRange)]; PackedRange *unk24; }; /* N64 0x24 */
        struct { u8 _hp_direction[T2_HOFF_Entry(m_pDirection)]; PackedRange *direction; }; /* N64 0x28 */
        struct { u8 _hp_unk2C[T2_HOFF_Entry(m_pOscillation)]; PackedRange *unk2C; }; /* N64 0x2C */
        struct { u8 _hp_motion[T2_HOFF_Entry(m_pMotion)]; EntryMotion *motion; }; /* N64 0x30 */
        struct { u8 _hp_color[T2_HOFF_Entry(m_pColors)]; EntryColor *color; }; /* N64 0x34 */
        struct { u8 _hp_params[T2_HOFF_Entry(m_pParams)]; EntryParams *params; }; /* N64 0x38 */
        u8 _hp_size[sizeof(CROMParticleEffect)];
    };
} Entry;
#else
typedef struct Entry {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ s16 sound;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ u8 playerMask;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ EntryModel *model;
    /* 0x1C */ PackedRange *offset;
    /* 0x20 */ PackedRange *unk20;
    /* 0x24 */ PackedRange *unk24;
    /* 0x28 */ PackedRange *direction;
    /* 0x2C */ PackedRange *unk2C;
    /* 0x30 */ EntryMotion *motion;
    /* 0x34 */ EntryColor *color;
    /* 0x38 */ EntryParams *params;
} Entry;
#endif /* 0x3C bytes */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ Entry entries[1];
} EntryBlock;
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
} Render;
#ifdef TARGET_PORT
typedef struct Actor {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Actor(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_type[T2_HOFF_Actor(header.m_Type)]; u8 type; }; /* N64 0x0 */
        struct { u8 _hp_pos[T2_HOFF_Actor(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_def[T2_HOFF_Actor(header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_flags[T2_HOFF_Actor(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_matrices[T2_HOFF_Actor(m_mtxDraw)]; CMtxF matrices[1]; }; /* N64 0x48 */
        struct { u8 _hp_bones[T2_HOFF_Actor(m_mtxDraw) + 80]; CMtxF *bones; }; /* N64 0x98 */
        struct { u8 _hp_model[T2_HOFF_Actor(m_pEffect)]; CROMParticleEffect *model; }; /* N64 0xF8 */
        struct { u8 _hp_unk130[T2_HOFF_Actor(m_vSize.y)]; f32 unk130; }; /* N64 0x130 */
        struct { u8 _hp_unk134[T2_HOFF_Actor(m_vSize.z)]; f32 unk134; }; /* N64 0x134 */
        struct { u8 _hp_ai[T2_HOFF_Actor(m_vGrowth.z)]; s32 ai; }; /* N64 0x140 */
    };
} Actor;
#else
typedef struct Actor {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0x40 - 0x18];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[4];
    /* 0x048 */ CMtxF matrices[1];
    /* 0x088 */ u8 pad88[0x98 - 0x88];
    /* 0x098 */ CMtxF *bones;
    /* 0x09C */ u8 pad9C[0xF8 - 0x9C];
    /* 0x0F8 */ CROMParticleEffect *model;
    /* 0x0FC */ u8 padFC[0x130 - 0xFC];
    /* 0x130 */ f32 unk130;
    /* 0x134 */ f32 unk134;
    /* 0x138 */ u8 pad138[0x140 - 0x138];
    /* 0x140 */ s32 ai;
} Actor;
#endif

typedef struct Owner {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1[0x7F];
    /* 0x80 */ void *ownerId;
} Owner;
#ifdef TARGET_PORT
typedef struct Effect {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Effect(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_kind[T2_HOFF_Effect(header.m_nObjType)]; u16 kind; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_Effect(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_region[T2_HOFF_Effect(header.m_pCurrentRegion)]; void *region; }; /* N64 0x10 */
        struct { u8 _hp_velocity[T2_HOFF_Effect(m_vVelocity)]; CVector3 velocity; }; /* N64 0x18 */
        struct { u8 _hp_unk24[T2_HOFF_Effect(unk_024)]; s32 unk24; }; /* N64 0x24 */
        struct { u8 _hp_unk28[T2_HOFF_Effect(unk_024[4])]; s32 unk28; }; /* N64 0x28 */
        struct { u8 _hp_unk2C[T2_HOFF_Effect(unk_024[8])]; s32 unk2C; }; /* N64 0x2C */
        struct { u8 _hp_unk30[T2_HOFF_Effect(unk_024[12])]; s32 unk30; }; /* N64 0x30 */
        struct { u8 _hp_attachPos[T2_HOFF_Effect(m_vAttachPos)]; CVector3 attachPos; }; /* N64 0x34 */
        struct { u8 _hp_flags[T2_HOFF_Effect(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_entry[T2_HOFF_Effect(m_pEffect)]; Entry *entry; }; /* N64 0xF8 */
        struct { u8 _hp_bits[T2_HOFF_Effect(unk_0FC)]; u32 bits; }; /* N64 0xFC */
        struct { u8 _hp_ownerId[T2_HOFF_Effect(m_pOwnerID)]; void *ownerId; }; /* N64 0x104 */
        struct { u8 _hp_source[T2_HOFF_Effect(m_pSource)]; Actor *source; }; /* N64 0x108 */
        struct { u8 _hp_owner[T2_HOFF_Effect(m_pOwner)]; Actor *owner; }; /* N64 0x10C */
        struct { u8 _hp_refCount[T2_HOFF_Effect(m_pOwnerCount)]; s32 *refCount; }; /* N64 0x110 */
        struct { u8 _hp_target[T2_HOFF_Effect(m_pTarget)]; sptr target; }; /* N64 0x114 */
        struct { u8 _hp_sound[T2_HOFF_Effect(m_pDLight)]; s32 sound; }; /* N64 0x118 */
        struct { u8 _hp_spread[T2_HOFF_Effect(m_cFrame)]; f32 spread; }; /* N64 0x11C */
        struct { u8 _hp_age[T2_HOFF_Effect(m_AnimFrame)]; f32 age; }; /* N64 0x120 */
        struct { u8 _hp_unk124[T2_HOFF_Effect(m_AnimSpeed)]; f32 unk124; }; /* N64 0x124 */
        struct { u8 _hp_life[T2_HOFF_Effect(m_nFrames)]; s16 life; }; /* N64 0x128 */
        struct { u8 _hp_unk12A[T2_HOFF_Effect(m_BuiltFrame)]; u8 unk12A; }; /* N64 0x12A */
        struct { u8 _hp_unk12B[T2_HOFF_Effect(m_RandomFrame)]; s8 unk12B; }; /* N64 0x12B */
        struct { u8 _hp_unk12C[T2_HOFF_Effect(m_vSize)]; f32 unk12C[6]; }; /* N64 0x12C: m_vSize, m_vGrowth */
        struct { u8 _hp_startPos[T2_HOFF_Effect(m_vLastPos)]; CVector3 startPos; }; /* N64 0x144 */
        struct { u8 _hp_direction[T2_HOFF_Effect(m_vDirection)]; CVector3 direction; }; /* N64 0x150 */
        struct { u8 _hp_unk15C[T2_HOFF_Effect(m_vRot)]; f32 unk15C[6]; }; /* N64 0x15C: m_vRot, m_vRotSpeed */
        struct { u8 _hp_alpha[T2_HOFF_Effect(m_Alpha)]; f32 alpha; }; /* N64 0x174 */
        struct { u8 _hp_rot[T2_HOFF_Effect(m_WobblePhase)]; f32 rot[6]; }; /* N64 0x178: the scalars to 0x190 */
        struct { u8 _hp_render[T2_HOFF_Effect(m_dwHitFlags)]; Render render; }; /* N64 0x190: the scalars to 0x1AC */
        struct { u8 _hp_mode[T2_HOFF_Effect(m_Alignment)]; s8 mode; }; /* N64 0x1AC */
        struct { u8 _hp_shake[T2_HOFF_Effect(m_Shake)]; u8 shake; }; /* N64 0x1AD */
        struct { u8 _hp_rgb0[T2_HOFF_Effect(m_PrimColor)]; u8 rgb0[3]; }; /* N64 0x1AE */
        struct { u8 _hp_rgb1[T2_HOFF_Effect(m_EnvColor)]; u8 rgb1[3]; }; /* N64 0x1B1 */
        struct { u8 _hp_alphaByte[T2_HOFF_Effect(m_Opacity)]; u8 alphaByte; }; /* N64 0x1B4 */
        struct { u8 _hp_unk1B5[T2_HOFF_Effect(m_ActiveSwooshes)]; u8 unk1B5; }; /* N64 0x1B5 */
        struct { u8 _hp_unk1BC[T2_HOFF_Effect(m_Scale)]; f32 unk1BC; }; /* N64 0x1BC */
        struct { u8 _hp_list[T2_HOFF_Effect(m_pActiveList)]; void *list; }; /* N64 0x1C0 */
        struct { u8 _hp_next[T2_HOFF_Effect(m_pNext)]; struct Effect *next; }; /* N64 0x1C8 */
        struct { u8 _hp_unk1CC[T2_HOFF_Effect(m_pDrawLast)]; s32 unk1CC; }; /* N64 0x1CC */
        struct { u8 _hp_groupNext[T2_HOFF_Effect(m_pDrawNext)]; struct Effect *groupNext; }; /* N64 0x1D0 */
    };
} Effect;
#else
typedef struct Effect {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 kind;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[4];
    /* 0x018 */ CVector3 velocity;
    /* 0x024 */ s32 unk24;
    /* 0x028 */ s32 unk28;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ s32 unk30;
    /* 0x034 */ CVector3 attachPos;
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ Entry *entry;
    /* 0x0FC */ u32 bits;
    /* 0x100 */ u8 pad100[4];
    /* 0x104 */ void *ownerId;
    /* 0x108 */ Actor *source;
    /* 0x10C */ Actor *owner;
    /* 0x110 */ s32 *refCount;
    /* 0x114 */ s32 target;
    /* 0x118 */ s32 sound;
    /* 0x11C */ f32 spread;
    /* 0x120 */ f32 age;
    /* 0x124 */ f32 unk124;
    /* 0x128 */ s16 life;
    /* 0x12A */ u8 unk12A;
    /* 0x12B */ s8 unk12B;
    /* 0x12C */ f32 unk12C[6];
    /* 0x144 */ CVector3 startPos;
    /* 0x150 */ CVector3 direction;
    /* 0x15C */ f32 unk15C[6];
    /* 0x174 */ f32 alpha;
    /* 0x178 */ f32 rot[6];
    /* 0x190 */ Render render;
    /* 0x1AC */ s8 mode;
    /* 0x1AD */ u8 shake;
    /* 0x1AE */ u8 rgb0[3];
    /* 0x1B1 */ u8 rgb1[3];
    /* 0x1B4 */ u8 alphaByte;
    /* 0x1B5 */ u8 unk1B5;
    /* 0x1B6 */ u8 pad1B6[0x1BC - 0x1B6];
    /* 0x1BC */ f32 unk1BC;
    /* 0x1C0 */ void *list;
    /* 0x1C4 */ u8 pad1C4[4];
    /* 0x1C8 */ struct Effect *next;
    /* 0x1CC */ s32 unk1CC;
    /* 0x1D0 */ struct Effect *groupNext;
} Effect;
#endif
#ifdef TARGET_PORT
/* the Effect view's spans over runs of CParticle scalars: contiguous on the host too */
_Static_assert(__builtin_offsetof(CParticle, m_vGrowth) == __builtin_offsetof(CParticle, m_vSize) + 12, "m_vSize/m_vGrowth");
_Static_assert(__builtin_offsetof(CParticle, m_vRotSpeed) == __builtin_offsetof(CParticle, m_vRot) + 12, "m_vRot/m_vRotSpeed");
_Static_assert(__builtin_offsetof(CParticle, m_dwHitFlags) == __builtin_offsetof(CParticle, m_WobblePhase) + 0x18, "rot[6]");
_Static_assert(__builtin_offsetof(CParticle, m_Alignment) == __builtin_offsetof(CParticle, m_dwHitFlags) + sizeof(Render), "render");
_Static_assert(sizeof(Entry) == sizeof(CROMParticleEffect), "Entry");
_Static_assert(sizeof(ParticleHead) == sizeof(CObjectHead), "ParticleHead");
#endif

extern s32 BinaryRange(Entry *entries, s32 count, s32 kind, s32 *first, s32 *last);
extern void CScene__BroadcastNoise(void *, s32, CVector3 *);
extern void CMtxF__SetScale(CMtxF *m, f32 x, f32 y, f32 z);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar) */
#define GameObject_GetAimRotation GameObject_GetAimRotation_sret
#endif

extern void GameObject_GetAimRotation(CQuaternion *q, Owner *owner, CVector3 pos);
extern void Material_GetTextureSet(u32 *bits, s32 value);
extern f32 RandomInPackedRange(PackedRange r);
extern void Color_ShiftHue(s8, u8, u8, u8, u8 *, u8 *, u8 *);
extern void Color_Saturate(u8, u8, u8, u8, u8 *, u8 *, u8 *);
extern void Color_Darken(u8, u8, u8, u8, u8 *, u8 *, u8 *);
void Trail_AddNode(void *list, void *owner, s32 arg2);
extern void *DLight_Spawn(void *, s32);
void CParticle__StartLoopSound(CParticle *arg0, s32 arg1);
extern void Sound__Play(s16 sound, CVector3 pos, sptr a, sptr b);

static __inline__ s32 randomCount(s32 range) {
    return range != 0 ? Random_Next() % (range + 1) : 0;
}

static __inline__ Effect *findOwned(CList *list, void *ownerId, Entry *entry) {
    Effect *effect;

    effect = list->m_pHead;
    if (entry != 0) {
        for (; effect != 0; effect = effect->next) {
            if (effect->ownerId == ownerId && effect->entry == entry) {
                return effect;
            }
        }
    } else {
        for (; effect != 0; effect = effect->next) {
            if (effect->ownerId == ownerId) {
                return effect;
            }
        }
    }
    return 0;
}

static __inline__ Effect *allocateEffect(CParticleSystem *sys, u8 index) {
    Effect *effect;
    CList *list;
    s32 i;

    if (sys->m_FreeList.m_pHead == 0) {
        for (i = 0; i <= index; i++) {
            if ((&sys->m_ActiveList[i])->m_pTail != 0) {
                CParticleSystem__Free(sys, (&sys->m_ActiveList[i])->m_pTail);
                break;
            }
        }
    }
    effect = sys->m_FreeList.m_pHead;
    if (effect == 0) {
        return 0;
    }
    CList__Remove(&sys->m_FreeList, effect);
    list = &sys->m_ActiveList[index];
    CList__AddHead(list, effect);
    effect->list = list;
    effect->groupNext = 0;
    effect->unk1CC = 0;
    effect->flags = 0x100;
    return effect;
}

/* The host aimEffect: the hit target is a CGameObjectInstance (bones, intelligence, AI by member). */
static __inline__ void aimEffect(Effect *effect, CVector3 *vel) {
    CMtxF targetMtx;
    CMtxF targetInv;
    CVector3 up;
    CGameObjectInstance *targetActor;
    s32 targetTeam;
    CCollisionResults *scene;

    if (gCollisionResults.m_nInstanceNode != -1) {
        effect->mode = -4;
        targetActor = (CGameObjectInstance *) gCollisionResults.m_pInstance;
        targetTeam = gCollisionResults.m_Frame.action;
        CMtxF__FromMtxAffine(&targetMtx, (CMtxF *) ((u8 *) targetActor->m_pmtDrawMtxs + gCollisionResults.m_nInstanceNode * 0x40));
        CMtxF__InvertAffine(&targetInv, &targetMtx);
        scene = &gCollisionResults;
        CMtxF__TransformPoint(&targetInv, &scene->m_vInstancePos, &effect->attachPos);
        up = gCollisionResults.m_vVelocity;
        CVector3__Normalize(&up);
        CMtxF__RotateVector(&targetInv, &up, vel);
        effect->target = (sptr) targetActor;
        effect->flags |= 0x10000;
        effect->shake = gCollisionResults.m_nInstanceNode;
        if (targetActor->header.m_pIntelligence->m_Type == 1) {
            if (effect->owner != 0) {
                AI_HearNoise((Actor *) targetActor, (s32 *) &targetActor->m_AI, &effect->owner->pos, 3);
            } else {
                AI_HearNoise((Actor *) targetActor, (s32 *) &targetActor->m_AI, &scene->m_vInstancePos, 3);
            }
        }
        if (effect->kind == 0x56 && targetTeam == 2) {
            effect->flags |= 0x4000000;
        }
    }
}

/* sys, source, owner: void * as protos/game.h declares them, read through casts (typed locals moved registers). */
s32 CParticleSystem__Spawn(void *sys, void *source, void *owner, s32 *refCount, sptr arg4, s32 kind,
                  CVector3 arg6, CQuaternion rotation, CVector3 pos, void *region, s32 arg10, s32 arg11) {
    CMtxF mtx;
    CVector3 dir;
    CVector3 dirOut;
    CVector3 offset;
    CVector3 offsetOut;
    CVector3 delta;
    CQuaternion quat;
    CCollisionResults target;
    CVector3 aimDelta;
    s32 first;
    s32 last;
    u8 h, sv, v;
    u8 r, g, b;
    u8 r0, g0, b0;
    u8 r1, g1, b1;
    s32 count;
    s32 j;
    s32 n;
    s32 mode;
    s32 i;
    Actor *anchor;
    void *ownerId;
    s32 extra;
    s32 life;
    s32 layer;
    s32 ok;
    s32 hue;
    u8 sat;
    u8 val;
    s32 spread;
    u32 *bits;
    u8 mask;
    void *resource;
    EntryBlock *block;
    s32 numEntries;
    Entry *templates;
    Entry *entries;
    Entry *entry;
    Effect *effect;
    Render *render;
    CVector3 *vel;
    CCamera *camera;
    f32 distSq;
    f32 scale;
    f32 depth;
    s32 sound;
    s32 *global = &gEngineApp_m_Settings.paused;

    if (global[0] != 0) {
        return 0;
    }
    count = 0;
    resource = ((CParticleSystem *) sys)->m_pParticleData;
    gCollisionResultsP = &target;
    block = CISet__GetBlock(resource, 1);
    numEntries = block->count;
    templates = block->entries;
    if (kind == 0xFC) {
        CScene__Blast(&gScene, &pos);
    }
    if (BinaryRange(templates, numEntries, kind, &first, &last) != 0) {
#ifdef TARGET_PORT
        /* the host's copy of the templates (full-width pointers: port/game/t2particles.c) */
        entries = (Entry *) T2Port_ParticleEffects(resource, NULL);
        if (entries == NULL) {
            gCollisionResultsP = &gCollisionResults;
            return 0;
        }
#else
        entries = ((EntryBlock *) CISet__GetBlock(resource, 2))->entries;
#endif
        anchor = 0;
        /* The current player list, gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead, addressed from gEngineApp_m_Settings_paused. */
#ifdef TARGET_PORT
        if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead != 0) {
            anchor = (Actor *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        }
#else
        if (((Actor **) global)[-0x505] != 0) {
            anchor = ((Actor **) global)[-0x505];
        }
#endif
        ownerId = 0;
        if (owner != 0) {
            switch (((Owner *) owner)->type) {
                case 1:
                case 2:
                    ownerId = owner;
                    break;
                case 0:
                    /* type 0 (a static object): its id word at N64 0x80 */
                    ownerId = (void *) (sptr) ((CStaticObject *) owner)->m_LitKey;
                    owner = 0;
                    break;
            }
        }
        mask = 1;
        if (gEngineApp_m_Settings.multiplayer != 0) {
            mask = 2;
        }
        for (i = first; i <= last; i++) {
            entry = &entries[i];
            ok = (entry->playerMask & mask) != 0;
            if ((entry->flags & 0x80) && ownerId != 0) {
                if (findOwned(&((CParticleSystem *) sys)->m_ActiveList[(s8) entry->params->listIndex], ownerId, entry) != 0) {
                    ok = 0;
                }
            }
            if (anchor != 0) {
                CVector3__Subtract(&delta, &pos, &anchor->pos);
            } else {
                delta.x = delta.y = delta.z = 0.0f;
            }
            distSq = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (entry->flags & 0x400000) {
                if (262144.0f <= distSq) {
                    ok = 0;
                }
            } else if (entry->flags & 0x200000) {
                if (distSq <= 262144.0f) {
                    ok = 0;
                }
            }
            switch (entry->params->mode) {
                case 6:
                    mode = -7;
                    break;
                case 5:
                    mode = -6;
                    break;
                case 0:
                    mode = -4;
                    break;
                case 1:
                    mode = -5;
                    break;
                default:
                    switch (arg10) {
                        case -5:
                        case -3:
                        case -2:
                            mode = arg10;
                            break;
                        default:
                            mode = -3;
                            break;
                    }
                    break;
                case 3:
                    if (0.0f <= (f32) arg10) {
                        mode = arg10;
                    } else {
                        ok = 0;
                        mode = 0;
                    }
                    break;
                case 4:
                    mode = arg10;
                    break;
            }
            if (entry->flags & 0x2000000) {
                if (gEngineApp_m_Settings.unk17 == 0 || !(arg11 & 1)) {
                    ok = 0;
                }
            }
            if (entry->flags & 0x4000000) {
                if (gEngineApp_m_Settings.unk17 != 0) {
                    ok = 0;
                }
            }
            if (entry->params->chance != 100) {
                if (Random_Next() % 1000 + 1 > entry->params->chance * 10) {
                    ok = 0;
                }
            }
            if (!ok) {
                continue;
            }
            if (entry->params->event != 0) {
                CScene__BroadcastNoise(&gScene, entry->params->event, &pos);
            }
            extra = 0;
            if (entry->flags & 0x8000) {
                switch (((Actor *) source)->type) {
                    case 2:
                        if (!(((Actor *) source)->flags & 0x100000)) {
                            if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 1) {
                                camera = gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
                                CMtxF__TransformPoint(&camera->m_mfView, &((Actor *) source)->pos, &aimDelta);
                                depth = aimDelta.z;
                                if (depth < 0.0f) {
                                    depth = -depth;
                                }
                                CParticle__BuildMatrix(source, &((Actor *) source)->matrices[gEvenOdd], camera, depth);
                            } else {
                                CParticle__BuildMatrix(source, &((Actor *) source)->matrices[gEvenOdd], 0, 0.0f);
                            }
                            ((Actor *) source)->flags |= 0x100000;
                        }
                        CMtxF__FromMtxAffine(&mtx, &((Actor *) source)->matrices[gEvenOdd]);
                        scale = 0.09765625f;
                        if (((Actor *) source)->model->m_pModel != 0) {
                            scale = 2.857143f;
                        }
                        CMtxF__ScaleAxes(&mtx, scale, scale, scale);
                        if (((Actor *) source)->model->m_dwFlags & 8) {
                            CMtxF__Translate(&mtx, 0.0f, -((Actor *) source)->unk130 * ((Actor *) source)->unk134, 0.0f);
                        }
                        break;
                    case 1:
                        extra = 0x400000;
                        CMtxF__SetScale(&mtx, -1.0f, 1.0f, -1.0f);
                        break;
                    default:
                        CMtxF__SetTranslate(&mtx, ((Actor *) source)->pos.x, ((Actor *) source)->pos.y, ((Actor *) source)->pos.z);
                        break;
                }
            } else if (entry->flags & 0x1000000) {
                CMtxF__Identity(&mtx);
            } else {
                if (entry->flags & 0x40000) {
                    GameObject_GetAimRotation(&quat, owner, pos);
                } else {
                    quat = rotation;
                }
                CQuaternion__ToMatrix(&quat, &mtx);
            }
            n = entry->params->countBase + randomCount(entry->params->countRange);
            for (j = 0; j < n; j++) {
                life = entry->params->lifeBase + randomCount(entry->params->lifeRange);
                if (life == 0) {
                    continue;
                }
                effect = allocateEffect(sys, entry->params->listIndex);
                if (effect == 0) {
                    break;
                }
                count++;
                Material_GetTextureSet(&effect->bits, entry->unk4);
                effect->velocity.x = 0;
                effect->velocity.y = 0;
                effect->velocity.z = 0;
                effect->unk24 = 0;
                effect->unk28 = 0;
                effect->unk2C = 0;
                effect->unk30 = 0;
                effect->kind = kind;
                effect->entry = entry;
                effect->mode = mode;
                if (((Actor *) source)->type == 2) {
                    effect->source = source;
                } else {
                    effect->source = 0;
                }
                effect->ownerId = ownerId;
                effect->owner = (Actor *) owner;
                effect->refCount = refCount;
                if (refCount != 0) {
                    (*refCount)++;
                }
                effect->target = arg4;
                effect->flags = (effect->flags & ~0xE600) | (arg11 | extra);
                bits = &effect->bits;
                layer = 0;
                if (effect->entry->flags & 0x2000000) {
                    if (effect->flags & 2) {
                        layer = 2;
                    } else if (gEngineApp_m_Settings.unk17 == 2) {
                        layer = 1;
                    }
                }
                *bits |= layer << 30;
                effect->unk1B5 = 0;
                spread = entry->params->spread;
                if (spread < 0) {
                    effect->spread = spread;
                } else {
                    effect->spread = -(f32) randomCount(spread);
                }
                effect->rot[0] = RandomInPackedRange(entry->unk2C[2]) * 6.2831855f;
                effect->rot[2] = RandomInPackedRange(entry->unk2C[1]) * 0.41887906f;
                effect->rot[4] = RandomInPackedRange(entry->unk2C[0]);
                effect->rot[1] = RandomInPackedRange(entry->unk2C[5]) * 6.2831855f;
                effect->rot[3] = RandomInPackedRange(entry->unk2C[4]) * 0.41887906f;
                effect->rot[5] = RandomInPackedRange(entry->unk2C[3]);
                if (entry->flags & 2) {
                    effect->age = randomCount(life);
                } else {
                    effect->age = 0.0f;
                }
                effect->unk124 = entry->params->unkB;
                effect->life = life;
                effect->unk12A = 0;
                effect->unk12B = -1;
                dir.x = RandomInPackedRange(entry->direction[0]);
                dir.y = RandomInPackedRange(entry->direction[1]);
                dir.z = RandomInPackedRange(entry->direction[2]);
                CVector3__Normalize(&dir);
                if (dir.x != 0.0f || dir.y != 0.0f || dir.z != 0.0f) {
                    CMtxF__TransformPoint(&mtx, &dir, &dirOut);
                } else {
                    dirOut = dir;
                }
                if (mode == -8) {
                    effect->direction = gCollisionResults.m_vNormal;
                    CVector3__Normalize(&effect->direction);
                } else {
                    effect->direction = dirOut;
                }
                effect->unk12C[0] = RandomInPackedRange(entry->unk24[0]);
                effect->unk12C[1] = RandomInPackedRange(entry->unk24[2]);
                effect->unk12C[2] = RandomInPackedRange(entry->unk24[4]);
                effect->unk12C[3] = RandomInPackedRange(entry->unk24[1]);
                effect->unk12C[4] = RandomInPackedRange(entry->unk24[3]);
                effect->unk12C[5] = RandomInPackedRange(entry->unk24[5]);
                /* Both branches are the same (as in Rage Wars); GCC merges them after loop
                 * optimisation, whose decisions still see both copies. */
                if (entry->unk14 != 0) {
                    effect->unk15C[0] = RandomInPackedRange(entry->unk20[0]);
                    effect->unk15C[1] = RandomInPackedRange(entry->unk20[2]);
                    effect->unk15C[2] = RandomInPackedRange(entry->unk20[4]);
                    effect->unk15C[3] = RandomInPackedRange(entry->unk20[1]);
                    effect->unk15C[4] = RandomInPackedRange(entry->unk20[3]);
                    effect->unk15C[5] = RandomInPackedRange(entry->unk20[5]);
                } else {
                    effect->unk15C[0] = RandomInPackedRange(entry->unk20[0]);
                    effect->unk15C[1] = RandomInPackedRange(entry->unk20[2]);
                    effect->unk15C[2] = RandomInPackedRange(entry->unk20[4]);
                    effect->unk15C[3] = RandomInPackedRange(entry->unk20[1]);
                    effect->unk15C[4] = RandomInPackedRange(entry->unk20[3]);
                    effect->unk15C[5] = RandomInPackedRange(entry->unk20[5]);
                }
                if ((entry->flags & 0x80000) && entry->unk8 == 1) {
                    effect->alphaByte = 0;
                    effect->alpha = 0.0f;
                } else {
                    effect->alphaByte = 0xFF;
                    effect->alpha = 255.0f;
                }
                effect->region = region;
                effect->pos = pos;
                offset.x = RandomInPackedRange(entry->offset[0]);
                offset.y = RandomInPackedRange(entry->offset[1]);
                offset.z = RandomInPackedRange(entry->offset[2]);
                if (offset.x != 0.0f || offset.y != 0.0f || offset.z != 0.0f) {
                    CMtxF__TransformPoint(&mtx, &offset, &offsetOut);
                    if (!(entry->flags & 0x8000)) {
                        CVector3__Add(&offsetOut, &pos, &offsetOut);
                    }
                    if (effect->region != 0) {
                        GameObject_TraceMove(effect, offsetOut, &effect->pos, &effect->region, (s8) entry->unkB,
                                      (s8) entry->unk9);
                    } else {
                        effect->pos = offsetOut;
                    }
                }
                effect->startPos = effect->pos;
                effect->attachPos = effect->pos;
                CVector3__Scale(&effect->velocity, &dirOut, RandomInPackedRange(entry->motion->speed));
                if (entry->flags & 0x20) {
                    CVector3__Add(&effect->velocity, &effect->velocity, &arg6);
                }
                hue = 0;
                if (entry->color->hueRange != 0) {
                    hue = Random_Next() % (entry->color->hueRange * 2) - (u8) entry->color->hueRange;
                }
                sat = entry->color->satRange != 0 ? Random_Next() % entry->color->satRange : 0;
                val = entry->color->valRange != 0 ? Random_Next() % entry->color->valRange : 0;
                r = entry->color->rgb0[0];
                g = entry->color->rgb0[1];
                b = entry->color->rgb0[2];
                Color_ShiftHue(hue, r, g, b, &h, &sv, &v);
                Color_Saturate(sat, h, sv, v, &r, &g, &b);
                Color_Darken(val, r, g, b, &r0, &g0, &b0);
                r = entry->color->rgb1[0];
                g = entry->color->rgb1[1];
                b = entry->color->rgb1[2];
                Color_ShiftHue(hue, r, g, b, &h, &sv, &v);
                Color_Saturate(sat, h, sv, v, &r, &g, &b);
                Color_Darken(val, r, g, b, &r1, &g1, &b1);
                effect->rgb0[0] = r0;
                effect->rgb0[1] = g0;
                effect->rgb0[2] = b0;
                effect->rgb1[0] = r1;
                effect->rgb1[1] = g1;
                effect->rgb1[2] = b1;
                render = &effect->render;
                *render = *(Render *) &gCollisionInfoParticle;
                render->unk5 = entry->unkA;
                render->unk4 = entry->unk9;
                render->unk6 = entry->unkB;
                render->unk8 = RandomInPackedRange(entry->motion->size);
                render->unkC = HalfToFloat(entry->motion->unk8[0]);
                render->unk10 = HalfToFloat(entry->motion->unk8[1]);
                render->unk14 = HalfToFloat(entry->motion->unk8[2]);
                render->unk18 = HalfToFloat(entry->motion->unk8[3]);
                if (kind == 0x101 || kind == 0x128) {
                    render->flags |= 4;
                }
                if (entry->flags & 0x800) {
                    render->flags |= 0x800000;
                }
                if (entry->flags & 0x40000000) {
                    render->flags |= 0x1000000;
                }
                if (entry->params->group != 3) {
                    Effect *other;

                    for (other = ((CParticleSystem *) sys)->m_DrawList.m_pHead; other != 0; other = other->groupNext) {
                        if (other->entry == effect->entry) {
                            break;
                        }
                    }
                    if (other != 0) {
                        CList__InsertBefore(&((CParticleSystem *) sys)->m_DrawList, other, effect);
                    } else if (effect->entry->flags & 0x2000) {
                        CList__AddHead(&((CParticleSystem *) sys)->m_DrawList, effect);
                    } else {
                        CList__AddTail(&((CParticleSystem *) sys)->m_DrawList, effect);
                    }
                    effect->flags |= 0x1000000;
                }
#ifdef TARGET_PORT
                /* type is the impact's m_ImpactEventType[4] (the extra effect), 0xFFFF (none) for
                 * most effects; CParticle__HandleImpactEvent checks for 0xFFFF, this does not. The N64
                 * then reads 0x7FFF8 bytes past the 34-entry table, RAM 0x8012D07C: 0x00C800FF in
                 * every reference dump (flags & 1 set: the particle's hit flags get 0x80000000, the
                 * enlarged node boxes of Sweep_TestBox). The host read went 1 MB past the table into
                 * host memory, so a shot hit the boss on macOS and missed on Linux (round 10 y4). */
                if (((entry->model->type < sizeof(gImpactEventHandlers) / sizeof(gImpactEventHandlers[0])
                          ? gImpactEventHandlers[entry->model->type].flags
                          : (entry->model->type == 0xFFFF ? 0x00C800FFu : 0u)) & 1) || kind == 0x68) {
#else
                if ((gImpactEventHandlers[entry->model->type].flags & 1) || kind == 0x68) {
#endif
                    render->flags |= 0x80000000;
                }
                vel = &effect->velocity;
                CVector3__Scale(vel, vel, gProjectileSpeedScale);
                if ((kind == 0x2D || kind == 0x4F) && ((Actor *) source)->type == 2) {
                    if (gCollisionResults.m_pInstance != 0) {
                        aimEffect(effect, vel);
                    }
                }
                effect->unk1BC = gProjectilePower;
                CParticle__Attach(effect, 0, 0);
                if (entry->unk6 != -1) {
                    Trail_AddNode(&gEngineApp_m_SwooshSystem, effect, entry->unk6);
                }
                if (entry->unk7 != -1) {
                    ((CParticle *) effect)->m_pDLight = DLight_Spawn(&gScene.m_DLightSet, entry->unk7);
                } else {
                    ((CParticle *) effect)->m_pDLight = NULL;
                }
                if (!(effect->flags & 0x200000) && (sound = entry->sound) != 0) {
                    switch (effect->kind) {
                        case 0x22:
                        case 0x5F:
                        case 0x60:
                            CParticle__StartLoopSound(effect, sound);
                            break;
                        default:
                            Sound__Play(sound, effect->pos, 0, -1);
                            break;
                    }
                }
            }
        }
    }
    gCollisionResultsP = &gCollisionResults;
    return count;
}

#include "structs/world.h"

typedef struct B18Ent B18Ent;
/* Turok 2 layout (Rage Wars offsets in parentheses). */
struct B18Ent {
    u8 pad0[2];
    u16 type;           /* 0x002 (0x004) */
    CVector3 pos;          /* 0x004 (0x008) */
    u8 pad10[0xFC];
    B18Ent *owner;      /* 0x10C (0x12C) */
    u8 pad110[0x18];
    s16 unk128;         /* 0x128 (0x14C) */
    u8 pad12A[0x1A];
    s32 health;         /* 0x144 (0x174) */
    u8 pad148[0x3DC];
    s32 unk524;         /* 0x524 (0x5E4) */
    u8 pad528[0xDF8];
    B18Ent *next;       /* 0x1320 (0x16E0) */
};
#ifdef TARGET_PORT
#include "structs/particle.h"
#include "structs/player.h"
/* the host structs by member: kind-1 instances (m_AI.m_Health), the players (m_Data.m_Health,
 * m_pNext), the particles (m_pOwner, m_nFrames, the type) */
void CParticle__CheckProximity(sptr unused, B18Ent *ov, B18Ent **arrv, s32 n) {
    CParticle *o = (CParticle *) ov;
    CParticle **arr = (CParticle **) arrv;
    CVector3 d;
    s32 i;
    s32 cnt;
    CScene *scene = &gScene;
    CPlayer *p;

    cnt = scene->m_nKind1;
    for (i = 0; i < cnt; i++) {
        CGameObjectInstance *e = scene->m_pKind1[i];
        if (e->m_AI.m_Health > 0) {
            CVector3__Subtract(&d, &e->header.m_vPos, &o->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                o->m_nFrames = 1;
                return;
            }
        }
    }
    for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if ((void *) o->m_pOwner != (void *) p && p->m_Data.m_Health > 0) {
            CVector3__Subtract(&d, &p->m_CharacterActor.header.m_vPos, &o->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                o->m_nFrames = 1;
                return;
            }
        }
    }
    for (i = 0; i < n; i++) {
        CParticle *e = arr[i];
        if (e != o && (e->header.m_nObjType == 0xC || e->header.m_nObjType == 0x22)) {
            CVector3__Subtract(&d, &e->header.m_vPos, &o->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
                o->m_nFrames = 1;
                return;
            }
        }
    }
}
#else
void CParticle__CheckProximity(sptr unused, B18Ent *o, B18Ent **arr, s32 n) {
    CVector3 d;
    s32 i;
    s32 cnt;
    CScene *scene = &gScene;
    cnt = scene->m_nKind1;
    for (i = 0; i < cnt; i++) {
        B18Ent *e = (B18Ent *)scene->m_pKind1[i];
        if (e->health > 0) {
            CVector3__Subtract(&d, &e->pos, &o->pos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                o->unk128 = 1;
                return;
            }
        }
    }
    {
        B18Ent *p;
        for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != ((void *)0); p = p->next) {
            if (o->owner != p && p->unk524 > 0) {
                CVector3__Subtract(&d, &p->pos, &o->pos);
                if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                    o->unk128 = 1;
                    return;
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        B18Ent *e = arr[i];
        if (e != o && (e->type == 0xC || e->type == 0x22)) {
            CVector3__Subtract(&d, &e->pos, &o->pos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
                o->unk128 = 1;
                return;
            }
        }
    }
}
#endif
#include "globals/camera.h"

void CParticle__CheckProximity();

void CParticleSystem__Update(CParticleSystem *world) {
    CParticle *colliders[128];
    CParticle *stepped[128];
    CEventValue value;
    CVector3 delta;
    CParticle *p;
    CParticle *node;
    CParticle *focus;
    CParticle **begin;
    CROMParticleEffect *effect;
    CDLight *light;
    s32 count;
    s32 idx;
    s32 pairCount;
    s32 i;
    s32 j;
    s32 event;
    u32 particleType;
    u32 eventType;
    u32 soundType;
    f32 savedStep;

    count = 0;
    for (idx = 0; idx < 3; idx++) {
        for (node = (world->m_ActiveList + idx)->m_pHead; node != 0; node = node->m_pNext) {
            node->m_dwFlags = (node->m_dwFlags & 0xFFEFFFFF) | 0x02000000;
            stepped[count++] = node;
        }
    }
    if (gEngineApp_m_Settings.paused != 0 || gScene.m_WarpState != 0) {
        return;
    }
    savedStep = gFrameIncrement;
    gFrameIncrement = savedStep * gParticleTimeScale;
    pairCount = 0;
    for (idx = 0; idx < count; idx++) {
        p = stepped[idx];
        if ((p->m_dwFlags & 0x02000100) != 0x02000100) {
            continue;
        }
        p->m_dwFlags &= 0xFFEFFF87;
        if (p->m_pDLight) {
            p->m_pDLight->m_bEnabled = 0;
        }
        if (p->m_cFrame < (f32)p->m_nFrames && p->m_nFrames != 1 && !(p->m_dwFlags & 0x1000)) {
            if (gCameraVisibleBox.m_vMax.x > p->header.m_vPos.x && gCameraVisibleBox.m_vMax.y > p->header.m_vPos.y && gCameraVisibleBox.m_vMax.z > p->header.m_vPos.z &&
                gCameraVisibleBox.m_vMin.x <= p->header.m_vPos.x && gCameraVisibleBox.m_vMin.y <= p->header.m_vPos.y && gCameraVisibleBox.m_vMin.z <= p->header.m_vPos.z) {
                CParticle__Update(p);
                light = p->m_pDLight;
                if (light) {
                    light->m_bEnabled = 1;
                    light->x = p->header.m_vPos.x;
                    light->y = p->header.m_vPos.y;
                    light->z = p->header.m_vPos.z;
                    light->m_ColorFade = p->m_Alpha * (1.0f / 255.0f);
                }
                p->m_cFrame += gFrameIncrement;
            } else {
                CParticleSystem__Free(world, p);
            }
        } else {
            effect = p->m_pEffect;
            event = CParticle__IsUnderWater(p) ? 12 : 10;
            particleType = effect->m_pImpact->m_ImpactParticleType[event];
            eventType = effect->m_pImpact->m_ImpactEventType[event];
            value = effect->m_pImpact->m_ImpactEventValue[event];
            soundType = effect->m_pImpact->m_ImpactSoundType[event];
            if (eventType != 0xFFFF) {
                ImpactEvent_Send(p, p, eventType, -1, p->header.m_vPos, value);
            }
            if (particleType != 0xFFFF) {
                CParticle__SpawnChild(p, particleType, p->m_Alignment, 1);
            }
            if (soundType != 0xFFFF) {
                Sound__Play(soundType, p->header.m_vPos, 0, -1);
            }
            if (!(p->m_dwFlags & 0x100)) {
                continue;
            }
            CParticleSystem__Free(world, p);
        }
        if (p->m_dwFlags & 0x100) {
            switch (p->header.m_nObjType) {
            case 12:
            case 34:
            case 87:
                colliders[pairCount++] = p;
                break;
            }
        }
    }
    if (pairCount != 0) {
        begin = colliders;
        for (i = 0; i < pairCount; i++) {
            focus = begin[i];
            switch (focus->header.m_nObjType) {
            case 34:
                CParticle__CheckProximity(world, focus, begin, pairCount);
                break;
            case 87:
                for (j = 0; j < pairCount; j++) {
                    p = begin[j];
                    if (p->header.m_nObjType != 87) {
                        CVector3__Subtract(&delta, &p->header.m_vPos, &focus->header.m_vPos);
                        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z <= 65536.0f) {
                            p->m_nFrames = 1;
                        }
                    }
                }
                break;
            case 12:
                break;
            }
        }
    }
    gFrameIncrement = savedStep;
}

#include "structs/region.h"

/* Draws the particle render list: culls each sprite particle by its bounds
 * against the view, builds its matrix once per frame, draws it as a model or
 * a sprite (plus its trail), and queues the attached effects of the ones
 * that are drawn even when hidden.
 * The list walk is a goto loop and the built flag a local, as in the ROM: the
 * flag stays in a saved register and nothing else is hoisted out of the walk. */

extern s32 Frustum_TestBox(void *planes, CBox *box);
void Render_QueueModelByResource(void *model, void *mtx, void *mtx2, s32 a, s32 b);
extern void Swoosh_AddParticleNode(void *list, CParticle *particle, void *mtx);

void CParticleSystem__Draw(CParticleSystem *world, CCamera *view) {
    CBox box;
    CVector3 transformed;
    CParticle *node;
    CCamera *camera;
    s32 visible, built;
    f32 zScale, xyScale, xScale, maximum, height, radius;
    f32 minX, maxX, minY, maxY, minZ, maxZ;
    f32 depth;
    s32 mask;
    u32 builtFlag;

    if (world->m_DrawList.m_pHead != 0) {
        GFX_CMD(0xE7000000, 0)
        GFX_CMD(0xE3001A01, 0x20)
        GFX_CMD(0xE3001801, 0x80)
        GFX_CMD(0xE3000A01, 0)
        GFX_CMD(0xD9F9FBFF, 0)
        node = world->m_DrawList.m_pHead;
        mask = view->m_ViewParams.m_VisBits;
        builtFlag = 0x100000;
        if (node != 0) {
        next:
            visible = 0;
            built = node->m_dwFlags & builtFlag;
            if (node->header.m_pCurrentRegion == 0 || (mask & node->header.m_pCurrentRegion->m_VisBits)) {
                if (!(node->m_dwFlags & 0x400000) && node->m_pEffect->m_pModel == 0) {
                    zScale = node->m_vSize.z;
                    xyScale = node->m_vSize.y * zScale;
                    xScale = node->m_vSize.x * zScale;
                    if (!(xScale <= xyScale)) xyScale = xScale;
                    maximum = zScale;
                    if (!(xyScale <= maximum)) maximum = xyScale;
                    height = maximum * 1.4142135f;
                    radius = height * 0.5f;
                    minX = node->header.m_vPos.x - radius;
                    box.m_vMin.x = minX;
                    maxX = node->header.m_vPos.x + radius;
                    box.m_vMax.x = maxX;
                    minY = node->header.m_vPos.y - radius;
                    box.m_vMin.y = minY;
                    maxY = node->header.m_vPos.y + height;
                    box.m_vMax.y = maxY;
                    minZ = node->header.m_vPos.z - radius;
                    box.m_vMin.z = minZ;
                    maxZ = node->header.m_vPos.z + radius;
                    box.m_vMax.z = maxZ;
                    if (minX < view->m_ViewMax.x && view->m_ViewMin.x < maxX &&
                        minZ < view->m_ViewMax.z && view->m_ViewMin.z < maxZ &&
                        minY < view->m_ViewMax.y && view->m_ViewMin.y < maxY &&
                        Frustum_TestBox(view->m_ViewVolume, &box)) {
                        visible = 1;
                    }
                } else visible = 1;
            }
            do {
                if (visible) {
                    if (!built) {
                        if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 1) {
                            camera = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
                            CMtxF__TransformPoint(camera->m_mfView, &node->header.m_vPos, &transformed);
                            depth = transformed.z;
                            if (depth < 0.0f) depth = -depth;
                            CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], camera, depth);
                        } else {
                            CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], 0, 0.0f);
                        }
                        node->m_dwFlags |= builtFlag;
                    }
                    if (node->m_pEffect->m_pModel != 0) {
                        CScene__GetLightsAt(&gScene, &node->header.m_vPos, &node->m_Lights[gEvenOdd]);
                        Render_QueueModelByResource(node->m_pEffect->m_pModel, node->m_mtxDraw[gEvenOdd], &node->m_Lights[gEvenOdd], 0, -1);
                    } else {
                        CParticle__DrawSprite(node, view);
                    }
                    if (node->m_pEffect->m_dwFlags & builtFlag) {
                        CParticle__DrawShadow(node, view);
                    }
                    if (!node->m_ActiveSwooshes || built) break;
                    if (!(node->m_pEffect->m_dwFlags & 0x10000)) {
                        Swoosh_AddParticleNode(&gEngineApp_m_SwooshSystem, node, node->m_mtxDraw[gEvenOdd]);
                    }
                } else {
                    if (!node->m_ActiveSwooshes || built) break;
                    if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 1) {
                        camera = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
                        CMtxF__TransformPoint(camera->m_mfView, &node->header.m_vPos, &transformed);
                        depth = transformed.z;
                        if (depth < 0.0f) depth = -depth;
                        CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], camera, depth);
                    } else {
                        CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], 0, 0.0f);
                    }
                    node->m_dwFlags |= builtFlag;
                    if (!(node->m_pEffect->m_dwFlags & 0x10000)) {
                        Swoosh_AddParticleNode(&gEngineApp_m_SwooshSystem, node, node->m_mtxDraw[gEvenOdd]);
                    }
                }
            } while (0);
            node = node->m_pDrawNext;
            if (node != 0) goto next;
        }
        GFX_CMD(0xE7000000, 0)
        GFX_CMD(0xE3000A01, 0x100000)
        GFX_CMD(0xE3001801, 0)
        GFX_CMD(0xE3001A01, 0x10)
    }
}

void CParticle__AddDrawModeBits(CParticle *arg0, s32 *arg1) {
    s32 var_a2;

    var_a2 = 0;
    if (arg0->m_pEffect->m_dwFlags & 0x02000000) {
        if (arg0->m_dwFlags & 2) {
            var_a2 = 2;
        } else if (gEngineApp_m_Settings.unk17 == 2) {
            var_a2 = 1;
        }
    }
    *arg1 |= var_a2 << 0x1E;
}

typedef struct Node {
    u8 pad0[0x1C8];
    struct Node *next;    /* 0x1C8 */
} Node;
typedef struct Bucket {
    Node *head;
    u8 pad4[0x10];
} Bucket;
#ifdef TARGET_PORT
#include "structs/particle.h"
/* the host CParticleSystem by member (the views are at N64 offsets) */
void CParticleSystem__FreeAll(u8 *arg0) {
    CParticleSystem *sys = (CParticleSystem *) arg0;
    CParticle *node;
    CParticle *cur;
    s32 i;

    for (i = 0; i < 3; i++) {
        node = (CParticle *) sys->m_ActiveList[i].m_pHead;
        while (node != NULL) {
            cur = node;
            node = node->m_pNext;
            CParticleSystem__Free(arg0, (Node *) cur);
        }
    }
}
#else
void CParticleSystem__FreeAll(u8 *arg0) {
    s32 i;
    Node *node;
    Node *cur;

    for (i = 0; i < 3; i++) {
        node = ((Bucket *)(arg0 + 0xEC28))[i].head;
        while (node != NULL) {
            cur = node;
            node = node->next;
            CParticleSystem__Free(arg0, cur);
        }
    }
}
#endif

s32 CParticle__GetDrawMode(CParticle *arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (arg0->m_pEffect->m_dwFlags & 0x02000000) {
        if (arg0->m_dwFlags & 2) {
            var_a1 = 2;
        } else if (gEngineApp_m_Settings.unk17 == 2) {
            var_a1 = 1;
        }
    }
    return var_a1;
}

void CParticle__InitHeader(CGameObjectInstance *arg0) {
    arg0->header.m_pIntelligence = (struct CIntelligence *) &gParticleIntelligence;
    arg0->header.m_Type = 2;
    arg0->m_qGround.x = 0.0f;
    arg0->m_AI.m_pTargetOverride = 0;
    arg0->m_AI.m_pSightTarget = 0;
}

void CParticle__Kill(s32 arg0) {
    CParticleSystem__Free(&gScene.m_ParticleSystem, arg0);
    CParticle__StopLoopSound(arg0);
}

/* The parent actor block reached through actor->m_AI.m_pParent. */
typedef struct {
    /* 0x000 */ u8 pad0[0x934];
    /* 0x934 */ s16 flags934[1];
} Parent0023181C;

void CParticle__Release(CParticle *arg0) {
    sptr temp_s0;
    u16 temp_v1;
    u8 temp_a0;
    CParticle *temp_s0_2;

    if (arg0->header.m_nObjType == 0x68) {
        temp_s0 = (sptr) arg0->m_pOwner->m_AI.m_pParent;
#ifdef TARGET_PORT
        ((CPlayer *) temp_s0)->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo((void *) temp_s0, 0x15)] = 1;
#else
        ((Parent0023181C *)((*CPlayer__GetFiringAmmo((void *) temp_s0, 0x15) * 2) + temp_s0))->flags934[0] = 1;
#endif
    }
    temp_s0_2 = arg0->m_pSource;
    if (temp_s0_2 != NULL) {
        temp_a0 = temp_s0_2->header.m_Type;
        if ((temp_a0 == 2) && (temp_s0_2->m_dwFlags & 0x100) && ((temp_v1 = temp_s0_2->header.m_nObjType, (temp_v1 == temp_a0)) || (temp_v1 == 0x111))) {
            CParticleSystem__Free(&gScene.m_ParticleSystem, temp_s0_2);
            CParticle__StopLoopSound(temp_s0_2);
        }
    }
    CParticle__StopLoopSound(arg0);
}

void CParticle__SpawnAtWallHit(CParticle *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (actor->m_pEffect->m_dwFlags & 0x10) {
        origin = gCollisionResults.m_vVelocity;
    } else {
        origin = actor->m_vVelocity;
    }
    CQuaternion__FromDirection(&rot, &origin);
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, origin, rot,
                  gCollisionResults.m_vWallPos, gCollisionResults.m_pWallRegion, gCollisionResults.m_nWallEdge, (actor->m_dwFlags & 0x200006) | 1);
}

void CParticle__SpawnAtFloorHit(CParticle *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (actor->m_pEffect->m_dwFlags & 0x10) {
        origin = gCollisionResults.m_vVelocity;
    } else {
        origin = actor->m_vVelocity;
    }
    CQuaternion__FromDirection(&rot, &origin);
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, origin, rot,
                  gCollisionResults.m_vFloorPos, gCollisionResults.m_pFloorRegion, -3, (actor->m_dwFlags & 0x200006) | 1);
}

void CParticle__SpawnAtCeilingHit(CParticle *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (actor->m_pEffect->m_dwFlags & 0x10) {
        origin = gCollisionResults.m_vVelocity;
    } else {
        origin = actor->m_vVelocity;
    }
    CQuaternion__FromDirection(&rot, &origin);
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, origin, rot,
                  gCollisionResults.m_vFloorPos, gCollisionResults.m_pFloorRegion, -2, (actor->m_dwFlags & 0x200006) | 1);
}

void CParticle__SpawnAtWaterHit(CParticle *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (actor->m_pEffect->m_dwFlags & 0x10) {
        origin = gCollisionResults.m_vVelocity;
    } else {
        origin = actor->m_vVelocity;
    }
    CQuaternion__FromDirection(&rot, &origin);
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, origin, rot,
                  gCollisionResults.m_vWaterPos, (actor->m_dwHitFlags & 0x1000000) ? 0 : (sptr)gCollisionResults.m_pWaterRegion, -5, (actor->m_dwFlags & 0x200006) | 1);
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002838F4.c, func_002838F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
#include "structs/particle.h"
#endif

typedef struct B19Def B19Def;
typedef struct B19Obj B19Obj;
struct B19Def {
              u8 pad0[0x6];
              s8 b6;
              s8 b7;
              u8 pad8[0x4];
              s16 sC;
};
#ifdef TARGET_PORT
struct B19Obj {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_B19Obj(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_id[T2_HOFF_B19Obj(header.m_nObjType)]; u16 id; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_B19Obj(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_flags[T2_HOFF_B19Obj(m_dwFlags)]; u32 flags; }; /* N64 0x40 */
        struct { u8 _hp_unk118[T2_HOFF_B19Obj(m_pDLight)]; s32 unk118; }; /* N64 0x118 */
    };
};
#else
struct B19Obj {
    u8 pad0[0x2];
    u16 id;          /* 0x2 */
    CVector3 pos;     /* 0x4 */
    u8 pad10[0x30];
    u32 flags;       /* 0x40 */
    u8 pad44[0xD4];
    s32 unk118;      /* 0x118 */
};
#endif

void *DLight_Spawn(void *, s32);
/* The position as three floats: the port's shim (port/game/t2abi.c), the N64 definition under its own name. */
#ifdef TARGET_PORT
void Sound__Play_f3(s32 id, f32 x, f32 y, f32 z, s32 follow, s32 owner);
#else
void Sound__Play_f3(s16 id, f32 x, f32 y, f32 z, s32 follow, s32 owner) __asm__("Sound__Play");
#endif

void CParticle__StartEffects(B19Obj *o, B19Def *def) {
    s32 t;
    CParticle__Attach(o, 0, 0);
    if (def->b6 != -1) Trail_AddNode(&gEngineApp_m_SwooshSystem, o, def->b6);
    /* the light pointer-wide, in the host CParticle's member (the view has the N64 offset) */
    if (def->b7 != -1) ((CParticle *) o)->m_pDLight = DLight_Spawn(&gScene.m_DLightSet, def->b7);
    else ((CParticle *) o)->m_pDLight = NULL;
    if (!(o->flags & 0x200000) && (t = def->sC) != 0) {
        switch (o->id) {
        case 0x22:
        case 0x5F:
        case 0x60:
            CParticle__StartLoopSound(o, t);
            break;
        default:
            Sound__Play_f3(t, o->pos.x, o->pos.y, o->pos.z, 0, -1);
            break;
        }
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CParticle__BuildCameraMatrix receives CParticle__StopEffects's own argument
 * registers (pass-through); the host passes them. */
void CParticle__StopEffects(CParticle *arg0) {
    CDLight *temp_a1;

    if (arg0->m_ActiveSwooshes != 0) {
        CParticle__BuildCameraMatrix(arg0);
        Trail_DetachOwner(&gEngineApp_m_SwooshSystem, arg0);
    }
    temp_a1 = arg0->m_pDLight;
    if (temp_a1 != 0) {
        CDLightSet__Free((s32 *) &gScene.m_DLightSet, temp_a1);
        arg0->m_pDLight = NULL;
    }
}

/* Accelerates obj->m_vVelocity along obj->m_vDirection, clamping the speed to the parent limit. */
void CParticle__Accelerate(CParticle *obj) {
    CVector3 delta;
    CVector3 dir;
    CVector3 *pdir;
    CVector3 *vel;
    f32 accel;
    f32 speed;
    f32 maxSpeed;

    accel = HalfToFloat(obj->m_pEffect->m_pMotion->m_Accel);
    if (accel != 0.0f) {
        pdir = &dir;
        *pdir = obj->m_vDirection;
        CVector3__Scale(&delta, pdir, accel);
        vel = &obj->m_vVelocity;
        CVector3__Add(vel, vel, &delta);
        speed = func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z);
        maxSpeed = HalfToFloat(obj->m_pEffect->m_pMotion->m_MaxSpeed);
        if (accel > 0.0f) {
            if (maxSpeed < speed) {
                CVector3__Scale(vel, pdir, maxSpeed);
            }
        } else {
            if (speed < maxSpeed) {
                CVector3__Scale(vel, pdir, maxSpeed);
            }
        }
    }
}

/* Particle type 0x56, mode 2: sets flag 0x04000000. */
void func_00232018(CParticle *arg0, s32 arg1) {
    if ((arg0->header.m_nObjType == 0x56) && (arg1 == 2)) {
        arg0->m_dwFlags |= 0x04000000;
    }
}

f32 Random_Blend(f32 a, f32 b);

void CParticle__PlaceAtInstanceHit(CParticle *actor, s32 kind) {
    CVector3 dir;

    actor->m_dwFlags |= 0x20000;
    actor->header.m_vPos = gCollisionResults.m_vInstancePos;
    actor->m_vVelocity = gCollisionResults.m_vVelocity;
    dir = gCollisionResults.m_vVelocity;
    CVector3__Normalize(&dir);
    actor->m_vDirection = dir;
    CVector3__Scale(&dir, &dir, Random_Blend(-49.92f, 0.0f));
    CVector3__Add(&actor->header.m_vPos, &actor->header.m_vPos, &dir);
    actor->m_Shake = 0x7F;
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}

void CParticle__AttachToHitNode(CParticle *actor, CMtxF *mtx, s32 kind) {
    CMtxF tmp;
    CMtxF m;
    CVector3 dir;
    u8 *owner;

    owner = (u8 *)gCollisionResults.m_pInstance;
    if (mtx != NULL) {
        CMtxF__InvertAffine(&m, mtx);
    } else {
        CMtxF__FromMtxAffine(&tmp, (u8 *) ((CGameObjectInstance *) owner)->m_pmtDrawMtxs + gCollisionResults.m_nInstanceNode * 0x40);
        CMtxF__InvertAffine(&m, &tmp);
    }
    CMtxF__TransformPoint(&m, &gCollisionResults.m_vInstancePos, &actor->m_vAttachPos);
    dir = gCollisionResults.m_vVelocity;
    CVector3__Normalize(&dir);
    CMtxF__RotateVector(&m, &dir, &actor->m_vVelocity);
    actor->m_pTarget = (CGameObjectInstance *) owner;
    actor->m_dwFlags |= 0x10000;
#ifdef TARGET_PORT
    /* the low byte (byte 3 on the N64 is byte 0 on a little-endian host); the instance by member */
    actor->m_nBone = (s8) gCollisionResults.m_nInstanceNode;
    if (((CGameObjectInstance *) owner)->header.m_pIntelligence->m_Type == 1) {
        u8 *ai = (u8 *) &((CGameObjectInstance *) owner)->m_AI;
        if (actor->m_pOwner != NULL) {
            AI_HearNoise(owner, ai, &actor->m_pOwner->header.m_vPos, 3);
        } else {
            AI_HearNoise(owner, ai, &gCollisionResults.m_vInstancePos, 3);
        }
    }
#else
    actor->m_nBone = ((u8 *)&gCollisionResults.m_nInstanceNode)[3] /* its low byte */;
    if (**(s32 **) (owner + 0x14) == 1) {
        if (actor->m_pOwner != NULL) {
            AI_HearNoise(owner, owner + 0x140, &actor->m_pOwner->header.m_vPos, 3);
        } else {
            AI_HearNoise(owner, owner + 0x140, &gCollisionResults.m_vInstancePos, 3);
        }
    }
#endif
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}

#include "m2c_macros.h"

s32 func_002322C0(s32 arg0) {
    if (arg0 == 10) {
        return 0;
    }
    return arg0;
}

void func_002322D8(CParticle *o) {
    CROMParticleEffect *par = o->m_pEffect;
    s32 k = CParticle__IsUnderWater(o) ? 12 : 10;
    s32 s2 = par->m_pImpact->m_ImpactParticleType[k];
    s32 a2 = par->m_pImpact->m_ImpactEventType[k];
    CEventValue r = par->m_pImpact->m_ImpactEventValue[k];
    s32 s3 = par->m_pImpact->m_ImpactSoundType[k];
    if (a2 != 0xFFFF) ImpactEvent_Send(o, o, a2, -1, o->header.m_vPos, r);
    if (s2 != 0xFFFF) CParticle__SpawnChild(o, s2, o->m_Alignment, 1);
    if (s3 != 0xFFFF) Sound__Play_f3(s3, o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, 0, -1);
}

void CParticle__FireUpdateEvent(CParticle *o) {
    CROMParticleEffect *par = o->m_pEffect;
    /* Turok 2 uses slots 13 / 11 (Rage Wars 12 / 10). */
    s32 k = CParticle__IsUnderWater(o) ? 13 : 11;
    s32 s2 = par->m_pImpact->m_ImpactParticleType[k];
    s32 a2 = par->m_pImpact->m_ImpactEventType[k];
    CEventValue r = par->m_pImpact->m_ImpactEventValue[k];
    s32 s3 = par->m_pImpact->m_ImpactSoundType[k];
    if (a2 != 0xFFFF) ImpactEvent_Send(o, o, a2, -1, o->header.m_vPos, r);
    if (s2 != 0xFFFF) CParticle__SpawnChild(o, s2, o->m_Alignment, 1);
    if (s3 != 0xFFFF) Sound__Play_f3(s3, o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, 0, -1);
}

void CParticle__FireExpireEvent(CParticle *o) {
    CROMParticleEffect *par = o->m_pEffect;
    s32 k = CParticle__IsUnderWater(o) ? 12 : 10;
    s32 s2 = par->m_pImpact->m_ImpactParticleType[k];
    s32 a2 = par->m_pImpact->m_ImpactEventType[k];
    CEventValue r = par->m_pImpact->m_ImpactEventValue[k];
    s32 s3 = par->m_pImpact->m_ImpactSoundType[k];
    if (a2 != 0xFFFF) ImpactEvent_Send(o, o, a2, -1, o->header.m_vPos, r);
    if (s2 != 0xFFFF) CParticle__SpawnChild(o, s2, o->m_Alignment, 1);
    if (s3 != 0xFFFF) Sound__Play_f3(s3, o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, 0, -1);
}

/* Whether the particle is under water: below its region's water height (flag 4 when it has no region). */
s32 CParticle__IsUnderWater(CParticle *obj) {
    CRegionInfo *target;

    if (obj->header.m_pCurrentRegion != NULL) {
        if (!(obj->header.m_pCurrentRegion->m_wFlags & REGION_FLAG_WATER)) {
            return 0;
        }
        target = CScene__GetRegionInfo(&gScene, obj->header.m_pCurrentRegion);
        if (target != NULL) {
            goto found;
        }
    }
    return (obj->m_dwFlags >> 2) & 1;

found:
    if (target->m_dwFlags & 0x1000000) {
        return 0;
    }
    return obj->header.m_vPos.y < target->m_WaterHeight;
}

/* Whether the particle is at or below the water height of its region's info flagged 0x1000000. */
s32 CParticle__IsBelowWaterSurface(CParticle *obj) {
    CRegionInfo *target;

    if (obj->header.m_pCurrentRegion == NULL) {
        return 0;
    }
    if (!(obj->header.m_pCurrentRegion->m_wFlags & REGION_FLAG_WATER)) {
        return 0;
    }
    target = CScene__GetRegionInfo(&gScene, obj->header.m_pCurrentRegion);
    if (target == NULL) {
        return 0;
    }
    if (!(target->m_dwFlags & 0x1000000)) {
        return 0;
    }
    return obj->header.m_vPos.y <= target->m_WaterHeight;
}

#ifdef TARGET_PORT
#include "structs/camera.h"
/* the camera's field of view by member */
typedef struct B19Q2 {
    union {
        struct { u8 _hp_f6C[__builtin_offsetof(CCamera, m_Fov)]; f32 f6C; }; /* N64 0x6C */
    };
} B19Q2;
#else
typedef struct B19Q2 {
    u8 pad0[0x6C];
    f32 f6C;
} B19Q2;
#endif

#ifndef TARGET_PORT
/* CParticle__GetDistanceScale's constants, which it reads by name (the FAKEMATCH body keeps its
 * register pins): in the ROM's .rodata between CParticle__PlaceAtInstanceHit's literal and CParticle__SpawnAlongPath's, so
 * defined here (the port keeps the generated copies). 0.000492125982 = 1/2032. */
const f32 sDistanceScaleNear_def __asm__("sDistanceScaleNear") = 16.0f;
const f32 sDistanceScaleInvRange_def __asm__("sDistanceScaleInvRange") = 0.000492125982f;
const f32 sDistanceScaleRampMax_def __asm__("sDistanceScaleRampMax") = 1.0f;
const f32 sDistanceScaleInvRange2_def __asm__("sDistanceScaleInvRange2") = 0.000492125982f;
const f32 sDistanceScaleUnit_def __asm__("sDistanceScaleUnit") = 0.00100000005f;
const f32 sDistanceScaleDefault_def __asm__("sDistanceScaleDefault") = 1.0f;
const f32 sDistanceScaleFovRef_def __asm__("sDistanceScaleFovRef") = 47.5f;
const f32 sDistanceScaleFovBlend_def __asm__("sDistanceScaleFovBlend") = 0.25f;
const f32 sDistanceScaleInvFovRef_def __asm__("sDistanceScaleInvFovRef") = 0.0210526325f;
#endif

/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline f32 Ramp2(CROMParticleEffect *par, f32 xin) {
    register f32 x asm("$f4") = xin;
    f32 d;
    f32 k;
    d = x - sDistanceScaleNear;
    k = !(d < 0.0f) ? (!(sDistanceScaleRampMax < d * sDistanceScaleInvRange) ? (!(d < 0.0f) ? d * sDistanceScaleInvRange2 : 0.0f) : sDistanceScaleRampMax) : 0.0f;
    {
        register f32 fb asm("$f2") = par->m_pDistanceRange->m_FarScale;
        register f32 fa asm("$f3") = par->m_pDistanceRange->m_NearScale;
        x = x * (fa + (fb - fa) * k) * sDistanceScaleUnit;
    }
    return x;
}

f32 CParticle__GetDistanceScale(CParticle *o, f32 px, B19Q2 *q) {
    CROMParticleEffect *par = o->m_pEffect;
    register f32 x asm("$f5") = px;
    f32 t;
    if (par->m_dwFlags & 4) {
        t = Ramp2(par, x);
    } else {
        t = sDistanceScaleDefault;
    }
    {
        f32 m = q->f6C;
        return t * sDistanceScaleInvFovRef * (m + (sDistanceScaleFovRef - m) * sDistanceScaleFovBlend);
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/CODE_8028414C.CPP, func_80284178_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Matrix {
    f32 m[4][4];
} Matrix;
/* Turok 2 layout (smaller than Rage Wars' Shared_Particle). */
typedef struct Shared_Particle {
    char pad0[0x4];
    CVector3 pos;      /* 0x04 */
    char pad10[0x30];
    s32 flags;         /* 0x40 */
    char pad44[0x4];
    Matrix mtx[2];     /* 0x48, double-buffered */
} Shared_Particle;
struct Shared_ParticleView {
    char pad0[0x20C];
    Matrix viewMtx;    /* 0x20C */
};
/* The camera count read as an array (globals/camera.h declares the scalar): this view keeps the ROM's code. */
extern s32 gCameraCountWords[] __asm__("gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount");
#ifdef TARGET_PORT
#include "structs/particle.h"
#include "structs/camera.h"
extern void *gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead; /* local: globals/camera.h is not included here (its _m_nCount would clash with the s32[] view above) */
/* the host CParticle and CCamera by member (the camera list head by name: state[-4] is N64 layout) */
void CParticle__BuildCameraMatrix(void *arg0) {
    CParticle *p = (CParticle *) arg0;
    CCamera *cam;
    CVector3 delta;
    f32 amount;

    if (gCameraCountWords[0] == 1) {
        cam = (CCamera *) gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
        CMtxF__TransformPoint(&cam->m_mfView, &p->header.m_vPos, &delta);
        amount = delta.z;
        if (amount < 0.0f) {
            amount = -amount;
        }
        CParticle__BuildMatrix(arg0, (sptr) p->m_mtxDraw[gEvenOdd], (sptr) cam, amount);
    } else {
        CParticle__BuildMatrix(arg0, (sptr) p->m_mtxDraw[gEvenOdd], 0, 0.0f);
    }
    p->m_dwFlags |= 0x100000;
}
#else
void CParticle__BuildCameraMatrix(void *arg0) {
    CVector3 delta;
    void *arg1;
    f32 amount;
    s32 *state;
    state = gCameraCountWords;
    if (state[0] == 1) {
        arg1 = (void *)state[-4];
        CMtxF__TransformPoint(&((struct Shared_ParticleView *)arg1)->viewMtx, &((Shared_Particle *)arg0)->pos, &delta);
        amount = delta.z;
        if (amount < 0.0f) {
            amount = -amount;
        }
        CParticle__BuildMatrix(arg0,
                      (sptr)((char *)arg0 + ((gEvenOdd << 6) + 0x48)),
                      (sptr)arg1, amount);
    } else {
        CParticle__BuildMatrix(arg0,
                      (sptr)((char *)arg0 + ((gEvenOdd << 6) + 0x48)),
                      0, 0.0f);
    }
    ((Shared_Particle *)arg0)->flags |= 0x100000;
}
#endif

void CParticle__DrawModel(CParticle *arg0) {
    CScene__GetLightsAt(&gScene, &arg0->header.m_vPos, &arg0->m_Lights[gEvenOdd]);
    Render_QueueModelByResource(arg0->m_pEffect->m_pModel, arg0->m_mtxDraw[gEvenOdd], &arg0->m_Lights[gEvenOdd], 0, -1);
}

/* Spawns `count` copies along the segment the actor covered this frame. */
void CParticle__SpawnAlongPath(CParticle *actor) {
    CQuaternion rot;
    CVector3 delta;
    CVector3 pos;
    CVector3 dir;
    s32 count;
    s32 kind;
    s32 i;
    u8 *table;

    count = actor->m_pEffect->m_pParams->m_nTrailCount;
    if (count > 0) {
        dir = actor->m_vVelocity;
        CQuaternion__FromDirection(&rot, &dir);
        CVector3__Subtract(&delta, &actor->header.m_vPos, &actor->m_vLastPos);
        table = &gScene.m_ParticleSystem;
        for (i = 1; i <= count; i++) {
            CVector3__Scale(&pos, &delta, i / (count + 1.0f));
            CVector3__Add(&pos, &pos, &actor->m_vLastPos);
            kind = actor->header.m_nObjType;
            CParticleSystem__Spawn(table, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, dir, rot,
                          pos, actor->header.m_pCurrentRegion, -1, 0x20E001);
        }
    }
}

/* Starts (or restarts) the looping sound on the object. */
void CParticle__StartLoopSound(CParticle *arg0, s32 arg1) {
    s32 var_s1;
    CVector3 *temp_a2;
    SndSource *temp_v1;
    CParticle *var_s0;

    var_s0 = arg0;
    temp_v1 = var_s0->m_pLoopSound;
    var_s1 = arg1;
    if (temp_v1 != NULL) {
        if ((temp_v1->sound.w != var_s1) || (temp_v1->handle == -1)) {
            CParticle__StopLoopSound(var_s0);
            goto block_4;
        }
    } else {
block_4:
        temp_a2 = &var_s0->header.m_vPos;
        var_s0->m_pLoopSound = (SndSource *) LoopSounds_Start(LoopSounds_Get(), var_s1, temp_a2, temp_a2, -1);
    }
}

/* the host CParticle's m_pLoopSound, pointer-wide */
void CParticle__StopLoopSound(SoundObject *arg0) {
    CParticle *p = (CParticle *) arg0;
    if (p->m_pLoopSound != NULL) {
        LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopSound);
        p->m_pLoopSound = NULL;
    }
}

struct PoolNode84478;
typedef struct PoolBucket84478 {
    struct PoolNode84478 *head;
    struct PoolNode84478 *tail;
    s32 data[3];
} PoolBucket84478;
typedef struct PoolOwner84478 {
    char pad0[0xEC00]; /* Turok 2 offsets (Rage Wars: 0xFC00) */
    PoolBucket84478 free;
    char padEC14[0x14];
    PoolBucket84478 slots[256];
} PoolOwner84478;
typedef struct PoolNode84478 {
    char pad0[0x40]; /* Turok 2 layout (Rage Wars: state 0x5C, owner 0x1E4) */
    s32 state;
    char pad44[0x1C0 - 0x44];
    PoolBucket84478 *owner;
    char pad1C4[8];
    s32 counter;
    s32 flags;
} PoolNode84478;
#ifdef TARGET_PORT
#include "structs/particle.h"
/* the host CParticleSystem/CParticle by member (the views are at N64 offsets) */
PoolNode84478 *CParticleSystem__AllocNode(PoolOwner84478 *owner, s32 index) {
    CParticleSystem *sys = (CParticleSystem *) owner;
    CParticle *node;
    CList *slot;
    s32 i;

    if (sys->m_FreeList.m_pHead == 0) {
        for (i = 0; i <= (index & 0xFF); i++) {
            if (sys->m_ActiveList[i].m_pTail != 0) {
                CParticleSystem__Free(owner, sys->m_ActiveList[i].m_pTail);
                break;
            }
        }
    }
    node = sys->m_FreeList.m_pHead;
    if (node == 0) {
        return 0;
    }
    CList__Remove(&sys->m_FreeList, node);
    slot = &sys->m_ActiveList[index & 0xFF];
    CList__AddHead(slot, node);
    node->m_pActiveList = slot;
    node->m_pDrawNext = 0;
    node->m_pDrawLast = 0;
    node->m_dwFlags = 0x100;
    return (PoolNode84478 *) node;
}
#else
PoolNode84478 *CParticleSystem__AllocNode(PoolOwner84478 *owner, s32 index) {
    PoolNode84478 *node;
    PoolBucket84478 *slot;
    s32 i;
    PoolNode84478 *tail;
    if (owner->free.head == 0) {
        for (i = 0; i <= (index & 0xFF); i++) {
            tail = (&owner->slots[i])->tail;
            if (tail != 0) {
                CParticleSystem__Free(owner, tail);
                break;
            }
        }
    }
    node = owner->free.head;
    if (node == 0) {
        return 0;
    }
    CList__Remove(&owner->free, node);
    slot = &owner->slots[index & 0xFF];
    CList__AddHead(slot, node);
    node->owner = slot;
    node->flags = 0;
    node->counter = 0;
    node->state = 0x100;
    return node;
}
#endif

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8028308C_4.CPP, func_8028466C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct EntryRecAt4V1;
struct EntryRecAt4V1 {
    int unk0;
    void *handle;
    char pad8[0xC];
};
typedef struct EntryRecAt4V1 EntryRecAt4V1;
#ifdef TARGET_PORT
#include "common.h"
#include "structs/particle.h"
/* the host CParticleSystem by member: the tail of the first non-empty active list up to count */
void CParticleSystem__FreeOldest(char *obj, unsigned char count) {
    CParticleSystem *sys = (CParticleSystem *) obj;
    int i;
    for (i = 0; i <= count; i++) {
        if (sys->m_ActiveList[i].m_pTail != 0) {
            CParticleSystem__Free(obj, sys->m_ActiveList[i].m_pTail);
            return;
        }
    }
}
#else
void CParticleSystem__FreeOldest(char *obj, unsigned char count) {
    int i;
    for (i = 0; i <= count; i++) {
        EntryRecAt4V1 *e = (EntryRecAt4V1 *)(obj + i * sizeof(EntryRecAt4V1) + 0xEC28);
        if (e->handle != 0) {
            CParticleSystem__Free(obj, e->handle);
            return;
        }
    }
}
#endif

/* Finds the particle of owner arg1 (and of template arg2, when given) in the template's active list. */
void *CParticleSystem__FindOwned(CParticleSystem *arg0, s32 arg1, CROMParticleEffect *arg2) {
    CParticle *record;
    /* index[array] order matches the original addu operand order */
    record = arg2->m_pParams->m_nList[arg0->m_ActiveList].m_pHead;
    if (arg2 != 0) {
        if (record != 0) {
            do {
                if ((sptr) record->m_pOwnerID == arg1) {
                    if (record->m_pEffect == arg2) {
                        return record;
                    }
                }
                record = record->m_pNext;
            } while (record != 0);
        }
    } else if (record != 0) {
        do {
            if ((sptr) record->m_pOwnerID == arg1) {
                return record;
            }
            record = record->m_pNext;
        } while (record != 0);
    }
    return 0;
}

/* Insert arg1 into the list at 0xEC14: after the node with the same key at
 * 0xF8 if there is one, else at the end or the front by a flag. */
void CParticleSystem__InsertDrawSorted(CParticleSystem *arg0, CParticle *arg1) {
    CParticle *p;

    for (p = arg0->m_DrawList.m_pHead; p != NULL; p = p->m_pDrawNext) {
        if (p->m_pEffect == arg1->m_pEffect) {
            break;
        }
    }
    if (p != NULL) {
        CList__InsertBefore(&arg0->m_DrawList, p, arg1);
    } else if (arg1->m_pEffect->m_dwFlags & 0x2000) {
        CList__AddHead(&arg0->m_DrawList, arg1);
    } else {
        CList__AddTail(&arg0->m_DrawList, arg1);
    }
    arg1->m_dwFlags |= 0x01000000;
}

s32 Random_UpTo(s32 arg0) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;

    if (arg0 == 0) {
        return 0;
    }
    temp_v0 = Random_Next();
    temp_v1 = arg0 + 1;
    temp_v0_2 = temp_v0 / temp_v1;
    if (temp_v1 == 0) {

    }
    if ((temp_v1 == -1) && (temp_v0_2 == 0x80000000)) {

    }
    return temp_v0 % temp_v1;
}

#ifdef TARGET_PORT
f32 Random_Float(f32 scale); /* prototyped: an unprototyped call would pass the f32 as a double */
#else
s32 Random_Float();                            /* extern */
#endif
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Random_Float receives func_00232E80's own argument
 * registers (pass-through); the host passes them. */
void func_00232E80(f32 arg0) {
    if (arg0 != 0.0f) {
        Random_Float(arg0);
    }
}
#else
void func_00232E80(f32 arg0) {
    if (arg0 != 0.0f) {
        Random_Float();
    }
}
#endif

f32 CParticle__GetSpriteRotX(CParticle *arg0) {
    f32 var_f0;

    var_f0 = 0.0f;
    if (arg0->m_pEffect->m_pModel == NULL) {
        var_f0 = arg0->m_vRot.x;
    }
    return var_f0;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
} AnimData;
/* The object as this function reads it: the word at 0xF8 (in m_BlendAnimation by the
 * LibTEngine layout) is a pointer here. */
typedef struct {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ AnimData *anim;
} ObjAnimView;
#ifdef TARGET_PORT
#include "structs/particle.h"
/* a particle (N64 0xF8 is CParticle.m_pEffect, 0x160 m_vRot.y): the host members */
f32 CParticle__GetSpriteRotY(CGameObjectInstance *arg0) {
    CParticle *p = (CParticle *) arg0;

    if (p->m_pEffect->m_pModel == NULL) {
        return p->m_vRot.y;
    }
    return 0.0f;
}
#else
f32 CParticle__GetSpriteRotY(CGameObjectInstance *arg0) {
    f32 var_f0;

    var_f0 = 0.0f;
    if (((ObjAnimView *) arg0)->anim->unk14 == 0) {
        var_f0 = arg0->m_AI.m_vFlinchAxis.z;
    }
    return var_f0;
}
#endif

s32 func_00232EEC(CParticle *o) {
    switch (o->header.m_nObjType) {
    case 2:
    case 0x56:
    case 0x111:
    case 0x126:
        return 1;
    default:
        return 0;
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002848FC.c, func_002848FC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B19PF B19PF;
typedef struct B19Lim B19Lim;
typedef struct B19Rec8 B19Rec8;
typedef struct B19Tbl B19Tbl;
typedef struct B19Info38 B19Info38;
typedef struct B19List B19List;
typedef struct B19Handle B19Handle;
typedef struct B19Parent B19Parent;
typedef struct B19Spawner B19Spawner;
struct B19PF {
              u8 pad0[0x1C];
               f32 a;
               f32 b;
};
struct B19Lim {
              u8 pad0[0x10];
               u16 a;
               u16 b;
};
struct B19Rec8 {
              s32 a;
              s32 b;
};
struct B19Tbl {
              B19Rec8 rec[14];
               u16 w70[14];
               u16 w8C[14];
               u16 wA8[14];
};
struct B19Info38 {
              u8 pad0[0x8];
              s8 idx;
              u8 pad9[0x3];
              s8 n;
};
struct B19List {
              B19Spawner * head;
              B19Spawner * tail;
              u8 pad8[0xC];
};
struct B19Handle {
              u8 pad0[0x8];
              s32 f8;
              s32 fC;
};
struct B19Parent {
              u32 flags;
              u8 pad4[0xC];
               s16 s10;
               u8 pad12[0x2];
               s32 unk14;
               B19Tbl * tbl;
               u8 pad1C[0x8];
               B19PF * pf;
               u8 pad28[0x8];
               B19Lim * lim;
               u8 pad34[0x4];
               B19Info38 * info38;
};
struct B19Spawner {
              s8 kind;
              u8 pad1[0x1];
              u16 id; /* Turok 2: id at 0x2 */
              u8 pad4[0x4];
              CVector3 pos;
               u8 pad14[0x4];
               void * info;
               CVector3 pos2;
               u8 pad28[0x34];
               u32 flags;
               u8 pad60[0xA0];
                u32 flags100;
                u8 pad104[0x14];
                B19Parent * parent;
                u8 pad11C[0x8];
                s32 unk124;
                s32 unk128;
                s32 unk12C;
                s32 * unk130;
                void * unk134;
                s32 unk138;
                u8 pad13C[0x10];
                s16 unk14C;
                u8 pad14E[0x26];
                CVector3 vel;
                f32 unk180;
                f32 unk184;
                u8 pad188[0x48];
                s8 unk1D0;
                u8 unk1D1;
                u8 pad1D2[0x7];
                u8 unk1D9;
                u8 pad1DA[0x2];
                B19Handle * unk1DC;
                u8 pad1E0[0x4];
                B19List * slotp;
                u8 pad1E8[0x4];
                B19Spawner * next;
                s32 unk1F0;
                B19Spawner * next2;
};

s32 func_00232F38(B19Spawner *o) {
    switch (o->id) {
    case 5: case 6: case 7: case 9: case 0x11: case 0x13: case 0x1A: case 0x101: case 0x110:
        return 1;
    default:
        return 0;
    }
}

int func_00232FBC(CParticle *o) {
    switch (o->header.m_nObjType) {
    case 8:
    case 0xD:
    case 0x12:
        return 1;
    default:
        return 0;
    }
}

struct CollisionInfo;

/* Whether the line from the camera to `target` is clear: a collision test
 * from the camera's position, made with the camera's player (moved there and
 * back) when it has one and the camera is in its normal mode, else with a
 * copy of the target's head. */
s32 CCamera__CanSeeTarget(CGameObjectInstance *target, CCamera *cam) {
    CObjectHead tmp;
    CGameObjectInstance *player;
    s32 hit;

    player = CCamera__GetPlayer(cam);
    if (player != NULL && cam->m_ViewParams.m_nType == 0) {
        tmp = *(CObjectHead *) player;
        player->header.m_vPos = cam->m_vPos;
        hit = Collision_MoveInstance(player, target->header.m_vPos, &gCollisionInfoLineOfSight);
        *(CObjectHead *) player = tmp;
    } else {
        tmp = *(CObjectHead *) target;
        tmp.header.m_vPos = cam->m_vPos;
        hit = Collision_MoveInstance(&tmp, target->header.m_vPos, &gCollisionInfoLineOfSight);
    }
    return hit == 0;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284D98.c, func_00284D98.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);

void CParticle__ExpireNearby(void *owner, CParticle *ref, CParticle **arr, s32 n) {
    s32 i;
    CVector3 d;
    for (i = 0; i < n; i++) {
        CParticle *o = arr[i];
        if (o->header.m_nObjType != 0x57) {
            CVector3__Subtract(&d, &o->header.m_vPos, &ref->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) o->m_nFrames = 1;
        }
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284E70.c, func_00284E70.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
#include "common.h"
#include "structs/particle.h"
#endif

typedef struct B19ProxObj B19ProxObj;
#ifdef TARGET_PORT
struct B19ProxObj {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_B19Obj(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_id[T2_HOFF_B19Obj(header.m_nObjType)]; u16 id; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_B19Obj(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_unk128[T2_HOFF_B19Obj(m_nFrames)]; s16 unk128; }; /* N64 0x128 */
    };
};
#else
struct B19ProxObj {
    u8 pad0[0x2];
    u16 id;          /* 0x2 */
    CVector3 pos;     /* 0x4 */
    u8 pad10[0x118];
    s16 unk128;      /* 0x128 */
};
#endif

void CParticleSystem__ResolveInteractions(void *owner, B19ProxObj **arr, s32 n) {
    s32 i, j;
    CVector3 d;
    B19ProxObj *o;
    B19ProxObj *e;
    for (i = 0; i < n; i++) {
        o = arr[i];
        switch (o->id) {
        case 0xC:
            break;
        case 0x22:
            CParticle__CheckProximity(owner, o, arr, n);
            break;
        case 0x57:
            for (j = 0; j < n; j++) {
                e = arr[j];
                if (e->id != 0x57) {
                    CVector3__Subtract(&d, &e->pos, &o->pos);
                    if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) e->unk128 = 1;
                }
            }
            break;
        }
    }
}
