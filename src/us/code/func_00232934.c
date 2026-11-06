#include "common.h"
#include "structs/particle.h"

extern u8 D_800F8928[];
void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
s32 CParticleSystem__Spawn(void *table, void *actor, void *owner, s32 *ownerCount, void *target, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, void *region, s32 b, s32 c);

/* Spawns `count` copies along the segment the actor covered this frame. */
void CParticle__SpawnAlongPath(CParticle *actor) {
    CQuaternion rot;
    CVector3 delta;
    CVector3 pos;
    CVector3 dir;
    s32 count;
    s32 kind;
    s32 i;
    u8 *table;

    count = actor->m_pEffect->m_pParams->m_nTrailCount;
    if (count > 0) {
        dir = actor->m_vVelocity;
        CQuaternion__FromDirection(&rot, &dir);
        CVector3__Subtract(&delta, &actor->header.m_vPos, &actor->m_vLastPos);
        table = D_800F8928;
        for (i = 1; i <= count; i++) {
            CVector3__Scale(&pos, &delta, i / (count + 1.0f));
            CVector3__Add(&pos, &pos, &actor->m_vLastPos);
            kind = actor->header.m_nObjType;
            CParticleSystem__Spawn(table, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, dir, rot,
                          pos, actor->header.m_pCurrentRegion, -1, 0x20E001);
        }
    }
}
