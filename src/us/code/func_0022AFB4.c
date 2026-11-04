/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027B720_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

typedef struct Box {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

typedef struct Target {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x10 - 1];
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[0xD4 - 0x14];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ s32 unk140;
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x174 - 0x148];
    /* 0x174 */ s8 state;
} Target;

typedef struct Player Player;
struct Player {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x524 - 0x58];
    /* 0x0524 */ s32 health;
    /* 0x0528 */ u8 pad528[0xA08 - 0x528];
    /* 0x0A08 */ s16 mode;
    /* 0x0A0A */ u8 padA0A[0x1320 - 0xA0A];
    /* 0x1320 */ Player *next;
};

typedef struct ContactHandler ContactHandler;
struct ContactHandler {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0x10 - 6];
    /* 0x10 */ s32 (*func)(Player *player, ContactHandler *handler);
};

extern f32 D_800B6D28;                  /* the frame's time step */
extern f32 D_800B6D30;
extern CCollisionResults D_800F1CE0;
extern Player *D_80119890;              /* current player */
extern ContactHandler D_800ACE18[4];
extern CCollisionResults *D_800F1ADC;
extern u8 D_800F8928[];

s32 func_002113A4(void);
s32 func_00239F00(CParticle *p, CVector3 pos, u32 *flags);
f32 func_002671B4(Player *player);
f32 func_002672C8(Player *player);
f32 func_00298470(f32 x);
void func_002324E8(CParticle *p);
void func_0022F1C8(void *list, CParticle *p);
void func_00232B40(CParticle *p);
void func_002323E0(CParticle *p);
void func_0022A4C8(CParticle *p);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0020F040(CVector3 *v);
void func_0022AAE0(CParticle *p);
void func_0022BD10(CParticle *p);
void func_0022BA88(CParticle *p);
void func_0022CA90(CParticle *p);
void func_0022C488(CParticle *p);
void func_0022BFA0(CParticle *p);
void func_0022C228(CParticle *p);
void func_00232934(CParticle *p);

#define MIN(a, b) ((a) > (b) ? (b) : (a))

/* Updates a particle for one frame: measures its bounding box, kills it or bounces it off its
 * target when its type's rule says so, lets the players it touches react to it, advances its
 * size and spin (dying when a size goes negative) and, when it moves or falls, probes the
 * ground, orients itself to the surface it hit and runs the collision responses its
 * descriptor allows. */
void func_0022AFB4(CParticle *p) {
    Box bounds;
    Target *target;
    Player *player;
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
        half = p->m_Shake - D_800B6D28 * 6.0f;
        fade = (half < 0.0f) ? 0 : (s32) half;
        p->m_Shake = fade;
    }
    if (p->m_dwFlags & 0x10000) {
        target = (Target *)p->m_pTarget;
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
                if (target->health == 0) {
                    bounce = 1;
                }
                break;
            case 2:
            case 0xF:
            case 0x56:
            case 0x126:
                if (p->m_dwFlags & 0x4000000) {
                    die = 1;
                } else if (!(target->flags & 0x100)) {
                    die = 1;
                } else if (target->flags & 0x300000) {
                    die = (f32) target->health <= 0.0f;
                } else {
                    die = target->unk140 & 0x20;
                }
                bounce = 0;
                break;
            case 0xB:
                die = 0;
                if (target->state != 0x21 || !(target->flags & 0x100)) {
                    bounce = 1;
                }
                break;
            case 4:
                die = 0;
                if (target->state != 0x20 || !(target->flags & 0x100)) {
                    bounce = 1;
                }
                break;
            case 0x2D:
            case 0x4F:
                bounce = !(target->flags & 0x100);
                break;
        }
        if (die || bounce) {
            p->m_cFrame = 0.0f;
            p->m_Alignment = -7;
            p->m_dwFlags = (p->m_dwFlags & ~0x30000) | 0x40000;
            p->m_vRot.z = (f32) (func_002113A4() % 360) * 0.017453294f;
            p->m_vVelocity.y = 0.0f;
            if (p->m_dwFlags & 0x4000000) {
                p->m_vVelocity.x = -p->m_vDirection.x;
                p->m_vVelocity.z = -p->m_vDirection.z;
            } else {
                p->m_vVelocity.x = 0.0f;
                p->m_vVelocity.z = 0.0f;
            }
            p->header.m_pCurrentRegion = (struct CGameRegion *)target->unk10;
            p->m_dwHitFlags &= ~0x1000000;
            if (bounce) {
                p->m_nFrames = 1;
            }
        }
    }
    if ((p->m_dwFlags & 0x40000) && func_00239F00(p, p->header.m_vPos, &p->m_dwHitFlags) && D_800F1CE0.m_pFloorRegion != 0) {
        p->m_Alignment = -3;
    }
    if (p->m_dwFlags & 0x60000) {
        for (player = D_80119890; player != NULL; player = player->next) {
            if (player->health != 0 && player->mode != 2) {
                top = player->pos.y + player->height;
                if (top - 20.479999f <= p->header.m_vPos.y && p->header.m_vPos.y <= top + func_002671B4(player) + 20.479999f) {
                    dist = func_00298470((player->pos.x - p->header.m_vPos.x) * (player->pos.x - p->header.m_vPos.x)
                                         + (player->pos.z - p->header.m_vPos.z) * (player->pos.z - p->header.m_vPos.z));
                    if (dist < func_002672C8(player) + 51.199997f) {
                        handler = D_800ACE18;
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
    p->m_AnimFrame += D_800B6D28 * p->m_AnimSpeed * (D_800B6D30 * 0.06666667f);
    if (p->m_pEffect->m_dwFlags & 0x1000) {
        step = D_800B6D28 * 10.24f;
        p->m_vSize.x += step * p->m_vGrowth.x;
        p->m_vSize.y += step * p->m_vGrowth.y;
        p->m_vSize.z += D_800B6D28 * p->m_vGrowth.z;
        p->m_vSize.x = (p->m_vSize.x > 4096.0f) ? 4096.0f : p->m_vSize.x;
        p->m_vSize.y = (p->m_vSize.y > 4096.0f) ? 4096.0f : p->m_vSize.y;
        p->m_vSize.z = (p->m_vSize.z > 400.0f) ? 400.0f : p->m_vSize.z;
    } else {
        grow = MIN(p->m_vSize.x * p->m_vGrowth.x, 4096.0f);
        p->m_vSize.x += (grow - p->m_vSize.x) * D_800B6D28;
        grow = MIN(p->m_vSize.y * p->m_vGrowth.y, 4096.0f);
        p->m_vSize.y += (grow - p->m_vSize.y) * D_800B6D28;
        grow = MIN(p->m_vSize.z * p->m_vGrowth.z, 400.0f);
        p->m_vSize.z += (grow - p->m_vSize.z) * D_800B6D28;
        if (p->m_vSize.x < 0.0f || p->m_vSize.y < 0.0f || p->m_vSize.z < 0.0f) {
            func_002324E8(p);
            func_0022F1C8(D_800F8928, p);
            func_00232B40(p);
            return;
        }
    }
    p->m_vRot.x += p->m_vRotSpeed.x * D_800B6D28;
    p->m_vRot.y += p->m_vRotSpeed.y * D_800B6D28;
    p->m_vRot.z += p->m_vRotSpeed.z * D_800B6D28;
    D_800F1ADC->m_vVelocity = p->m_vVelocity;
    func_002323E0(p);
    func_0022A4C8(p);
    if ((p->m_Gravity == 0.0f || (p->header.m_vPos.y == 0.0f && p->m_Gravity < 0.0f))
        && p->m_vVelocity.x * p->m_vVelocity.x + p->m_vVelocity.y * p->m_vVelocity.y + p->m_vVelocity.z * p->m_vVelocity.z < 0.001f) {
        return;
    }
    if (!(p->m_dwFlags & 0x72000)) {
        die = func_00239F00(p, p->header.m_vPos, &p->m_dwHitFlags);
        if (p->header.m_nObjType == 0x68 && D_800F1CE0.m_pInstance != NULL) {
            p->m_pTarget = (CGameObjectInstance *)D_800F1CE0.m_pInstance;
        }
        if (die) {
            if (D_800F1CE0.m_pInstance != NULL) {
                func_0020EF2C(&p->m_vDirection, &p->header.m_vPos, &D_800F1CE0.m_vInstancePos);
            } else if (D_800F1CE0.m_pShape != 0) {
                func_0020EF2C(&p->m_vDirection, &p->header.m_vPos, &D_800F1CE0.m_vShapePos);
            } else if (D_800F1CE0.m_pWallRegion != 0) {
                func_0020EF2C(&p->m_vDirection, &p->header.m_vPos, &D_800F1CE0.m_vWallPos);
            } else if (D_800F1CE0.m_pFloorRegion != 0) {
                func_0020EF2C(&p->m_vDirection, &p->header.m_vPos, &D_800F1CE0.m_vFloorPos);
                if (p->m_Alignment == -7) {
                    p->m_Alignment = -3;
                }
            } else if (D_800F1CE0.m_pWaterRegion != 0) {
                func_0020EF2C(&p->m_vDirection, &p->header.m_vPos, &D_800F1CE0.m_vWaterPos);
            }
            func_0020F040(&p->m_vDirection);
        }
        func_0022AAE0(p);
        if (D_800F1CE0.m_pWaterRegion != 0) {
            func_0022BD10(p);
        }
        if (p->m_dwFlags & 0x100) {
            if (D_800F1CE0.m_pWallRegion != 0) {
                if (!(p->m_pEffect->m_dwFlags & 0x10000000)) {
                    func_0022BA88(p);
                }
                if (p->m_pEffect->m_dwFlags & 0x800) {
                    p->m_bDieOnImpact = 1;
                }
            }
            if (p->m_dwFlags & 0x100) {
                if (D_800F1CE0.m_pInstance != NULL) {
                    if ((p->m_dwHitFlags & 0x1000000) && D_800F1CE0.m_pInstance->header.m_Type == 0) {
                        func_0022CA90(p);
                    } else {
                        func_0022C488(p);
                    }
                }
                if ((p->m_dwFlags & 0x100) && D_800F1CE0.m_nFloorKind != 0 && !(p->m_pEffect->m_dwFlags & 0x20000000)) {
                    if (D_800F1CE0.m_nFloorKind == 1) {
                        func_0022BFA0(p);
                    } else {
                        func_0022C228(p);
                    }
                }
            }
        }
    }
    if ((p->m_dwFlags & 0x8100) == 0x100) {
        func_00232934(p);
    }
}
