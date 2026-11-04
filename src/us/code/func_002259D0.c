#include "common.h"
#include "structs/game_object.h"

typedef struct ActorDef {
    /* 0x00 */ s32 kind;
} ActorDef;


typedef struct World {
    /* 0x000 */ u8 pad0[0x1050];
    /* 0x1050 */ CGameObjectInstance *actors[4];
    /* 0x1060 */ s32 actorCount;
} World;

s32 func_00264C80(CGameObjectInstance *actor, void *ai);

/* The actor of kind 5 in the second world list (0x1050) nearest to target (squared distance) for which
 * func_00264C80 returns 0, or NULL. */
CGameObjectInstance *func_002259D0(World *world, CGameObjectInstance *target) {
    CGameObjectInstance **actors;
    CGameObjectInstance *actor;
    CGameObjectInstance *nearest;
    f32 best;
    f32 dx, dy, dz, dist;
    s32 count;
    s32 i;

    i = 0;
    nearest = NULL;
    count = world->actorCount;
    best = 3.4028235e38f;
    actors = world->actors;
    for (; i < count; i++) {
        actor = actors[i];
        if (((ActorDef *)actor->header.m_pIntelligence)->kind != 5) {
            continue;
        }
        if (func_00264C80(actor, ((u8 *)&actor->m_AI)) != 0) {
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
