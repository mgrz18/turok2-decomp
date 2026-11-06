#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

extern CCollisionInfo D_800F1AE0;
extern CCollisionResults D_800F1CE0;
f32 GameObject_GetCollisionHeight(CGameObjectInstance *obj); /* the object's height */
s32 Collision_MoveInstance(void *obj, CVector3 to, CCollisionInfo *info);

/* Whether the line between the two objects' eyes (0.8 of their height up)
 * hits a wall or floor region flagged 2. */
s32 AI_IsEyeLineBlocked(CGameObjectInstance *from, s32 unused, CGameObjectInstance *to) {
    CObjectHead saved;
    CVector3 target;

    saved = *(CObjectHead *) from;
    from->header.m_vPos.y += GameObject_GetCollisionHeight(from) * 0.8f;
    from->header.m_vPos.y += from->m_CollisionYOffset;
    target = to->header.m_vPos;
    target.y += GameObject_GetCollisionHeight(to) * 0.8f;
    target.y += to->m_CollisionYOffset;
    Collision_MoveInstance(from, target, &D_800F1AE0);
    do {
        *(CObjectHead *) from = saved;
    } while (0);
    if (D_800F1CE0.m_pWallRegion != NULL && (D_800F1CE0.m_pWallRegion->m_wFlags & 2)) {
        return 1;
    }
    if (D_800F1CE0.m_pFloorRegion != NULL && (D_800F1CE0.m_pFloorRegion->m_wFlags & 2)) {
        return 1;
    }
    return 0;
}
