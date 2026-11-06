/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, cross_edges, with Turok 2's layouts. */
#include "common.h"
#include "structs/region.h"

extern s32 CGameRegion__IsSteep(CGameRegion *tri);                      /* is the triangle a slope? */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */
extern f32 func_00298470(f32);                                /* sqrtf */
extern f32 Math_AcosLookup(f32);                                /* acosf */

extern CGameRegion *D_800B2608;    /* triangle of the cached raw normal */
extern CVector3 D_800F2020;        /* cached raw normal */

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ CVector3 raw_normal(CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    if (tri == NULL) {
        D_800F2020.x = 0;
        D_800F2020.z = 0;
        D_800F2020.y = 1.0f;
    } else if (tri != D_800B2608) {
        CVector3__Subtract(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&D_800F2020, &edge0, &edge1);
    }
    D_800B2608 = tri;
    return D_800F2020;
}

/* Downhill yaw of a sloped triangle; 0 when it is not a slope or is level. */
f32 CGameRegion__GetDownhillYaw(CGameRegion *tri) {
    CVector3 normal;
    f32 mag;
    f32 result;

    if (!CGameRegion__IsSteep(tri)) {
        return 0.0f;
    }
    normal = raw_normal(tri);
    mag = func_00298470(normal.x * normal.x + normal.z * normal.z);
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = Math_AcosLookup(normal.z / mag);
    if (!(normal.x > 0.0f)) {
        result = -result;
    }
    return result;
}
