#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

extern CCollisionResults gCollisionResults;
void CVector3__Normalize(CVector3 *v);
f32 Random_Blend(f32 a, f32 b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);

void CParticle__PlaceAtInstanceHit(CParticle *actor, s32 kind) {
    CVector3 dir;

    actor->m_dwFlags |= 0x20000;
    actor->header.m_vPos = gCollisionResults.m_vInstancePos;
    actor->m_vVelocity = gCollisionResults.m_vVelocity;
    dir = gCollisionResults.m_vVelocity;
    CVector3__Normalize(&dir);
    actor->m_vDirection = dir;
    CVector3__Scale(&dir, &dir, Random_Blend(-49.92f, 0.0f));
    CVector3__Add(&actor->header.m_vPos, &actor->header.m_vPos, &dir);
    actor->m_Shake = 0x7F;
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}
