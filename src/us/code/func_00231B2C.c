#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"
#include "globals/scene.h"
#include "globals/collision.h"
#include "protos/game.h"

void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);

void CParticle__SpawnAtCeilingHit(CParticle *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (actor->m_pEffect->m_dwFlags & 0x10) {
        origin = gCollisionResults.m_vVelocity;
    } else {
        origin = actor->m_vVelocity;
    }
    CQuaternion__FromDirection(&rot, &origin);
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, origin, rot,
                  gCollisionResults.m_vFloorPos, gCollisionResults.m_pFloorRegion, -2, (actor->m_dwFlags & 0x200006) | 1);
}
