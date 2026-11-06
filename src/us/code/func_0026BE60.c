/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, func_8027525C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/region.h"
#include "globals/region.h"

extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */

extern CVector3 sRegionNormal;        /* cached raw normal */

static __inline__ void cross_edges(CVector3 *out, CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    CVector3__Subtract(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
    CVector3__Subtract(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
    CVector3__Cross(out, &edge0, &edge1);
}

/* Height of a triangle's plane at (x, z); the mean vertex height for a vertical one. */
f32 CGameRegion__GetHeight(CGameRegion *tri, f32 x, f32 z) {
    CVector3 normal;
    CVector3 point;

    if (tri == NULL) {
        return -14336.0f;
    }
    if (tri != sCachedRawNormalRegion) {
        cross_edges(&sRegionNormal, tri);
    }
    normal = sRegionNormal;
    sCachedRawNormalRegion = tri;
    if (normal.y == 0.0f) {
        return (tri->m_pVertices[0]->y + tri->m_pVertices[1]->y + tri->m_pVertices[2]->y) * (1.0f / 3);
    }
    point = *tri->m_pVertices[0];
    return ((point.z - z) * normal.z + (point.x - x) * normal.x + point.y * normal.y) / normal.y;
}
