#include "common.h"
#include "structs/region.h"
#include "globals/region.h"

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);

#ifdef TARGET_PORT
/* The port (ABI): every caller passes the result's address first (the MIPS hidden result
 * pointer of a struct return; CGameRegion__GetCeilingNormalRaw is written that way); x86-64
 * returns a 12-byte struct in registers, so the callers' region arrived as the result pointer. */
CVector3 *CGameRegion__GetNormalRaw(CVector3 *out, CGameRegion *polygon) {
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
    *out = sRegionNormal;
    return out;
}
#else
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
#endif
