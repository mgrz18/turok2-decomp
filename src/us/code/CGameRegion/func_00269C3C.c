/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_8027525C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/level_file.h"
#include "structs/region.h"
#include "protos/mtxf.h"
#include "globals/region.h"

extern void CVector3__Cross(CVector3 *, CVector3 *, CVector3 *);
f32 CGameRegion__GetCeilingHeight(CGameRegion *node, f32 x, f32 z) {
    CVector3 normal;
    CVector3 point;
    CVector3 a;
    CVector3 b;
    char *p;
    char *q;
    if (node == 0) {
        return 0.0f;
    }
    if (node != sCachedCeilingRawNormalRegion) {
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
    normal = *(CVector3 *)&sRegionCeilingNormal;
    sCachedCeilingRawNormalRegion = node;
    if (normal.y == 0.0f) {
        return (((CollisionBuildVertex *)node->m_pVertices[0])->ceiling +
                ((CollisionBuildVertex *)node->m_pVertices[1])->ceiling +
                ((CollisionBuildVertex *)node->m_pVertices[2])->ceiling) * 0.33333334f;
    }
    p = (char *)node->m_pVertices[0];
    point.x = ((CollisionBuildVertex *)p)->x;
    p = (char *)node->m_pVertices[0];
    point.y = ((CollisionBuildVertex *)p)->ceiling;
    p = (char *)node->m_pVertices[0];
    point.z = ((CollisionBuildVertex *)p)->z;
    return (((point.z - z) * normal.z) +
            ((point.x - x) * normal.x) + (point.y * normal.y)) /
           normal.y;
}
