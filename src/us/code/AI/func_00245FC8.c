#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "globals/collision.h"

struct CollisionInfo;

/* Whether the line from the top of `from` to the top of `target` is blocked,
 * by something other than `target` itself. */
s32 AI_IsLineOfSightBlocked(CGameObjectInstance *target, sptr unused, CGameObjectInstance *from) {
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
