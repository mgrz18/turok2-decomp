#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"


/* A vertex of this kind of triangle keeps its height after z. */
typedef struct Vertex {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 y;
} Vertex;

#define REGION_VERTEX(tri, i) ((Vertex *)(tri)->m_pVertices[i])

extern CGameRegion *D_800B2600;    /* triangle whose normal is cached */
extern CVector3 D_800F2000;         /* its normal */

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);    /* cross product */

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ CVector3 triNormal(CGameRegion *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri != D_800B2600) {
        edge1.x = REGION_VERTEX(tri, 1)->x - REGION_VERTEX(tri, 0)->x;
        edge1.y = REGION_VERTEX(tri, 1)->y - REGION_VERTEX(tri, 0)->y;
        edge1.z = REGION_VERTEX(tri, 1)->z - REGION_VERTEX(tri, 0)->z;
        edge2.x = REGION_VERTEX(tri, 2)->x - REGION_VERTEX(tri, 1)->x;
        edge2.y = REGION_VERTEX(tri, 2)->y - REGION_VERTEX(tri, 1)->y;
        edge2.z = REGION_VERTEX(tri, 2)->z - REGION_VERTEX(tri, 1)->z;
        func_0020EFDC(&D_800F2000, &edge2, &edge1);
    }
    D_800B2600 = tri;
    return D_800F2000;
}

/*
 * Where the segment from -> to crosses tri's plane, going against its normal:
 * *t gets the fraction along the segment (1 when moving away from the plane);
 * returns whether the crossing lies strictly between the ends.
 */
s32 func_0026A798(CGameRegion *tri, CVector3 *from, CVector3 *to, f32 *t) {
    CVector3 normal;
    CVector3 dir;
    CVector3 v;
    CVector3 diff;
    f32 denom;
    f32 frac;

    if (tri == NULL) {
        return 0;
    }
    normal = triNormal(tri);
    func_0020EF2C(&dir, to, from);
    v.x = REGION_VERTEX(tri, 0)->x;
    v.y = REGION_VERTEX(tri, 0)->y;
    v.z = REGION_VERTEX(tri, 0)->z;
    denom = normal.x * dir.x + normal.y * dir.y + normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    func_0020EF2C(&diff, from, &v);
    frac = -(normal.x * diff.x + normal.y * diff.y + normal.z * diff.z) / denom;
    *t = frac;
    return frac > 0.0f && frac < 1.0f;
}
