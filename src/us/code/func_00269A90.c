#include "structs/vector.h"
#include "globals/region.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C.CPP, func_802750B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct Node75;
typedef struct Node75 Node75;
struct Node75;
struct Node75 {
    u16 field0;
    u16 flags;
    CVector3 *prev;
    CVector3 *cur;
    CVector3 *next;
    u8 reserved10[0x10];
};
typedef struct CollisionBuildVertex {
    f32 x;
    f32 y;
    f32 z;
    f32 ceiling;
} CollisionBuildVertex;
extern int sRegionCeilingNormal;
extern int sRegionCeilingUnitNormal;
extern void CVector3__Normalize(f32 *arg0);
struct Node75;
extern int sRegionCeilingNormal;
extern int sRegionCeilingUnitNormal;
extern void CVector3__Cross(CVector3 *, CVector3 *, CVector3 *);
static __inline__ CVector3 *cross_edges(CVector3 *out, Node75 *node) {
    CVector3 a;
    CVector3 b;
    char *p;
    char *q;
    if (node == 0) {
        ((CVector3 *)&sRegionCeilingNormal)->x = 0;
        ((CVector3 *)&sRegionCeilingNormal)->z = 0;
        ((CVector3 *)&sRegionCeilingNormal)->y = -1.0f;
    } else if ((struct CGameRegion *)node != sCachedCeilingRawNormalRegion) {
        p = node->cur;
        q = node->prev;
        a.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = node->cur;
        q = node->prev;
        a.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = node->cur;
        q = node->prev;
        a.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        p = node->next;
        q = node->cur;
        b.x = ((CollisionBuildVertex *)p)->x - ((CollisionBuildVertex *)q)->x;
        p = node->next;
        q = node->cur;
        b.y = ((CollisionBuildVertex *)p)->ceiling - ((CollisionBuildVertex *)q)->ceiling;
        p = node->next;
        q = node->cur;
        b.z = ((CollisionBuildVertex *)p)->z - ((CollisionBuildVertex *)q)->z;
        CVector3__Cross((CVector3 *)&sRegionCeilingNormal, &b, &a);
    }
    sCachedCeilingRawNormalRegion = (struct CGameRegion *)node;
    *out = *(CVector3 *)&sRegionCeilingNormal;
    return out;
}
CVector3 *CGameRegion__GetCeilingNormal(CVector3 *out, Node75 *node) {
    if (node == 0) {
        ((CVector3 *)&sRegionCeilingUnitNormal)->x = 0;
        ((CVector3 *)&sRegionCeilingUnitNormal)->z = 0;
        ((CVector3 *)&sRegionCeilingUnitNormal)->y = -1.0f;
    } else if ((struct CGameRegion *)node != sCachedCeilingNormalRegion) {
        cross_edges((CVector3 *)&sRegionCeilingUnitNormal, node);
        CVector3__Normalize((CVector3 *)&sRegionCeilingUnitNormal);
    }
    *out = *(CVector3 *)&sRegionCeilingUnitNormal;
    sCachedCeilingNormalRegion = (struct CGameRegion *)node;
    return out;
}
