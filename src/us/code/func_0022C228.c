/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027C9CC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"
#include "globals/scene.h"
#include "globals/collision.h"
#include "protos/game.h"

extern s32 func_0026C114(void *region);
/* gCollisionResults_m_pFloorRegion / gCollisionResults_m_vFloorPos are CCollisionResults (gCollisionResults, structs/collision.h)
 * m_pFloorRegion and m_vFloorPos; kept as their own symbols: reading them
 * through gCollisionResults does not give this code. */
extern void ImpactEvent_Send(CParticle *, CParticle *, s32 effect, s32, CVector3 pos, CEventValue params);
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);          /* rotation facing dir */
extern void Sound__Play(s16 sound, CVector3 pos, s32, s32);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CParticleSystem__Free(CParticleSystem *sys, CParticle *p);
extern void CParticle__StopLoopSound(CParticle *p);

void CParticle__HitCeiling(CParticle *arg0) {
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
        ImpactEvent_Send(arg0, arg0, effect, -1, gCollisionResults_m_vFloorPos, pair);
    }
    if (particle != 0xFFFF) {
        if ((arg0->m_pEffect->m_dwFlags & 0x10) != 0) {
            position = gCollisionResults_m_vVelocity;
        } else {
            position = arg0->m_vVelocity;
        }
        CQuaternion__FromDirection(&rotation, &position);
        CParticleSystem__Spawn(&gScene.m_ParticleSystem, arg0, arg0->m_pOwner, arg0->m_pOwnerCount, arg0->m_pTarget, particle, position,
                      rotation, gCollisionResults_m_vFloorPos, gCollisionResults_m_pFloorRegion, -2, (arg0->m_dwFlags & 0x200006) | 1);
    }
    if (sound != 0xFFFF) {
        Sound__Play((s16)sound, gCollisionResults_m_vFloorPos, 0, -1);
    }
    CVector3__Scale(&arg0->m_vRotSpeed, &arg0->m_vRotSpeed, arg0->m_RotationScale);
    arg0->m_dwFlags |= 0x200;
    if (arg0->m_bDieOnBounce == 1) {
        CParticleSystem__Free(&gScene.m_ParticleSystem, arg0);
        CParticle__StopLoopSound(arg0);
    }
}
