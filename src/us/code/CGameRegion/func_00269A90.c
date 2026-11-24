#include "structs/vector.h"
#include "globals/region.h"
#include "protos/mtxf.h"
#include "structs/collision.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C.CPP, func_802750B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern void CVector3__Cross(CVector3 *, CVector3 *, CVector3 *);
static __inline__ CVector3 *cross_edges(CVector3 *out, CGameRegion *node) {
    CVector3 a;
    CVector3 b;
    char *p;
    char *q;
    if (node == 0) {
        ((CVector3 *)&sRegionCeilingNormal)->x = 0;
        ((CVector3 *)&sRegionCeilingNormal)->z = 0;
        ((CVector3 *)&sRegionCeilingNormal)->y = -1.0f;
    } else if (node != sCachedCeilingRawNormalRegion) {
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = (char *)node->m_pVertices[1];
        q = (char *)node->m_pVertices[0];
        a.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = (char *)node->m_pVertices[2];
        q = (char *)node->m_pVertices[1];
        b.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        CVector3__Cross((CVector3 *)&sRegionCeilingNormal, &b, &a);
    }
    sCachedCeilingRawNormalRegion = node;
    *out = *(CVector3 *)&sRegionCeilingNormal;
    return out;
}
CVector3 *CGameRegion__GetCeilingNormal(CVector3 *out, CGameRegion *node) {
    if (node == 0) {
        ((CVector3 *)&sRegionCeilingUnitNormal)->x = 0;
        ((CVector3 *)&sRegionCeilingUnitNormal)->z = 0;
        ((CVector3 *)&sRegionCeilingUnitNormal)->y = -1.0f;
    } else if (node != sCachedCeilingNormalRegion) {
        cross_edges((CVector3 *)&sRegionCeilingUnitNormal, node);
        CVector3__Normalize((CVector3 *)&sRegionCeilingUnitNormal);
    }
    *out = *(CVector3 *)&sRegionCeilingUnitNormal;
    sCachedCeilingNormalRegion = node;
    return out;
}
