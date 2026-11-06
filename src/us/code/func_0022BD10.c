/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027C4F0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"

extern s32 D_800F1DA4;
extern CVector3 D_800F1DA8;
extern s32 D_800F1DB4;
extern void ImpactEvent_Send(CParticle *, CParticle *, s32 effect, s32, CVector3 pos, CEventValue params);
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);          /* rotation facing dir */
extern void CParticleSystem__Spawn(void *sys, CParticle *p, void *owner, s32 *ownerCount, void *target, s32 type, CVector3 dir,
                          CQuaternion rot, CVector3 pos, s32 region, s32 arg10, s32 flags);
extern void Sound__Play(s16 sound, CVector3 pos, s32, s32);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CParticleSystem__Free(void *sys, CParticle *p);
extern void CParticle__StopLoopSound(CParticle *p);
extern CVector3 D_800F1DB8;
extern u8 D_800F8928[];

void CParticle__HitWater(CParticle *arg0) {
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

    switch (D_800F1DB4) {
    case 1:
    default:
        surface = 1;
        break;
    case 7:
        surface = 7;
        break;
    case 8:
        surface = 8;
        break;
    }
    arg0->m_dwFlags ^= 4;
    desc = arg0->m_pEffect;
    index = surface;
    events = desc->m_pImpact;
    particle = events->m_ImpactParticleType[index];
    effect = events->m_ImpactEventType[index];
    pair = events->m_ImpactEventValue[surface];
    sound = desc->m_pImpact->m_ImpactSoundType[index];
    if (effect != 0xFFFF) {
        ImpactEvent_Send(arg0, arg0, effect, -1, D_800F1DA8, pair);
    }
    if (particle != 0xFFFF) {
        if ((arg0->m_pEffect->m_dwFlags & 0x10) != 0) {
            position = D_800F1DB8;
        } else {
            position = arg0->m_vVelocity;
        }
        CQuaternion__FromDirection(&rotation, &position);
        CParticleSystem__Spawn(D_800F8928, arg0, arg0->m_pOwner, arg0->m_pOwnerCount, arg0->m_pTarget, particle, position,
                      rotation, D_800F1DA8, (arg0->m_dwHitFlags & 0x1000000) ? 0 : D_800F1DA4, -5, (arg0->m_dwFlags & 0x200006) | 1);
    }
    if (sound != 0xFFFF) {
        Sound__Play((s16)sound, D_800F1DA8, 0, -1);
    }
    arg0->m_dwFlags |= 0x200;
    if ((arg0->m_pEffect->m_dwFlags & 0x20000) != 0) {
        CParticleSystem__Free(D_800F8928, arg0);
        CParticle__StopLoopSound(arg0);
    }
}
