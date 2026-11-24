/* FAKEMATCH: a do { } while (0) around `fa = 0.5f` (the permuter, round 22, in place of round 21's empty
 * `__asm__` barrier); without it CParticle__Steer copies the player pointer before the two constants are
 * loaded: 5 of 390 words. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0027ADBC.c, func_0027ADBC, with Turok 2's layouts. */
/* A projectile's per-frame steering: by type, then homing, thrust and movement. */

#include "common.h"
#include "structs/player.h"
#include "structs/vector.h"
#include "globals/game.h"
#include "protos/game.h"


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

#include "structs/sound.h"

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


f32 HalfToFloat(s32 half);
extern f32 func_00298470(f32);             /* sqrtf */
f32 GameObject_GetCollisionHeight(CGameObjectInstance *a);
f32 GameObject_GetCollisionYOffset(CGameObjectInstance *item);
extern f32 Random_Blend(f32 lo, f32 hi);   /* random in range */
extern f32 Math_AcosLookup(f32);             /* acosf */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CVector3__LimitLength(CVector3 *v, f32 max);
extern void CVector3__Normalize(CVector3 *v);
void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
void CQuaternion__SlerpFast(CQuaternion *out, f32 t, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, void *m);
void CMtxF__TransformPoint(void *m, CVector3 *in, CVector3 *out);
void *LoopSounds_Start(void *pool, s32 id, CVector3 *pos, void *pos2, sptr loops);
extern void CParticle__StopLoopSound(Projectile *p);
extern void CParticle__Wobble(Projectile *p, f32 scale);
extern void CParticle__UpdateHomingMissile(Projectile *p);
extern void CParticle__UpdateReturning(Projectile *p);
extern void CParticle__UpdateHoming(Projectile *p);

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
