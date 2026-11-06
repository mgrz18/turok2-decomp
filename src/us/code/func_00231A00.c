#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

extern u8 gScene_m_ParticleSystem[];
extern CCollisionResults gCollisionResults;  /* the last collision test: the spawn lands on its floor hit */
extern CVector3 gCollisionResults_m_vVelocity;
void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
s32 CParticleSystem__Spawn(void *table, void *actor, void *owner, s32 *ownerCount, void *target, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, s32 arg, s32 b, s32 c);

void CParticle__SpawnAtFloorHit(CParticle *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (actor->m_pEffect->m_dwFlags & 0x10) {
        origin = gCollisionResults_m_vVelocity;
    } else {
        origin = actor->m_vVelocity;
    }
    CQuaternion__FromDirection(&rot, &origin);
    CParticleSystem__Spawn(gScene_m_ParticleSystem, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, origin, rot,
                  gCollisionResults.m_vFloorPos, (s32)gCollisionResults.m_pFloorRegion, -3, (actor->m_dwFlags & 0x200006) | 1);
}
