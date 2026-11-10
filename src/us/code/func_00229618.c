#include "common.h"
#include "structs/vector.h"
#include "globals/engine.h"

/* Projectile wobble: swings the velocity sideways (and then up/down) along
 * sine waves whose amplitude scales with how close the projectile is to its
 * top speed. */

#ifdef TARGET_PORT
#include "structs/particle.h"
#endif
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 maxSpeed;
} WobbleParams;

#ifdef TARGET_PORT
typedef struct ProjectileDef {
/* the view at the host offsets of CROMParticleEffect (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ProjectileDef(m) __builtin_offsetof(CROMParticleEffect, m)
    union {
        struct { u8 _hp_wobble[T2_HOFF_ProjectileDef(m_pOscillation)]; WobbleParams *wobble; }; /* N64 0x2C */
    };
} ProjectileDef;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ WobbleParams *wobble;
} ProjectileDef;
#endif

#ifdef TARGET_PORT
typedef struct Projectile {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Projectile(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_vel[T2_HOFF_Projectile(m_vVelocity)]; CVector3 vel; }; /* N64 0x18 */
        struct { u8 _hp_def[T2_HOFF_Projectile(m_pEffect)]; ProjectileDef *def; }; /* N64 0xF8 */
        struct { u8 _hp_speed[T2_HOFF_Projectile(m_cFrame)]; f32 speed; }; /* N64 0x11C */
        struct { u8 _hp_dir[T2_HOFF_Projectile(m_vDirection)]; CVector3 dir; }; /* N64 0x150 */
        struct { u8 _hp_phaseX[T2_HOFF_Projectile(m_WobblePhase)]; f32 phaseX; }; /* N64 0x178 */
        struct { u8 _hp_phaseY[T2_HOFF_Projectile(m_WobblePhase[1])]; f32 phaseY; }; /* N64 0x17C */
        struct { u8 _hp_freqX[T2_HOFF_Projectile(unk_180)]; f32 freqX; }; /* N64 0x180 */
        struct { u8 _hp_freqY[T2_HOFF_Projectile(unk_180[4])]; f32 freqY; }; /* N64 0x184 */
        struct { u8 _hp_ampX[T2_HOFF_Projectile(m_WobbleAmplitude)]; f32 ampX; }; /* N64 0x188 */
        struct { u8 _hp_ampY[T2_HOFF_Projectile(m_WobbleAmplitude[1])]; f32 ampY; }; /* N64 0x18C */
    };
} Projectile;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ CVector3 vel;
    /* 0x024 */ u8 pad24[0xF8 - 0x24];
    /* 0x0F8 */ ProjectileDef *def;
    /* 0x0FC */ u8 padFC[0x11C - 0xFC];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x150 - 0x120];
    /* 0x150 */ CVector3 dir;
    /* 0x15C */ u8 pad15C[0x178 - 0x15C];
    /* 0x178 */ f32 phaseX;
    /* 0x17C */ f32 phaseY;
    /* 0x180 */ f32 freqX;
    /* 0x184 */ f32 freqY;
    /* 0x188 */ f32 ampX;
    /* 0x18C */ f32 ampY;
} Projectile;
#endif

extern f32 func_002982F0(f32);                              /* sinf */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a x b */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a - b */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */

void CParticle__Wobble(Projectile *p, f32 scale) {
    CVector3 up;
    CVector3 axis;
    CVector3 dir;
    CVector3 before;
    CVector3 after;
    CVector3 side;
    f32 maxSpeed;
    f32 k;

    k = 1.0f;
    maxSpeed = p->def->wobble->maxSpeed;
    if (p->speed < maxSpeed) {
        k = p->speed / maxSpeed;
    }
    k *= scale;
    dir = p->dir;
    if (p->ampX > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3__Cross(&axis, &up, &dir);
        CVector3__Scale(&before, &axis, func_002982F0(p->phaseX) * p->ampX * k);
        p->phaseX += p->freqX * k * gFrameIncrement;
        CVector3__Scale(&after, &axis, func_002982F0(p->phaseX) * p->ampX * k);
        CVector3__Subtract(&p->vel, &p->vel, &before);
        CVector3__Add(&p->vel, &p->vel, &after);
    } else {
        p->phaseX += p->freqX * k * gFrameIncrement;
    }
    if (p->ampY > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3__Cross(&side, &up, &dir);
        CVector3__Cross(&axis, &side, &dir);
        CVector3__Scale(&before, &axis, func_002982F0(p->phaseY) * p->ampY * k);
        p->phaseY += p->freqY * k * gFrameIncrement;
        CVector3__Scale(&after, &axis, func_002982F0(p->phaseY) * p->ampY * k);
        CVector3__Subtract(&p->vel, &p->vel, &before);
        CVector3__Add(&p->vel, &p->vel, &after);
    } else {
        p->phaseY += p->freqY * k * gFrameIncrement;
    }
}
