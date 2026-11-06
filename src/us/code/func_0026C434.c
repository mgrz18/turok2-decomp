/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, cross_edges, with Turok 2's layouts. */
#include "common.h"
#include "structs/region.h"
#include "globals/region.h"

extern s32 CGameRegion__IsSteep(CGameRegion *tri);                      /* is the triangle a slope? */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */
extern f32 func_00298470(f32);                                /* sqrtf */
extern f32 Math_AcosLookup(f32);                                /* acosf */

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ CVector3 raw_normal(CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    if (tri == NULL) {
        sRegionNormal.x = 0;
        sRegionNormal.z = 0;
        sRegionNormal.y = 1.0f;
    } else if (tri != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge0, &edge1);
    }
    sCachedRawNormalRegion = tri;
    return sRegionNormal;
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
