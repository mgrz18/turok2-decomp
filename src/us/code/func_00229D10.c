#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

/* Same family as CParticle__UpdateHomingMissile (homing missile): here a projectile that flies
 * back to a point beside its owner's player and is caught there. */

extern s32 LoopSounds_Get(void);
extern void *LoopSounds_Start(s32 bank, s32 sound, CVector3 *pos, CVector3 *pos2, s32 loops);  /* starts a sound */
extern void CParticle__StopLoopSound(void *obj);

extern f32 HalfToFloat(s32 half);                         /* half float -> f32 */
extern f32 func_00298470(f32);                              /* sqrtf */
extern f32 Math_AcosLookup(f32);                              /* acosf */
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

extern void CPlayer__TransformByView(CPlayer *player, CVector3 *pos);      /* player-relative point to world */
extern f32 GameObject_GetCollisionHeight(CGameObjectInstance *owner);                     /* height */

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 accel;      /* half floats */
    /* 0x12 */ u16 maxSpeed;
} MoveParams;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ MoveParams *move;
} ParentDef;

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
    /* 0x024 */ u8 pad24[0xF8 - 0x24];
    /* 0x0F8 */ ParentDef *parent;
    /* 0x0FC */ u8 padFC[0x10C - 0xFC];
    /* 0x10C */ CGameObjectInstance *owner;
    /* 0x110 */ u8 pad110[0x11C - 0x110];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x128 - 0x120];
    /* 0x128 */ s16 caught;
    /* 0x12A */ u8 pad12A[0x150 - 0x12A];
    /* 0x150 */ CVector3 dir;
    /* 0x15C */ u8 pad15C[0x1B8 - 0x15C];
    /* 0x1B8 */ SoundHandle *loop;
} Missile;

extern f32 D_800ACE68; /* speed above which it flies home (15) */
extern f32 D_800ACE6C; /* catch radius (512) */

/* Same as CParticle__Accelerate: accelerate along dir, clamp to the max speed. */
static __inline__ void Missile_Accelerate(Missile *obj) {
    CVector3 delta;
    CVector3 dir;
    CVector3 *vel;
    f32 accel;
    f32 speed;
    f32 maxSpeed;

    accel = HalfToFloat(obj->parent->move->accel);
    if (accel != 0.0f) {
        dir = obj->dir;
        CVector3__Scale(&delta, &dir, accel);
        vel = &obj->vel;
        CVector3__Add(vel, vel, &delta);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        maxSpeed = HalfToFloat(obj->parent->move->maxSpeed);
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

void CParticle__UpdateReturning(Missile *obj) {
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
    SoundHandle *loop;
    f32 speed;
    f32 dist;
    f32 t;
    f32 angle;
    s32 sound;

    if (obj->speed > D_800ACE68) {
        CQuaternion__FromDirection(&cur, &obj->dir);
        speed = func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z);
        player = (CPlayer *) obj->owner->m_AI.m_pParent;
        home.x = -26.623999f;
        home.y = -20.48f;
        home.z = -143.36f;
        CPlayer__TransformByView(player, &home);
        CVector3__Subtract(&toCatch, &home, &obj->pos);
        dist = func_00298470(toCatch.x * toCatch.x + toCatch.y * toCatch.y + toCatch.z * toCatch.z);
        if (dist <= D_800ACE6C + 30.72f) {
            if (dist < D_800ACE6C * (1.0f / 3) || obj->speed >= 65.0f) {
                obj->pos = home;
                obj->caught = 1;
                if (player->m_Data.m_CurrentWeapon == 0x15) {
                    player->m_WeaponActor.m_AI.m_Weapon.m_BarrelSpeed = 0.73303837f;
                }
                return;
            }
            toHome = toCatch;
        } else {
            aim.x = -102.399994f;
            aim.y = GameObject_GetCollisionHeight(obj->owner) * 0.5f;
            aim.z = -D_800ACE6C;
            CPlayer__TransformByView(player, &aim);
            CVector3__Subtract(&toHome, &aim, &obj->pos);
            func_00298470(toHome.x * toHome.x + toHome.y * toHome.y + toHome.z * toHome.z);
        }
        CVector3__Normalize(&toHome);
        CQuaternion__FromDirection(&want, &toHome);
        CQuaternion__Normalize(&want);
        CQuaternion__Normalize(&cur);
        CVector3__Normalize(&toHome);
        facing = obj->dir;
        CVector3__Normalize(&facing);
        angle = Math_AcosLookup(toHome.x * facing.x + toHome.y * facing.y + toHome.z * facing.z);
        t = (angle < 0.26179942f) ? 1.0f : 0.5f;
        CQuaternion__SlerpFast(&rot, t, &cur, &want);
        CQuaternion__ToMatrix(&rot, mtx);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        CMtxF__TransformPoint(mtx, &forward, &obj->dir);
        CVector3__Scale(&obj->vel, &obj->dir, speed);
    } else {
        loop = obj->loop;
        sound = 0x1B3;
        if (loop != NULL) {
            if (loop->sound == sound && loop->state != -1) {
                goto playing;
            }
            CParticle__StopLoopSound(obj);
        }
        obj->loop = LoopSounds_Start(LoopSounds_Get(), 0x1B3, &obj->pos, &obj->pos, -1);
    }
playing:
    Missile_Accelerate(obj);
    CParticle__Wobble(obj, 1.0f);
}
