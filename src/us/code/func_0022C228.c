/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027C9CC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"

extern s32 func_0026C114(void *region);
extern s32 D_800F1D94;
extern CVector3 D_800F1D98;
extern void func_00236EA0(CParticle *, CParticle *, s32 effect, s32, CVector3 pos, CEventValue params);
extern void func_0020E810(CQuaternion *out, CVector3 *dir);          /* rotation facing dir */
extern void func_0022F350(void *sys, CParticle *p, void *owner, s32 *ownerCount, void *target, s32 type, CVector3 dir,
                          CQuaternion rot, CVector3 pos, s32 region, s32 arg10, s32 flags);
extern void func_00275544(s16 sound, CVector3 pos, s32, s32);
extern void func_0020EF60(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void func_0022F1C8(void *sys, CParticle *p);
extern void func_00232B40(CParticle *p);
extern CVector3 D_800F1DB8;
extern u8 D_800F8928[];

void func_0022C228(CParticle *arg0) {
    CEventValue pair;
    CQuaternion rotation;
    CVector3 position;
    s32 surface;
    s32 index;
    CROMParticleImpact *events;
    s32 particle;
    s32 effect;
    s32 sound;
    CROMParticleEffect *desc;

    surface = func_0026C114(arg0->header.m_pCurrentRegion);
    if (surface == 10) {
        surface = 0;
    }
    desc = arg0->m_pEffect;
    index = surface;
    events = desc->m_pImpact;
    particle = events->m_ImpactParticleType[index];
    effect = events->m_ImpactEventType[index];
    pair = events->m_ImpactEventValue[surface];
    sound = desc->m_pImpact->m_ImpactSoundType[index];
    if (effect != 0xFFFF) {
        func_00236EA0(arg0, arg0, effect, -1, D_800F1D98, pair);
    }
    if (particle != 0xFFFF) {
        if ((arg0->m_pEffect->m_dwFlags & 0x10) != 0) {
            position = D_800F1DB8;
        } else {
            position = arg0->m_vVelocity;
        }
        func_0020E810(&rotation, &position);
        func_0022F350(D_800F8928, arg0, arg0->m_pOwner, arg0->m_pOwnerCount, arg0->m_pTarget, particle, position,
                      rotation, D_800F1D98, D_800F1D94, -2, (arg0->m_dwFlags & 0x200006) | 1);
    }
    if (sound != 0xFFFF) {
        func_00275544((s16)sound, D_800F1D98, 0, -1);
    }
    func_0020EF60(&arg0->m_vRotSpeed, &arg0->m_vRotSpeed, arg0->m_RotationScale);
    arg0->m_dwFlags |= 0x200;
    if (arg0->m_bDieOnBounce == 1) {
        func_0022F1C8(D_800F8928, arg0);
        func_00232B40(arg0);
    }
}
