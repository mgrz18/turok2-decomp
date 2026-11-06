#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/world.h"


s32 Throwable_GetModeAnim(CGameObjectInstance *actor, void *ai);

/* The actor of kind 5 in the second world list (0x1050) nearest to target (squared distance) for which
 * Throwable_GetModeAnim returns 0, or NULL. */
CGameObjectInstance *func_002259D0(CWorld *world, CGameObjectInstance *target) {
    CGameObjectInstance **actors;
    CGameObjectInstance *actor;
    CGameObjectInstance *nearest;
    f32 best;
    f32 dx, dy, dz, dist;
    s32 count;
    s32 i;

    i = 0;
    nearest = NULL;
    count = world->m_nList7;
    best = 3.4028235e38f;
    actors = (CGameObjectInstance **)world->m_pList7;
    for (; i < count; i++) {
        actor = actors[i];
        if (actor->header.m_pIntelligence->m_Type != 5) {
            continue;
        }
        if (Throwable_GetModeAnim(actor, ((u8 *)&actor->m_AI)) != 0) {
            continue;
        }
        dx = target->header.m_vPos.x - actor->header.m_vPos.x;
        dy = target->header.m_vPos.y - actor->header.m_vPos.y;
        dz = target->header.m_vPos.z - actor->header.m_vPos.z;
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < best) {
            best = dist;
            nearest = actor;
        }
    }
    return nearest;
}
