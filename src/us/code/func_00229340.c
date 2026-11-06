/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/80279208_11.CPP, func_80279B40_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);          /* rotation facing dir */
extern void CMtxF__SetRotateZ(CMtxF m, s32 angle);              /* rotation about Y */
extern void CMtxF__TransformPoint(CMtxF m, CVector3 *in, CVector3 *out);  /* transform */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */
extern CMtxF *CParticle__GetParentTransform(CParticle *p, s32 arg1);         /* parent transform */
extern void GameObject_TraceMove(CParticle *p, CVector3 to, CVector3 *outPos, void **outRegion, s32, s32);
extern void CParticleSystem__Spawn(void *sys, CParticle *p, void *owner, s32 *ownerCount, void *target, s32 model, CVector3 dir,
                          CQuaternion rot, CVector3 pos, void *region, s32 arg2, s32 flags);
extern CVector3 D_800F1DB8;
extern CCollisionResults *D_800F1ADC;
extern CCollisionResults D_800F1CE0;
extern u8 D_800F8928[];

static __inline__ f32 offset_x(CParticle *p) {
    if (p->m_pEffect->m_pModel != NULL) {
        return 0.0f;
    }
    return p->m_vRot.x;
}

static __inline__ f32 offset_y(CParticle *p) {
    if (p->m_pEffect->m_pModel != NULL) {
        return 0.0f;
    }
    return p->m_vRot.y;
}

void CParticle__SpawnChild(CParticle *p, s32 model, s32 arg2, s32 flags) {
    CQuaternion rotation;
    CVector3 direction;
    CVector3 offset;
    CVector3 turned;
    CVector3 carried;
    CVector3 point;
    CVector3 moved;
    CMtxF matrix;
    CCollisionResults scratch;
    void *region;
    CMtxF *parent;

    if (p->m_pEffect->m_dwFlags & 0x10) {
        direction = D_800F1DB8;
    } else {
        direction = p->m_vVelocity;
    }
    CQuaternion__FromDirection(&rotation, &direction);
    if (offset_x(p) == 0.0f && offset_y(p) == 0.0f) {
        point = p->header.m_vPos;
        region = p->header.m_pCurrentRegion;
    } else {
        offset.x = -offset_x(p);
        offset.y = -offset_y(p);
        offset.z = 0.0f;
        CMtxF__SetRotateZ(matrix, *(s32 *)&p->m_vRot.z);
        CMtxF__TransformPoint(matrix, &offset, &turned);
        parent = CParticle__GetParentTransform(p, 0);
        if (parent != NULL) {
            CMtxF__TransformPoint(*parent, &turned, &carried);
        } else {
            carried = turned;
        }
        CVector3__Add(&moved, &carried, &p->header.m_vPos);
        D_800F1ADC = &scratch;
        if (p->header.m_pCurrentRegion != NULL) {
            GameObject_TraceMove(p, moved, &point, &region, 0, 0);
        } else {
            point = moved;
            region = NULL;
        }
        D_800F1ADC = &D_800F1CE0;
    }
    CParticleSystem__Spawn(D_800F8928, p, p->m_pOwner, p->m_pOwnerCount, p->m_pTarget, model, direction, rotation, point, region,
                  arg2, flags | (p->m_dwFlags & 0x200006));
}
