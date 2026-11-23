/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ANIM/80275E44.CPP, func_802760BC_de (inlined), with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"

extern f32 func_00298470(f32);                                  /* sqrtf */
extern f32 Math_AcosLookup(f32);                                  /* acosf */
extern void CMtxF__SetRotateY(f32 mtx[4][4], f32 angle);            /* rotation about Y */
extern void CMtxF__TransformPoint(f32 mtx[4][4], CVector3 *in, CVector3 *out); /* out = mtx * in */

/* Yaw of a triangle edge (CGameRegion__GetEdgeYaw). */
static __inline__ f32 edge_yaw(CGameRegion *tri, s32 edge) {
    CVector3 *v0;
    CVector3 *v1;
    f32 dx, dz;
    f32 mag;
    f32 result;

    v1 = tri->m_pVertices[edge];
    v0 = tri->m_pVertices[(edge + 1) % 3];
    dx = v0->x - v1->x;
    dz = v0->z - v1->z;
    mag = func_00298470(dx * dx + dz * dz);
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = Math_AcosLookup(dx / mag);
    if (!(dz < 0.0f)) {
        result = -result;
    }
    return result;
}

/* Horizontal unit vector facing along a triangle edge's yaw. */
CVector3 *CGameRegion__GetEdgeDirection(CVector3 *out, CGameRegion *tri, s32 edge) {
    CVector3 dir;
    CVector3 forward;
    f32 mtx[4][4];

    CMtxF__SetRotateY(mtx, edge_yaw(tri, edge));
    forward.x = 0;
    forward.y = 0;
    forward.z = 1.0f;
    CMtxF__TransformPoint(mtx, &forward, &dir);
    *out = dir;
    return out;
}
