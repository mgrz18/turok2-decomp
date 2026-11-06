/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_4.CPP, Particle_ParentTransform, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"

/* The transform a particle's position is relative to, picked by its
 * attachment mode: the particle's own facing, the owner's effect transforms,
 * the region's ground or wall orientation, or a fixed one. */

extern void CMtxF__SetLookDirection(CMtxF m, CVector3 *dir);             /* matrix facing dir */
extern f32 CGameRegion__GetHeight(void *region, f32 x, f32 z);        /* ground height */
extern void CGameRegion__GetTiltRotation(CQuaternion *out, void *region);         /* ground orientation */
extern void CGameRegion__GetCeilingTiltRotation(CQuaternion *out, void *region);         /* wall orientation */
extern void CQuaternion__ToMatrix(CQuaternion *q, CMtxF m);                /* to matrix */
extern void CMtxF__MultiplyAffine(CMtxF out, CMtxF a, CMtxF b);      /* out = a * b */
extern f32 CGameRegion__GetEdgeYaw(void *region, s32 attach);
extern void CMtxF__SetRotateY(CMtxF m, f32 angle);
extern CMtxF D_800F1810;
extern CMtxF D_800F18F0;

CMtxF *CParticle__GetParentTransform(CParticle *p, u8 *view) {
    CMtxF output;
    CQuaternion state;

    switch (p->m_Alignment) {
    case -8:
        CMtxF__SetLookDirection(D_800F1810, &p->m_vDirection);
        return &D_800F1810;
    case -6:
        if (view == NULL) {
            return NULL;
        }
        return (CMtxF *)(view + 0xE7C);
    case -4:
    case -1:
        if (view == NULL) {
            return NULL;
        }
        return (CMtxF *)(view + 0xE3C);
    case -3:
        if (p->header.m_vPos.y - CGameRegion__GetHeight(p->header.m_pCurrentRegion, p->header.m_vPos.x, p->header.m_vPos.z) <= 0.01f) {
            CGameRegion__GetTiltRotation(&state, p->header.m_pCurrentRegion);
            CQuaternion__ToMatrix(&state, output);
            CMtxF__MultiplyAffine(D_800F1810, D_800F18F0, output);
            return &D_800F1810;
        }
    case -5:
        return &D_800F18F0;
    case -2:
        CGameRegion__GetCeilingTiltRotation(&state, p->header.m_pCurrentRegion);
        CQuaternion__ToMatrix(&state, output);
        CMtxF__MultiplyAffine(D_800F1810, D_800F18F0, output);
        return &D_800F1810;
    case 0:
    case 1:
    case 2:
        CMtxF__SetRotateY(D_800F1810, CGameRegion__GetEdgeYaw(p->header.m_pCurrentRegion, p->m_Alignment));
        return &D_800F1810;
    }
    return NULL;
}
