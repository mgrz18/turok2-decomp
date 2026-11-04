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

extern void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */

extern CGameRegion *D_800B2600;    /* triangle of the cached ceiling normal */
extern CVector3 D_800F2000;        /* cached raw ceiling normal */

/* Raw normal of a triangle's ceiling plane (straight down for none), cached for the last triangle. */
CVector3 *func_0026BFB4(CVector3 *out, CGameRegion *tri) {
    CVector3 a;
    CVector3 b;

    if (tri == NULL) {
        D_800F2000.x = 0;
        D_800F2000.z = 0;
        D_800F2000.y = -1.0f;
    } else if (tri != D_800B2600) {
        a.x = CEILING_VERTEX(tri, 1)->x - CEILING_VERTEX(tri, 0)->x;
        a.y = CEILING_VERTEX(tri, 1)->ceiling - CEILING_VERTEX(tri, 0)->ceiling;
        a.z = CEILING_VERTEX(tri, 1)->z - CEILING_VERTEX(tri, 0)->z;
        b.x = CEILING_VERTEX(tri, 2)->x - CEILING_VERTEX(tri, 1)->x;
        b.y = CEILING_VERTEX(tri, 2)->ceiling - CEILING_VERTEX(tri, 1)->ceiling;
        b.z = CEILING_VERTEX(tri, 2)->z - CEILING_VERTEX(tri, 1)->z;
        func_0020EFDC(&D_800F2000, &b, &a);
    }
    *out = D_800F2000;
    D_800B2600 = tri;
    return out;
}
