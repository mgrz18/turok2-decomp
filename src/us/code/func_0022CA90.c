/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027D4A8_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

typedef struct Target Target;

typedef struct EffectInfo {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[4];
} EffectInfo;

extern CCollisionResults D_800F1CE0;  /* the last collision test (m_pInstance: what it hit) */
extern CVector3 D_800F1DB8;                /* the direction of the hit */
extern u8 D_8011AAF1;
extern EffectInfo D_800AD084[];
extern u8 D_800F8928[];
extern u8 D_801102B8[];
extern u8 D_8010FF34[];

s32 func_00245724(Target *target);
s32 func_002325F0(CParticle *p);
void func_00236EA0(void *source, CParticle *p, s32 effect, s32 arg3, CVector3 pos, CEventValue params);
void func_00229340(CParticle *p, s32 model, s32 arg2, s32 enabled);
void func_00275544(s16 sound, CVector3 pos, s32 arg2, s32 arg3);
void func_002364A4(void *list, CVector3 pos, CVector3 normal, s32 kind, s32 arg4);
void func_0020F040(CVector3 *v);
f32 func_002119FC(f32 lo, f32 hi);
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);
void func_002327F4(CParticle *p);
void func_00234E48(void *list, CParticle *p);
void func_00228B4C(void *list, CDLight *light);
void func_0022F1C8(void *list, CParticle *p);
void func_00232B40(CParticle *p);

/* Puts the particle at the hit point, flying along the hit direction, and pulls it back
 * along that direction by a random distance. */
static __inline__ void placeAtHit(CParticle *p) {
    CVector3 dir;

    p->header.m_vPos = D_800F1CE0.m_vInstancePos;
    p->m_vVelocity = D_800F1DB8;
    dir = D_800F1DB8;
    func_0020F040(&dir);
    p->m_vDirection = dir;
    func_0020EF60(&dir, &dir, func_002119FC(-49.92f, 0.0f));
    func_0020EEF8(&p->header.m_vPos, &p->header.m_vPos, &dir);
}

/* A particle hit the world (no target): leaves a decal, sticks or bounces off the surface
 * by type, plays the impact's effect, model and sound, and dies when its descriptor says so.
 * A particle under player control plays its own impact instead and always dies. */
void func_0022CA90(CParticle *p) {
    CEventValue params;
    CVector3 normal;
    CEventValue controlledParams;
    Target *source;
    CROMParticleEffect *definition;
    s32 kind;

    source = (Target *)D_800F1CE0.m_pInstance;
    definition = p->m_pEffect;
    kind = func_00245724(source);
    if (kind == 5) {
        kind = 4;
        p->m_dwFlags |= 2;
    }
    if (D_8011AAF1 != 0 && func_002325F0(p)) {
        CROMParticleEffect *controlledDefinition = p->m_pEffect;
        s32 controlledKind, model, sound, effect;

        if (func_002325F0(p)) {
            controlledKind = 12;
        } else {
            controlledKind = 10;
        }
        model = controlledDefinition->m_pImpact->m_ImpactParticleType[controlledKind];
        effect = controlledDefinition->m_pImpact->m_ImpactEventType[controlledKind];
        controlledParams = controlledDefinition->m_pImpact->m_ImpactEventValue[controlledKind];
        sound = controlledDefinition->m_pImpact->m_ImpactSoundType[controlledKind];
        if (effect != 0xFFFF) {
            func_00236EA0(p, p, effect, -1, p->header.m_vPos, controlledParams);
        }
        if (model != 0xFFFF) {
            func_00229340(p, model, p->m_Alignment, 1);
        }
        if (sound != 0xFFFF) {
            func_00275544(sound, p->header.m_vPos, 0, -1);
        }
        goto remove;
    }
    switch (kind) {
        case 0:
        case 2:
        case 3:
            if (definition->m_dwFlags & 0x800000) {
                func_002364A4(D_801102B8, D_800F1CE0.m_vInstancePos, D_800F1CE0.m_vNormal, kind, 0);
            }
            break;
    }
    if (p->header.m_nObjType == 0x56 || p->header.m_nObjType == 0x126 || p->header.m_nObjType == 0xCD || p->header.m_nObjType == 0xF) {
        p->m_dwFlags |= 0x20000;
        placeAtHit(p);
        p->m_Shake = 0x7F;
        if (p->header.m_nObjType == 0x56 && kind == 2) {
            p->m_dwFlags |= 0x4000000;
        }
        if (p->m_ActiveSwooshes) {
            func_002327F4(p);
            func_00234E48(D_801102B8, p);
        }
        if (p->m_pDLight) {
            func_00228B4C(D_8010FF34, p->m_pDLight);
            p->m_pDLight = NULL;
        }
    }
    normal = D_800F1CE0.m_vNormal;
    func_0020F040(&normal);
    func_0020EF60(&normal, &normal, 1.024f);
    func_0020EEF8(&p->header.m_vPos, &p->header.m_vPos, &normal);
    {
        s32 model, sound, effect;

        model = definition->m_pImpact->m_ImpactParticleType[kind];
        effect = definition->m_pImpact->m_ImpactEventType[kind];
        params = definition->m_pImpact->m_ImpactEventValue[kind];
        sound = definition->m_pImpact->m_ImpactSoundType[kind];
        if (effect != 0xFFFF) {
            void *eventSource = source;

            if (D_800AD084[effect].flags & 2) {
                eventSource = p;
            }
            func_00236EA0(eventSource, p, effect, -1, D_800F1CE0.m_vInstancePos, params);
        }
        if (model != 0xFFFF) {
            func_00229340(p, model, -8, 1);
        }
        if (sound != 0xFFFF) {
            func_00275544(sound, D_800F1CE0.m_vInstancePos, 0, -1);
        }
    }
    if (!(p->m_dwFlags & 0x30000) && p->m_bDieOnImpact == 1) {
remove:
        func_0022F1C8(D_800F8928, p);
        func_00232B40(p);
    }
}
