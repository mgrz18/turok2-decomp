#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/collision.h"



s32 Collision_MoveInstance(CObjectHead *obj, CVector3 to, CCollisionInfo *info);

/* Whether obj can move to `to` without hitting anything; obj is left where it was. */
s32 GameObject_CanMoveTo(CObjectHead *obj, CVector3 to, CCollisionInfo *info) {
    CObjectHead saved;
    CCollisionInfo probe;
    s32 hit;

    saved = *obj;
    probe = *info;
    probe.m_WallBehavior = 1;
    probe.m_InstanceBehavior = 0;
    Collision_MoveInstance(obj, to, &probe);
    *obj = saved;
    hit = 0;
    if (gCollisionResults.m_pInstance != NULL || gCollisionResults.m_pWallRegion != NULL) {
        hit = 1;
    }
    return hit ^ 1;
}
