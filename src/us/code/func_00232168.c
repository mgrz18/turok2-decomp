#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"
#include "globals/collision.h"


void CMtxF__FromMtxAffine(CMtxF *out, void *src);
void CMtxF__InvertAffine(CMtxF *out, CMtxF *in);
void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
void CVector3__Normalize(CVector3 *v);
void CMtxF__RotateVector(CMtxF *m, CVector3 *in, CVector3 *out);
void AI_HearNoise(u8 *owner, u8 *ai, void *pos, s32 n);

void CParticle__AttachToHitNode(CParticle *actor, CMtxF *mtx, s32 kind) {
    CMtxF tmp;
    CMtxF m;
    CVector3 dir;
    u8 *owner;

    owner = (u8 *)gCollisionResults.m_pInstance;
    if (mtx != NULL) {
        CMtxF__InvertAffine(&m, mtx);
    } else {
#ifdef TARGET_PORT
        CMtxF__FromMtxAffine(&tmp, (u8 *) ((CGameObjectInstance *) owner)->m_pmtDrawMtxs + gCollisionResults.m_nInstanceNode * 0x40);
#else
        CMtxF__FromMtxAffine(&tmp, *(u8 **) (owner + 0x98) + gCollisionResults.m_nInstanceNode * 0x40);
#endif
        CMtxF__InvertAffine(&m, &tmp);
    }
    CMtxF__TransformPoint(&m, &gCollisionResults.m_vInstancePos, &actor->m_vAttachPos);
    dir = gCollisionResults.m_vVelocity;
    CVector3__Normalize(&dir);
    CMtxF__RotateVector(&m, &dir, &actor->m_vVelocity);
    actor->m_pTarget = (CGameObjectInstance *) owner;
    actor->m_dwFlags |= 0x10000;
#ifdef TARGET_PORT
    /* the low byte (byte 3 on the N64 is byte 0 on a little-endian host); the instance by member */
    actor->m_nBone = (s8) gCollisionResults.m_nInstanceNode;
    if (((CGameObjectInstance *) owner)->header.m_pIntelligence->m_Type == 1) {
        u8 *ai = (u8 *) &((CGameObjectInstance *) owner)->m_AI;
        if (actor->m_pOwner != NULL) {
            AI_HearNoise(owner, ai, &actor->m_pOwner->header.m_vPos, 3);
        } else {
            AI_HearNoise(owner, ai, &gCollisionResults.m_vInstancePos, 3);
        }
    }
#else
    actor->m_nBone = ((u8 *)&gCollisionResults.m_nInstanceNode)[3] /* its low byte */;
    if (**(s32 **) (owner + 0x14) == 1) {
        if (actor->m_pOwner != NULL) {
            AI_HearNoise(owner, owner + 0x140, &actor->m_pOwner->header.m_vPos, 3);
        } else {
            AI_HearNoise(owner, owner + 0x140, &gCollisionResults.m_vInstancePos, 3);
        }
    }
#endif
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}
