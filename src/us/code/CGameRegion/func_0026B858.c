#include "common.h"
#include "structs/region.h"

void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);    /* a + b */
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
f32 func_00298470(f32 x);                           /* sqrtf */

/* Centre of tri, as the mean of its edges' midpoints. */
static __inline__ void triCentre(CVector3 *out, CGameRegion *tri) {
    CVector3 mid0;
    CVector3 mid1;
    CVector3 mid2;

    CVector3__Add(&mid0, tri->m_pVertices[0], tri->m_pVertices[1]);
    CVector3__Scale(&mid0, &mid0, 0.5f);
    CVector3__Add(&mid1, tri->m_pVertices[1], tri->m_pVertices[2]);
    CVector3__Scale(&mid1, &mid1, 0.5f);
    CVector3__Add(&mid2, tri->m_pVertices[2], tri->m_pVertices[0]);
    CVector3__Scale(&mid2, &mid2, 0.5f);
    CVector3__Add(out, &mid0, &mid1);
    CVector3__Add(out, out, &mid2);
    CVector3__Scale(out, out, 1.0f / 3);
}

/* Distance between the centres of two triangles. */
f32 CGameRegion__GetCentreDistance(CGameRegion *a, CGameRegion *b) {
    CVector3 diff;
    CVector3 centreA;
    CVector3 centreB;

    triCentre(&centreA, a);
    triCentre(&centreB, b);
    CVector3__Subtract(&diff, &centreA, &centreB);
    return func_00298470(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}
