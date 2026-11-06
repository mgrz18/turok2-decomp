#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

struct CollisionInfo;
extern struct CollisionInfo gCollisionInfoLineOfSight;
extern CCollisionResults gCollisionResults;  /* the last collision test (m_pInstance: what it hit) */
f32 GameObject_GetCollisionHeight(CGameObjectInstance *obj); /* the object's height */
s32 Collision_MoveInstance(void *obj, CVector3 to, struct CollisionInfo *info);

/* Whether the line from the top of `from` to the top of `target` is blocked,
 * by something other than `target` itself. */
s32 AI_IsLineOfSightBlocked(CGameObjectInstance *target, s32 unused, CGameObjectInstance *from) {
    CObjectHead saved;
    CVector3 to;
    s32 hit;

    saved = *(CObjectHead *) from;
    from->header.m_vPos.y += GameObject_GetCollisionHeight(from);
    from->header.m_vPos.y += from->m_CollisionYOffset;
    to = target->header.m_vPos;
    to.y += GameObject_GetCollisionHeight(target);
    to.y += target->m_CollisionYOffset;
    hit = Collision_MoveInstance(from, to, &gCollisionInfoLineOfSight);
    if (gCollisionResults.m_pInstance == target) {
        hit = 0;
    }
    *(CObjectHead *) from = saved;
    return hit;
}
