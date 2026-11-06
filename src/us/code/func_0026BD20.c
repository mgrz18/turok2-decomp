/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, func_802750B0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/region.h"
#include "globals/region.h"

extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */
extern void CVector3__Normalize(CVector3 *v);                         /* normalise */

extern CVector3 sRegionNormal;        /* cached raw normal */
extern CVector3 sRegionUnitNormal;        /* cached unit normal */

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ CVector3 *raw_normal(CVector3 *out, CGameRegion *tri) {
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
    *out = sRegionNormal;
    return out;
}

/* Unit normal of a triangle (straight up for none), cached for the last triangle. */
CVector3 *CGameRegion__GetNormal(CVector3 *out, CGameRegion *tri) {
    if (tri == NULL) {
        sRegionUnitNormal.x = 0;
        sRegionUnitNormal.z = 0;
        sRegionUnitNormal.y = 1.0f;
    } else if (tri != sCachedNormalRegion) {
        raw_normal(&sRegionUnitNormal, tri);
        CVector3__Normalize(&sRegionUnitNormal);
    }
    *out = sRegionUnitNormal;
    sCachedNormalRegion = tri;
    return out;
}
