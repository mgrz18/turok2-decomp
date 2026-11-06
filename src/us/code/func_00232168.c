#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

typedef struct { f32 m[4][4]; } Mtx4;

extern CCollisionResults gCollisionResults;  /* the last collision test: the spawn is placed at its hit */
void CMtxF__FromMtxAffine(Mtx4 *out, void *src);
void CMtxF__InvertAffine(Mtx4 *out, Mtx4 *in);
void CMtxF__TransformPoint(Mtx4 *m, CVector3 *in, CVector3 *out);
void CVector3__Normalize(CVector3 *v);
void CMtxF__RotateVector(Mtx4 *m, CVector3 *in, CVector3 *out);
void AI_HearNoise(u8 *owner, u8 *ai, void *pos, s32 n);

void CParticle__AttachToHitNode(CParticle *actor, Mtx4 *mtx, s32 kind) {
    Mtx4 tmp;
    Mtx4 m;
    CVector3 dir;
    u8 *owner;

    owner = (u8 *)gCollisionResults.m_pInstance;
    if (mtx != NULL) {
        CMtxF__InvertAffine(&m, mtx);
    } else {
        CMtxF__FromMtxAffine(&tmp, *(u8 **) (owner + 0x98) + gCollisionResults.m_nInstanceNode * 0x40);
        CMtxF__InvertAffine(&m, &tmp);
    }
    CMtxF__TransformPoint(&m, &gCollisionResults.m_vInstancePos, &actor->m_vAttachPos);
    dir = gCollisionResults.m_vVelocity;
    CVector3__Normalize(&dir);
    CMtxF__RotateVector(&m, &dir, &actor->m_vVelocity);
    actor->m_pTarget = (CGameObjectInstance *) owner;
    actor->m_dwFlags |= 0x10000;
    actor->m_nBone = ((u8 *)&gCollisionResults.m_nInstanceNode)[3] /* its low byte */;
    if (**(s32 **) (owner + 0x14) == 1) {
        if (actor->m_pOwner != NULL) {
            AI_HearNoise(owner, owner + 0x140, &actor->m_pOwner->header.m_vPos, 3);
        } else {
            AI_HearNoise(owner, owner + 0x140, &gCollisionResults.m_vInstancePos, 3);
        }
    }
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}
