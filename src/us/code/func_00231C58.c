#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

extern u8 D_800F8928[];
extern CCollisionResults D_800F1CE0;  /* the last collision test: the spawn lands on its water hit */
extern CVector3 D_800F1DB8;
void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
s32 CParticleSystem__Spawn(void *table, void *actor, void *owner, s32 *ownerCount, void *target, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, s32 arg, s32 b, s32 c);

void CParticle__SpawnAtWaterHit(CParticle *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (actor->m_pEffect->m_dwFlags & 0x10) {
        origin = D_800F1DB8;
    } else {
        origin = actor->m_vVelocity;
    }
    CQuaternion__FromDirection(&rot, &origin);
    CParticleSystem__Spawn(D_800F8928, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, origin, rot,
                  D_800F1CE0.m_vWaterPos, (actor->m_dwHitFlags & 0x1000000) ? 0 : (s32)D_800F1CE0.m_pWaterRegion, -5, (actor->m_dwFlags & 0x200006) | 1);
}
