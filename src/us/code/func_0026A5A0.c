#include "common.h"
#include "structs/region.h"
#include "globals/region.h"

extern CVector3 sRegionNormal;         /* its normal */

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);    /* cross product */

/* tri's face normal, cached for the last triangle asked about; straight up without one. */
static __inline__ CVector3 triNormal(CGameRegion *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri == NULL) {
        sRegionNormal.x = 0.0f;
        sRegionNormal.z = 0.0f;
        sRegionNormal.y = 1.0f;
    } else if (tri != sCachedRawNormalRegion) {
        CVector3__Subtract(&edge1, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge2, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&sRegionNormal, &edge1, &edge2);
    }
    sCachedRawNormalRegion = tri;
    return sRegionNormal;
}

/*
 * Where the segment from -> to crosses tri's plane, going against its normal:
 * *t gets the fraction along the segment (1 when moving away from the plane);
 * returns whether the crossing lies strictly between the ends.
 */
s32 CGameRegion__IntersectSegment(CGameRegion *tri, CVector3 *from, CVector3 *to, f32 *t) {
    CVector3 normal;
    CVector3 dir;
    CVector3 v;
    CVector3 diff;
    f32 denom;
    f32 frac;

    normal = triNormal(tri);
    CVector3__Subtract(&dir, to, from);
    if (tri != NULL) {
        v = *tri->m_pVertices[0];
    } else {
        v.x = v.y = v.z = 0.0f;
    }
    denom = normal.x * dir.x + normal.y * dir.y + normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    CVector3__Subtract(&diff, from, &v);
    frac = -(normal.x * diff.x + normal.y * diff.y + normal.z * diff.z) / denom;
    *t = frac;
    return frac > 0.0f && frac < 1.0f;
}
