#include "common.h"
#include "structs/region.h"

extern CGameRegion *sCachedRawNormalRegion;    /* triangle whose normal is cached */
extern CVector3 sRegionNormal;         /* its normal */

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);    /* cross product */
void CVector3__Normalize(CVector3 *v);                        /* normalize */

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

/* The velocity something slides with down the slope of tri. */
CVector3 CGameRegion__GetSlideVelocity(CGameRegion *tri, f32 speed) {
    CVector3 normal;
    CVector3 up;
    CVector3 side;
    CVector3 downhill;
    CVector3 down;
    CVector3 result;

    if (tri == NULL) {
        result.x = result.y = result.z = 0.0f;
        return result;
    } else {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        normal = triNormal(tri);
        CVector3__Cross(&side, &normal, &up);
        CVector3__Cross(&downhill, &normal, &side);
        CVector3__Normalize(&downhill);
        if (downhill.y > 0.0f) {
            CVector3__Scale(&downhill, &downhill, -1.0f);
        }
        down.x = 0.0f;    /* set but never used */
        down.z = 0.0f;
        down.y = -1.0f;
        CVector3__Scale(&result, &downhill, -downhill.y * -speed * 10.24f);
    }
    return result;
}
