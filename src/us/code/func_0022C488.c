/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, Particle_HitTarget, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"
#include "structs/event.h"

typedef struct Bone {
    /* 0x00 */ u8 pad0[0x40];
} Bone;

typedef struct Target Target;
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

extern CCollisionResults D_800F1CE0;
extern u8 D_800F1D5D;
extern u8 D_8011AAF1;
extern u8 D_800F8928[];
extern u8 D_801102B8[];
extern u8 D_8010FF34[];

s32 func_002682CC(Target *target);
s32 func_00245724(Target *target);
void func_0020D908(f32 *out, Bone *bone);
void func_0021B39C(Target *target, s32 *ai, f32 *matrix, CVector3 origin, CVector3 direction, s32 frame);
void func_0020C208(f32 *out, f32 *in);
void func_0020F85C(f32 *matrix, CVector3 *in, CVector3 *out);
void func_0020F040(CVector3 *v);
void func_0020FAFC(f32 *matrix, CVector3 *in, CVector3 *out);
void func_0025E52C(Target *target, s32 *ai, CVector3 *from, s32 arg3);
void func_002327F4(CParticle *p);
void func_00234E48(void *list, CParticle *p);
void func_00228B4C(void *list, CDLight *light);
s32 func_002325F0(CParticle *p);
s32 func_00268148(Target *target);
void func_00236EA0(void *source, CParticle *p, s32 effect, s32 arg3, CVector3 pos, CEventValue params);
void func_00229340(CParticle *p, s32 model, s32 arg2, s32 enabled);
void func_00275544(s16 sound, CVector3 pos, s32 arg2, s32 arg3);
void func_0022F1C8(void *list, CParticle *p);
void func_00232B40(CParticle *p);

/* Sticks the particle to the bone it hit: its position and velocity in the bone's frame. */
static __inline__ void attachParticle(CParticle *p, f32 *transform, s32 action) {
    f32 localTransform[16];
    f32 inverse[16];
    CVector3 direction;
    Target *attached;
    CCollisionResults *scene;

    attached = (Target *)D_800F1CE0.m_pInstance;
    if (transform) {
        func_0020C208(inverse, transform);
    } else {
        func_0020D908(localTransform, &attached->bones[D_800F1CE0.m_nInstanceNode]);
        func_0020C208(inverse, localTransform);
    }
    scene = &D_800F1CE0;
    func_0020F85C(inverse, &scene->m_vInstancePos, &p->m_vAttachPos);
    direction = scene->m_vVelocity;
    func_0020F040(&direction);
    func_0020FAFC(inverse, &direction, &p->m_vVelocity);
    p->m_dwFlags |= 0x10000;
    p->m_pTarget = (CGameObjectInstance *)attached;
    p->m_nBone = D_800F1CE0.m_nInstanceNode;
    if (attached->def->m_Type == 1) {
        if (p->m_pOwner != NULL) {
            func_0025E52C(attached, &attached->aiFlags, &p->m_pOwner->header.m_vPos, 3);
        } else {
            func_0025E52C(attached, &attached->aiFlags, &D_800F1CE0.m_vInstancePos, 3);
        }
    }
    if (p->header.m_nObjType == 0x56 && action == 2) {
        p->m_dwFlags |= 0x4000000;
    }
}

/* A particle hit the current collision target: attaches to it when its type sticks, then
 * plays the impact's model, effects and sound for the hit's action and removes the particle
 * when its descriptor says so. */
void func_0022C488(CParticle *p) {
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

    source = (Target *)D_800F1CE0.m_pInstance;
    enabled = 1;
    special = 0;
    def = p->m_pEffect;
    if (func_002682CC(source)) {
        mode = p->m_bDieOnImpact;
    } else {
        mode = p->m_ObjectImpact;
    }
    if (p->header.m_pCurrentRegion == NULL) {
        p->header.m_pCurrentRegion = (struct CGameRegion *)source->unk10;
    }
    if (D_800F1CE0.m_nInstanceNode != -1) {
        action = D_800F1CE0.m_Frame.action;
        if (p->header.m_nObjType == 13) {
            action = 4;
        }
        owner = (Target *)D_800F1CE0.m_pInstance;
        if (owner->flags & 0x3000000) {
            func_0020D908(boneMatrix, &owner->bones[D_800F1CE0.m_nInstanceNode]);
            if (owner->flags & 0x1000000) {
                func_0021B39C(owner, &owner->aiFlags, boneMatrix, D_800F1CE0.m_vInstancePos, D_800F1CE0.m_vVelocity,
                              D_800F1CE0.m_nInstanceNode);
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
                    || (p->header.m_nObjType == 11 && (D_800F1D5D & 3) == 1)) {
                    attachParticle(p, boneMatrix, action);
                    if (p->m_ActiveSwooshes) {
                        func_002327F4(p);
                        func_00234E48(D_801102B8, p);
                    }
                    if (p->m_pDLight) {
                        func_00228B4C(D_8010FF34, p->m_pDLight);
                        p->m_pDLight = NULL;
                    }
                }
            }
        }
    } else {
        action = func_00245724(source);
    }
    if (action == 5) {
        action = 4;
        p->m_dwFlags |= 2;
    }
    if (func_002325F0(p) && action < 6 && action >= 4) {
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
        if (D_800AD084[effect].m_Flags & 4) {
            if (p->header.m_nObjType == 0x56) {
                primary.lower = primary.lower * p->m_Scale;
            }
        }
        other = source;
        if (D_800AD084[effect].m_Flags & 2) {
            other = p;
        }
        func_00236EA0(other, p, effect, -1, D_800F1CE0.m_vInstancePos, primary);
    }
    if (action != 4) {
        draw = 1;
        if (D_8011AAF1) {
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
                if (D_800AD084[extraEffect].m_Flags & 4) {
                    other = source;
                    if (D_800AD084[extraEffect].m_Flags & 2) {
                        other = p;
                    }
                    func_00236EA0(other, p, extraEffect, -1, D_800F1CE0.m_vInstancePos, secondary);
                }
            }
        }
    }
    if (model != 0xFFFF) {
        func_00229340(p, model, -8, enabled);
    }
    if (sound != 0xFFFF) {
        func_00275544((s16) sound, D_800F1CE0.m_vInstancePos, 0, -1);
    }
    if (!(p->m_dwFlags & 0x30000)) {
        if (mode == 1 || (effect != 0xFFFF && (D_800AD084[effect].m_Flags & 1))) {
            func_0022F1C8(D_800F8928, p);
            func_00232B40(p);
        }
    }
}
