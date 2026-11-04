/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, func_802750B0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/region.h"

extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */
extern void func_0020F040(CVector3 *v);                         /* normalise */

extern CGameRegion *D_800B2608;    /* triangle of the cached raw normal */
extern CGameRegion *D_800B260C;    /* triangle of the cached unit normal */
extern CVector3 D_800F2020;        /* cached raw normal */
extern CVector3 D_800F2030;        /* cached unit normal */

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ CVector3 *raw_normal(CVector3 *out, CGameRegion *tri) {
    CVector3 edge0;
    CVector3 edge1;

    if (tri == NULL) {
        D_800F2020.x = 0;
        D_800F2020.z = 0;
        D_800F2020.y = 1.0f;
    } else if (tri != D_800B2608) {
        func_0020EF2C(&edge0, tri->m_pVertices[1], tri->m_pVertices[0]);
        func_0020EF2C(&edge1, tri->m_pVertices[2], tri->m_pVertices[1]);
        func_0020EFDC(&D_800F2020, &edge0, &edge1);
    }
    D_800B2608 = tri;
    *out = D_800F2020;
    return out;
}

/* Unit normal of a triangle (straight up for none), cached for the last triangle. */
CVector3 *func_0026BD20(CVector3 *out, CGameRegion *tri) {
    if (tri == NULL) {
        D_800F2030.x = 0;
        D_800F2030.z = 0;
        D_800F2030.y = 1.0f;
    } else if (tri != D_800B260C) {
        raw_normal(&D_800F2030, tri);
        func_0020F040(&D_800F2030);
    }
    *out = D_800F2030;
    D_800B260C = tri;
    return out;
}
