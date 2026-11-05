#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

struct CollisionInfo;
extern struct CollisionInfo D_800F1B40;
extern CCollisionResults D_800F1CE0;  /* the last collision test (m_pInstance: what it hit) */
f32 func_002671B4(CGameObjectInstance *obj); /* the object's height */
s32 func_00239F00(void *obj, CVector3 to, struct CollisionInfo *info);

/* Whether the line from the top of `from` to the top of `target` is blocked,
 * by something other than `target` itself. */
s32 func_00245FC8(CGameObjectInstance *target, s32 unused, CGameObjectInstance *from) {
    CObjectHead saved;
    CVector3 to;
    s32 hit;

    saved = *(CObjectHead *) from;
    from->header.m_vPos.y += func_002671B4(from);
    from->header.m_vPos.y += from->m_CollisionYOffset;
    to = target->header.m_vPos;
    to.y += func_002671B4(target);
    to.y += target->m_CollisionYOffset;
    hit = func_00239F00(from, to, &D_800F1B40);
    if (D_800F1CE0.m_pInstance == target) {
        hit = 0;
    }
    *(CObjectHead *) from = saved;
    return hit;
}
