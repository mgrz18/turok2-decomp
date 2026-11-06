#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C_3.CPP, func_802757E4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/region.h"
#include "globals/region.h"

extern void CVector3__Normalize(f32 *arg0);
extern CVector3 sRegionNormal;
extern f32 sRegionNormal_y;
extern s32 sRegionNormal_z;
extern CVector3 sRegionUnitNormal;
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
static __inline__ void edge(CVector3 *out, CVector3 *a, CVector3 *b) {
    CVector3__Subtract(out, a, b);
}
static __inline__ void normalize_copy(CVector3 *out, CVector3 *in) {
    *out = *in;
    CVector3__Normalize(out);
}
s32 CGameRegion__IsSteep(CGameRegion *polygon) {
    CVector3 up;
    CVector3 normal;
    CVector3 edge0;
    CVector3 edge1;
    if (polygon == 0) {
        return 0;
    }
    if (!(polygon->m_wFlags & 0x2000)) {
        return polygon->m_wFlags & 8;
    }
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    if (polygon != sCachedNormalRegion) {
        if (polygon == 0) {
            *(s32 *)&sRegionNormal = 0;
            sRegionNormal_y = 1.0f;
            sRegionNormal_z = 0;
        } else if (polygon != sCachedRawNormalRegion) {
            edge(&edge0, polygon->m_pVertices[1], polygon->m_pVertices[0]);
            edge(&edge1, polygon->m_pVertices[2], polygon->m_pVertices[1]);
            CVector3__Cross(&sRegionNormal, &edge0, &edge1);
        }
        sCachedRawNormalRegion = polygon;
        normalize_copy(&sRegionUnitNormal, &sRegionNormal);
    }
    normal = sRegionUnitNormal;
    sCachedNormalRegion = polygon;
    if (normal.x * up.x + normal.y * up.y + normal.z * up.z <= 0.5f) {
        return 1;
    }
    return 0;
}
