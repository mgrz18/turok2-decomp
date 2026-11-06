/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, cross_edges, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"


/* A collision vertex with a floor and a ceiling height. */
typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 floor;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 ceiling;
} CeilingVertex;

/* A region's vertex seen with its ceiling height. */
#define CEILING_VERTEX(tri, i) ((CeilingVertex *)(tri)->m_pVertices[i])

extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */

extern CGameRegion *sCachedCeilingRawNormalRegion;    /* triangle of the cached ceiling normal */
extern CVector3 sRegionCeilingNormal;        /* cached raw ceiling normal */

/* Raw normal of a triangle's ceiling plane (straight down for none), cached for the last triangle. */
CVector3 *CGameRegion__GetCeilingNormalRaw(CVector3 *out, CGameRegion *tri) {
    CVector3 a;
    CVector3 b;

    if (tri == NULL) {
        sRegionCeilingNormal.x = 0;
        sRegionCeilingNormal.z = 0;
        sRegionCeilingNormal.y = -1.0f;
    } else if (tri != sCachedCeilingRawNormalRegion) {
        a.x = CEILING_VERTEX(tri, 1)->x - CEILING_VERTEX(tri, 0)->x;
        a.y = CEILING_VERTEX(tri, 1)->ceiling - CEILING_VERTEX(tri, 0)->ceiling;
        a.z = CEILING_VERTEX(tri, 1)->z - CEILING_VERTEX(tri, 0)->z;
        b.x = CEILING_VERTEX(tri, 2)->x - CEILING_VERTEX(tri, 1)->x;
        b.y = CEILING_VERTEX(tri, 2)->ceiling - CEILING_VERTEX(tri, 1)->ceiling;
        b.z = CEILING_VERTEX(tri, 2)->z - CEILING_VERTEX(tri, 1)->z;
        CVector3__Cross(&sRegionCeilingNormal, &b, &a);
    }
    *out = sRegionCeilingNormal;
    sCachedCeilingRawNormalRegion = tri;
    return out;
}
