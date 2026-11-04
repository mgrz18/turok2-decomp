#include "common.h"
#include "structs/region.h"

void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);    /* a + b */
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);    /* a - b */
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
f32 func_00298470(f32 x);                           /* sqrtf */

/* Centre of tri, as the mean of its edges' midpoints. */
static __inline__ void triCentre(CVector3 *out, CGameRegion *tri) {
    CVector3 mid0;
    CVector3 mid1;
    CVector3 mid2;

    func_0020EEF8(&mid0, tri->m_pVertices[0], tri->m_pVertices[1]);
    func_0020EF60(&mid0, &mid0, 0.5f);
    func_0020EEF8(&mid1, tri->m_pVertices[1], tri->m_pVertices[2]);
    func_0020EF60(&mid1, &mid1, 0.5f);
    func_0020EEF8(&mid2, tri->m_pVertices[2], tri->m_pVertices[0]);
    func_0020EF60(&mid2, &mid2, 0.5f);
    func_0020EEF8(out, &mid0, &mid1);
    func_0020EEF8(out, out, &mid2);
    func_0020EF60(out, out, 1.0f / 3);
}

/* Distance between the centres of two triangles. */
f32 func_0026B858(CGameRegion *a, CGameRegion *b) {
    CVector3 diff;
    CVector3 centreA;
    CVector3 centreB;

    triCentre(&centreA, a);
    triCentre(&centreB, b);
    func_0020EF2C(&diff, &centreA, &centreB);
    return func_00298470(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}
