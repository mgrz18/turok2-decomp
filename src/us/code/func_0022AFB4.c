/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027B720_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"
#include "structs/player.h"
#include "globals/engine.h"

typedef struct Box {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

typedef struct ContactHandler ContactHandler;
struct ContactHandler {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0x10 - 6];
    /* 0x10 */ s32 (*func)(CPlayer *player, ContactHandler *handler);
};

extern CCollisionResults gCollisionResults;
extern CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;              /* current player */
extern ContactHandler sParticleAmmoPickups[4];
extern CCollisionResults *gCollisionResultsP;
extern u8 gScene_m_ParticleSystem[];

s32 Random_Next(void);
s32 Collision_MoveInstance(CParticle *p, CVector3 pos, u32 *flags);
f32 GameObject_GetCollisionHeight(CPlayer *player);
f32 GameObject_GetCollisionRadius(CPlayer *player);
f32 func_00298470(f32 x);
void CParticle__FireExpireEvent(CParticle *p);
void CParticleSystem__Free(void *list, CParticle *p);
void CParticle__StopLoopSound(CParticle *p);
void CParticle__FireUpdateEvent(CParticle *p);
void CParticle__Steer(CParticle *p);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Normalize(CVector3 *v);
void CParticle__ReportCollision(CParticle *p);
void CParticle__HitWater(CParticle *p);
void CParticle__HitWall(CParticle *p);
void CParticle__HitInstanceBody(CParticle *p);
void CParticle__HitTarget(CParticle *p);
void CParticle__HitFloor(CParticle *p);
void CParticle__HitCeiling(CParticle *p);
void CParticle__SpawnAlongPath(CParticle *p);

#define MIN(a, b) ((a) > (b) ? (b) : (a))

/* Updates a particle for one frame: measures its bounding box, kills it or bounces it off its
 * target when its type's rule says so, lets the players it touches react to it, advances its
 * size and spin (dying when a size goes negative) and, when it moves or falls, probes the
 * ground, orients itself to the surface it hit and runs the collision responses its
 * descriptor allows. */
void CParticle__Update(CParticle *p) {
    Box bounds;
    CGameObjectInstance *target;
    CPlayer *player;
    ContactHandler *handler;
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
         * 0.0f of the time check into the code below, as in the ROM. */
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
    bounds.min.x = p->header.m_vPos.x - half;
    bounds.max.x = p->header.m_vPos.x + half;
    bounds.min.y = p->header.m_vPos.y - half;
    bounds.max.y = p->header.m_vPos.y + scaled;
    bounds.min.z = p->header.m_vPos.z - half;
    bounds.max.z = p->header.m_vPos.z + half;
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
        for (player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead; player != NULL; player = player->m_pNext) {
            if (player->m_Data.m_Health != 0 && player->m_Mode != 2) {
                top = player->m_CharacterActor.header.m_vPos.y + player->m_CharacterActor.m_CollisionYOffset;
                if (top - 20.479999f <= p->header.m_vPos.y && p->header.m_vPos.y <= top + GameObject_GetCollisionHeight(player) + 20.479999f) {
                    dist = func_00298470((player->m_CharacterActor.header.m_vPos.x - p->header.m_vPos.x) * (player->m_CharacterActor.header.m_vPos.x - p->header.m_vPos.x)
                                         + (player->m_CharacterActor.header.m_vPos.z - p->header.m_vPos.z) * (player->m_CharacterActor.header.m_vPos.z - p->header.m_vPos.z));
                    if (dist < GameObject_GetCollisionRadius(player) + 51.199997f) {
                        handler = sParticleAmmoPickups;
                        for (i = 3; i != -1; handler++, i--) {
                            if (handler->type == p->header.m_nObjType && handler->func(player, handler)) {
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
            CParticleSystem__Free(gScene_m_ParticleSystem, p);
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
