#include "common.h"
#include "structs/vector.h"
#include "structs/weapon.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/player.h"

extern f32 gProjectileSpeedScale;          /* projectile speed scale */
extern f32 gProjectilePower;          /* projectile power */
extern u8 gScene_m_ParticleSystem[];

CQuaternion CGameObjectInstance__GetRotation(CPlayer *p);
AmmoInfo *CPlayer__GetFiringAmmo(CPlayer *p, s32 weapon);
void CParticleSystem__Spawn(void *list, CPlayer *src, CPlayer *owner, s32 a3, s32 target, s32 type,
                   CVector3 vel, CQuaternion aim, CVector3 pos, s32 a40, s32 a44, s32 a48);

/* A debug print compiled out of the release build. Its empty loop notes stop
 * loop.c (find_and_verify_loops) from moving the found block out of the
 * projectile loop, as in the ROM. */
#define DEBUG_MSG() do { } while (0)

#define CHARGE(p) if ((p)->m_CharacterActor.m_CurrentAnimation.m_AnimationFrame >= 3.0f) { (p)->m_CharacterActor.m_CurrentAnimation.m_AnimationFrame -= 3.0f; }
/* Whether the weapon fires the given projectile. */
static __inline__ s32 usesProjectile(s32 weapon, s32 type) {
    WeaponInfo *def = gWeaponInfoTable[weapon];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (def->ammo[i] == NULL) {
            break;
        }
        if (def->ammo[i]->projectile == type) {
            DEBUG_MSG();
            return 1;
        }
    }
    return 0;
}

/* Fires the player's weapon: spends the ammo, advances the firing
 * animation, picks the projectile (sniper and charge variants) and
 * launches it from the camera. */
void CPlayer__FireWeapon(CPlayer *p, s32 type) {
    CVector3 pos;
    CVector3 vel;
    CQuaternion aim;
    CCamera *camera = p->m_pCamera;
    AmmoInfo *info;
    s16 *ammo;
    s32 target;
    f32 f;
    s32 found;

    if ((u16) p->m_Mode < 2) {
        return;
    }
    aim = CGameObjectInstance__GetRotation(p);
    info = CPlayer__GetFiringAmmo(p, p->m_Data.m_CurrentWeapon);
    if (info != NULL) {
        found = usesProjectile(p->m_Data.m_CurrentWeapon, type); /* a user variable: loop.c keeps found = 1 in the loop */
        if (found) {
            s32 readyAnim = 1; /* compared below; held in $a2 from here */
            aim = p->m_qAutoAim;
            ammo = &p->m_Data.m_AmmoList[info->type];
            *ammo -= info->cost;
            if (*ammo < 0) {
                *ammo = 0;
            }
            type = info->projectile;
            if (p->m_CharAnim == readyAnim) {
                p->m_CharAnim = 0x1130;
            } else if (p->m_CharAnim == 0x1130) {
                CHARGE(p);
            } else if (p->m_CharAnim == 0xA28) {
                p->m_CharAnim = 0x1135;
            } else if (p->m_CharAnim == 0x1135) {
                CHARGE(p);
            } else if (p->m_CharAnim == 0x14) {
                p->m_CharAnim = 0x1144;
            } else if (p->m_CharAnim == 0x1144) {
                CHARGE(p);
            }
            p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
        }
    }
    gProjectileSpeedScale = gWeaponChargeScale;
    target = 0;
    if (camera != NULL) {
        pos = camera->m_vPos;
    } else {
        pos = p->m_CharacterActor.header.m_vPos;
    }
    vel = p->m_CharacterActor.m_vVelocity;
    if (p->m_CrossHairMode == 2) {
        target = (s32) p->m_pCrossHairTarget;
    }
    gProjectilePower = p->weapon_particle_damage_scaler;
    if (p->m_SniperMode) {
        switch (p->m_Data.m_CurrentWeapon) {
            case 10:
                type = 0x110;
                break;
            case 0x1B:
                type = 0x13A;
                break;
            case 3:
                if (type == 2) {
                    type = 0x111;
                } else if (type == 0x56) {
                    type = 0x126;
                }
                break;
        }
    }
    if (type == 4) {
        f = gProjectilePower * 6.0f;
        type = (s32)((f > 6.0f ? 6.0f : f) + 460.0f);
    }
    CParticleSystem__Spawn(gScene_m_ParticleSystem, p, p, 0, target, type, vel, aim, pos, 0, -1, (p->m_CharacterActor.m_AI.m_dwFlags & 0x80) ? 5 : 1);
    gProjectilePower = p->weapon_particle_damage_scaler = gWeaponChargeScale = gProjectileSpeedScale = 1.0f;
}
