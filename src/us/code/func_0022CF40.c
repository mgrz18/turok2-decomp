/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_4.CPP, Particle_ParentTransform, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"

/* The transform a particle's position is relative to, picked by its
 * attachment mode: the particle's own facing, the owner's effect transforms,
 * the region's ground or wall orientation, or a fixed one. */

extern void func_00210090(CMtxF m, CVector3 *dir);             /* matrix facing dir */
extern f32 func_0026BE60(void *region, f32 x, f32 z);        /* ground height */
extern void func_0026A9C0(CQuaternion *out, void *region);         /* ground orientation */
extern void func_0026AC38(CQuaternion *out, void *region);         /* wall orientation */
extern void func_00211114(CQuaternion *q, CMtxF m);                /* to matrix */
extern void func_0020C618(CMtxF out, CMtxF a, CMtxF b);      /* out = a * b */
extern f32 func_0026C240(void *region, s32 attach);
extern void func_002105A4(CMtxF m, f32 angle);
extern CMtxF D_800F1810;
extern CMtxF D_800F18F0;

CMtxF *func_0022CF40(CParticle *p, u8 *view) {
    CMtxF output;
    CQuaternion state;

    switch (p->m_Alignment) {
    case -8:
        func_00210090(D_800F1810, &p->m_vDirection);
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
        if (p->header.m_vPos.y - func_0026BE60(p->header.m_pCurrentRegion, p->header.m_vPos.x, p->header.m_vPos.z) <= 0.01f) {
            func_0026A9C0(&state, p->header.m_pCurrentRegion);
            func_00211114(&state, output);
            func_0020C618(D_800F1810, D_800F18F0, output);
            return &D_800F1810;
        }
    case -5:
        return &D_800F18F0;
    case -2:
        func_0026AC38(&state, p->header.m_pCurrentRegion);
        func_00211114(&state, output);
        func_0020C618(D_800F1810, D_800F18F0, output);
        return &D_800F1810;
    case 0:
    case 1:
    case 2:
        func_002105A4(D_800F1810, func_0026C240(p->header.m_pCurrentRegion, p->m_Alignment));
        return &D_800F1810;
    }
    return NULL;
}
