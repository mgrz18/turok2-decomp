#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"

void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);

/* The centroid of the region's triangle, as the mean of its three edge midpoints. */
void CGameRegion__GetCentroid(CGameRegion *region, CVector3 *out) {
    CVector3 sp10;
    CVector3 sp20;
    CVector3 sp30;
    f32 half;

    CVector3__Add(&sp10, region->m_pVertices[0], region->m_pVertices[1]);
    half = 0.5f;
    CVector3__Scale(&sp10, &sp10, half);
    CVector3__Add(&sp20, region->m_pVertices[1], region->m_pVertices[2]);
    CVector3__Scale(&sp20, &sp20, half);
    CVector3__Add(&sp30, region->m_pVertices[2], region->m_pVertices[0]);
    CVector3__Scale(&sp30, &sp30, half);
    CVector3__Add(out, &sp10, &sp20);
    CVector3__Add(out, out, &sp30);
    CVector3__Scale(out, out, 0.33333334f);
}
