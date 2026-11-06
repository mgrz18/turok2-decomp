#include "common.h"
#include "structs/region.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

extern CGameRegion *D_800B2608;    /* triangle whose normal is cached */
extern CVector3 D_800F2020;         /* its normal */

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);    /* cross product */
f32 func_00298470(f32 x);                           /* sqrtf */
f32 Math_AcosLookup(f32 x);                           /* acosf */

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ CVector3 triNormal(CGameRegion *tri) {
    CVector3 edge1;
    CVector3 edge2;

    if (tri != D_800B2608) {
        CVector3__Subtract(&edge1, tri->m_pVertices[1], tri->m_pVertices[0]);
        CVector3__Subtract(&edge2, tri->m_pVertices[2], tri->m_pVertices[1]);
        CVector3__Cross(&D_800F2020, &edge1, &edge2);
    }
    D_800B2608 = tri;
    return D_800F2020;
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

/* Slope angle climbed going from (x1, z1) to (x2, z2) on tri; negative going down. */
f32 CGameRegion__GetClimbAngle(CGameRegion *tri, f32 x1, f32 z1, f32 x2, f32 z2) {
    f32 h1;
    f32 h2;
    f32 flat;
    f32 total;
    f32 angle;
    f32 cosine;

    if (tri == NULL) {
        return 0.0f;
    }
    h1 = triHeight(tri, x1, z1);
    h2 = triHeight(tri, x2, z2);
    flat = (x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1);
    total = flat + (h2 - h1) * (h2 - h1);
    angle = 0.0f;
    if (total != 0.0f) {
        cosine = func_00298470(flat / total);
        angle = Math_AcosLookup(MAX(-1.0f, MIN(cosine, 1.0f)));
        if (!(h1 < h2)) {
            angle = -angle;
        }
    }
    return angle;
}
