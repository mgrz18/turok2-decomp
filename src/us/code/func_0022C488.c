/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, Particle_HitTarget, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"
#include "structs/particle_event.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/collision.h"
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

extern u8 gEngineApp_m_SwooshSystem[];
extern u8 D_8010FF34[];

s32 func_002682CC(Target *target);
s32 Actor_GetTypeClass(Target *target);
void CMtxF__FromMtxAffine(f32 *out, Bone *bone);
void func_0021B39C(Target *target, s32 *ai, f32 *matrix, CVector3 origin, CVector3 direction, s32 frame);
void CMtxF__InvertAffine(f32 *out, f32 *in);
void CMtxF__TransformPoint(f32 *matrix, CVector3 *in, CVector3 *out);
void CVector3__Normalize(CVector3 *v);
void CMtxF__RotateVector(f32 *matrix, CVector3 *in, CVector3 *out);
void AI_HearNoise(Target *target, s32 *ai, CVector3 *from, s32 arg3);
void CParticle__BuildCameraMatrix(CParticle *p);
void Trail_DetachOwner(void *list, CParticle *p);
void CDLightSet__Free(void *list, CDLight *light);
s32 CParticle__IsUnderWater(CParticle *p);
s32 func_00268148(Target *target);
void ImpactEvent_Send(void *source, CParticle *p, s32 effect, s32 arg3, CVector3 pos, CEventValue params);
void CParticle__SpawnChild(CParticle *p, s32 model, s32 arg2, s32 enabled);
void Sound__Play(s16 sound, CVector3 pos, sptr arg2, sptr arg3);
void CParticleSystem__Free(CParticleSystem *list, CParticle *p);
void CParticle__StopLoopSound(CParticle *p);

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
    if (func_002682CC(source)) {
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
                        Trail_DetachOwner(gEngineApp_m_SwooshSystem, p);
                    }
                    if (p->m_pDLight) {
                        CDLightSet__Free(D_8010FF34, p->m_pDLight);
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
