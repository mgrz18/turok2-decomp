#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/particle.h"
#include "structs/sound.h"
#include "protos/game.h"

/* Same family as CParticle__UpdateHomingMissile (homing missile): here a projectile that flies
 * back to a point beside its owner's player and is caught there. */

extern void *LoopSounds_Start(sptr bank, s32 sound, CVector3 *pos, CVector3 *pos2, sptr loops);  /* starts a sound */
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


extern f32 D_800ACE68; /* speed above which it flies home (15) */
extern f32 D_800ACE6C; /* catch radius (512) */

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

    if (obj->m_cFrame > D_800ACE68) {
        CQuaternion__FromDirection(&cur, &obj->m_vDirection);
        speed = func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z);
        player = (CPlayer *) obj->m_pOwner->m_AI.m_pParent;
        home.x = -26.623999f;
        home.y = -20.48f;
        home.z = -143.36f;
        CPlayer__TransformByView(player, &home);
        CVector3__Subtract(&toCatch, &home, &obj->header.m_vPos);
        dist = func_00298470(toCatch.x * toCatch.x + toCatch.y * toCatch.y + toCatch.z * toCatch.z);
        if (dist <= D_800ACE6C + 30.72f) {
            if (dist < D_800ACE6C * (1.0f / 3) || obj->m_cFrame >= 65.0f) {
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
            aim.z = -D_800ACE6C;
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
