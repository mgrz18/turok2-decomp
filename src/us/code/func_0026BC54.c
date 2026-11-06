#include "common.h"
#include "structs/region.h"
#include "globals/region.h"

extern CVector3 sRegionNormal;  /* cached (unnormalized) normal */

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);

/* Unnormalized normal of a triangle (up when there is none), cached. */
CVector3 CGameRegion__GetNormalRaw(CGameRegion *polygon) {
    CVector3 edge0;
    CVector3 edge1;

    if (polygon == NULL) {
        sRegionNormal.x = 0.0f;
        sRegionNormal.z = 0.0f;
        sRegionNormal.y = 1.0f;
    } else if (polygon != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge0, polygon->m_pVertices[1], polygon->m_pVertices[0]);
        CVector3__Subtract(&edge1, polygon->m_pVertices[2], polygon->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge0, &edge1);
    }
    sCachedRawNormalRegion = polygon;
    return sRegionNormal;
}
