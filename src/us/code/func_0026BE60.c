/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, func_8027525C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/region.h"

extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */

extern CGameRegion *D_800B2608;    /* triangle of the cached raw normal */
extern CVector3 D_800F2020;        /* cached raw normal */

static __inline__ void cross_edges(CVector3 *out, CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    func_0020EF2C(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
    func_0020EF2C(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
    func_0020EFDC(out, &edge0, &edge1);
}

/* Height of a triangle's plane at (x, z); the mean vertex height for a vertical one. */
f32 func_0026BE60(CGameRegion *tri, f32 x, f32 z) {
    CVector3 normal;
    CVector3 point;

    if (tri == NULL) {
        return -14336.0f;
    }
    if (tri != D_800B2608) {
        cross_edges(&D_800F2020, tri);
    }
    normal = D_800F2020;
    D_800B2608 = tri;
    if (normal.y == 0.0f) {
        return (tri->m_pVertices[0]->y + tri->m_pVertices[1]->y + tri->m_pVertices[2]->y) * (1.0f / 3);
    }
    point = *tri->m_pVertices[0];
    return ((point.z - z) * normal.z + (point.x - x) * normal.x + point.y * normal.y) / normal.y;
}
