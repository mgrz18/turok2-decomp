/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0027ADBC.c, func_0027ADBC, with Turok 2's layouts. */
/* A projectile's per-frame steering: by type, then homing, thrust and movement. */

#include "common.h"
#include "structs/vector.h"
#include "globals/game.h"
#include "protos/game.h"

#ifdef TARGET_PORT
typedef struct ProjNode {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ProjNode(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_ProjNode(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_height[T2_HOFF_ProjNode(m_CharacterActor.m_CollisionYOffset)]; f32 height; }; /* N64 0x54 */
        struct { u8 _hp_next[T2_HOFF_ProjNode(m_pNext)]; struct ProjNode *next; }; /* N64 0x1320 */
    };
} ProjNode;
#else
typedef struct ProjNode {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x1320 - 0x58];
    /* 0x1320 */ struct ProjNode *next;
} ProjNode;
#endif

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
        struct { u8 _hp_turnRate[T2_HOFF_ProjDef(unk_0E)]; s16 turnRate; }; /* N64 0xE */
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
#include "structs/sound.h"
#endif
#ifdef TARGET_PORT
typedef struct ProjSound {
/* the view at the host offsets of SndSource (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ProjSound(m) __builtin_offsetof(SndSource, m)
    union {
        struct { u8 _hp_state[T2_HOFF_ProjSound(handle)]; s32 state; }; /* N64 0x8 */
        struct { u8 _hp_id[T2_HOFF_ProjSound(sound.w)]; s32 id; }; /* N64 0xC */
    };
} ProjSound;
#else
typedef struct ProjSound {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 id;
} ProjSound;
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
        struct { u8 _hp_target[T2_HOFF_Projectile(m_pTarget)]; ProjNode *target; }; /* N64 0x114 */
        struct { u8 _hp_heading[T2_HOFF_Projectile(m_vDirection)]; CVector3 heading; }; /* N64 0x150 */
        struct { u8 _hp_sound[T2_HOFF_Projectile(m_pLoopSound)]; ProjSound *sound; }; /* N64 0x1B8 */
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
    /* 0x114 */ ProjNode *target;
    /* 0x118 */ u8 pad118[0x150 - 0x118];
    /* 0x150 */ CVector3 heading;
    /* 0x15C */ u8 pad15C[0x1B8 - 0x15C];
    /* 0x1B8 */ ProjSound *sound;
} Projectile;
#endif


extern f32 HalfToFloat(s32 halfFloat);
extern f32 func_00298470(f32);             /* sqrtf */
extern f32 GameObject_GetCollisionHeight(ProjNode *node);
extern f32 func_0026841C(ProjNode *node);
extern f32 Random_Blend(f32 lo, f32 hi);   /* random in range */
extern f32 Math_AcosLookup(f32);             /* acosf */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CVector3__LimitLength(CVector3 *v, f32 max);
extern void CVector3__Normalize(CVector3 *v);
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
extern void CQuaternion__SlerpFast(CQuaternion *out, f32 t, CQuaternion *a, CQuaternion *b);
extern void CQuaternion__ToMatrix(CQuaternion *q, CMtxF *out);
extern void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
extern ProjSound *LoopSounds_Start(void *pool, s32 id, CVector3 *pos, CVector3 *pos2, sptr arg4);
extern void CParticle__StopLoopSound(Projectile *p);
extern void CParticle__Wobble(Projectile *p, f32 scale);
extern void CParticle__UpdateHomingMissile(Projectile *p);
extern void CParticle__UpdateReturning(Projectile *p);
extern void CParticle__UpdateHoming(Projectile *p);

/* CParticle__StartLoopSound, inlined: (re)start the projectile's looping sound unless it is already playing. */
static __inline__ void PlayLoopingSound(Projectile *o, s32 id) {
    if (o->sound != NULL) {
        if (o->sound->id == id && o->sound->state != -1) {
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
    ProjNode *p;
    ProjNode *p2;
    CVector3 *tpp;
    CVector3 *velp;
    ProjNode *t;
    ProjDef *def;
    f32 h, fa, fb, fc, fd, k, speed, ang, w, tt;

    switch (o->type) {
        case 0x2E:
            p2 = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
            if (p2 != NULL) {
                tpp = &tp;
                velp = &o->velocity;
                fa = 0.5f;
                fb = 2.0f;
                __asm__ __volatile__("");
                p = p2;
                do {
                    h = p->height;
                    tp = p->pos;
                    tp.y += h;
                    tp.y += GameObject_GetCollisionHeight(p2) * fa;
                    CVector3__Subtract(&d, tpp, &o->pos);
                    k = func_00298470(d.x * d.x + d.y * d.y + d.z * d.z);
                    if (k < 512.0f) {
                        k = 1.0f - k * (1.0f / 512);
                        d.x *= k * Random_Blend(fa, fb);
                        d.y *= k * Random_Blend(fa, fb);
                        d.z *= k * Random_Blend(fa, fb);
                        CVector3__Subtract(velp, velp, &d);
                    }
                    p2 = p->next;
                    p = p2;
                } while (p2 != NULL);
            }
            t = o->target;
            tt = func_0026841C(t);
            tp = t->pos;
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
                tt = func_0026841C(t);
                tp = t->pos;
                tp.y += tt;
                tp.y += GameObject_GetCollisionHeight(t) * 0.5f;
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
