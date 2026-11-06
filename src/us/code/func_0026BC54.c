#include "common.h"
#include "structs/region.h"

extern CGameRegion *D_800B2608; /* triangle whose normal is cached */
extern CVector3 D_800F2020;  /* cached (unnormalized) normal */

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);

/* Unnormalized normal of a triangle (up when there is none), cached. */
CVector3 CGameRegion__GetNormalRaw(CGameRegion *polygon) {
    CVector3 edge0;
    CVector3 edge1;

    if (polygon == NULL) {
        D_800F2020.x = 0.0f;
        D_800F2020.z = 0.0f;
        D_800F2020.y = 1.0f;
    } else if (polygon != D_800B2608) {
        CVector3__Subtract(&edge0, polygon->m_pVertices[1], polygon->m_pVertices[0]);
        CVector3__Subtract(&edge1, polygon->m_pVertices[2], polygon->m_pVertices[1]);
        CVector3__Cross(&D_800F2020, &edge0, &edge1);
    }
    D_800B2608 = polygon;
    return D_800F2020;
}
