#include "common.h"
#include "structs/region.h"
#include "globals/region.h"


void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);    /* cross product */
f32 func_00298470(f32 x);                           /* sqrtf */

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ CVector3 triNormal(CGameRegion *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge1, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge2, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge1, &edge2);
    }
    sCachedRawNormalRegion = tri;
    return sRegionNormal;
}

/* Height of tri's plane above (x, z). */
static __inline__ f32 triHeight(CGameRegion *tri, f32 x, f32 z) {
    CVector3 normal;
    CVector3 v;

    if (tri == NULL) {
        return -14336.0f;
    }
    normal = triNormal(tri);
    if (normal.y == 0.0f) {
        return (tri->m_pVertices[0]->y + tri->m_pVertices[1]->y + tri->m_pVertices[2]->y) * (1.0f / 3);
    }
    v = *tri->m_pVertices[0];
    return ((v.z - z) * normal.z + (v.x - x) * normal.x + v.y * normal.y) / normal.y;
}

/* Cosine of the slope climbed going from (x1, z1) to (x2, z2) on tri; 1 when not climbing. */
f32 CGameRegion__GetClimbCos(CGameRegion *tri, f32 x1, f32 z1, f32 x2, f32 z2) {
    f32 h1;
    f32 h2;
    f32 flat;
    f32 total;

    if (tri != NULL) {
        h1 = triHeight(tri, x1, z1);
        h2 = triHeight(tri, x2, z2);
        if (h1 < h2) {
            flat = (x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1);
            total = flat + (h2 - h1) * (h2 - h1);
            if (total != 0.0f) {
                return func_00298470(flat / total);
            }
        }
    }
    return 1.0f;
}
