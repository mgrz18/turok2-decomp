#include "common.h"
#include "structs/world.h"

typedef struct ActorDef {
    /* 0x00 */ s32 kind;
} ActorDef;

typedef struct Actor {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ f32 x, y, z;
    /* 0x010 */ u8 pad10[0x4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0x140 - 0x18];
    /* 0x140 */ u8 ai[1];
} Actor;

s32 func_00264C80(Actor *actor, void *ai);

/* The actor of kind 5 in the second world list (0x1050) nearest to target (squared distance) for which
 * func_00264C80 returns 0, or NULL. */
Actor *func_002259D0(CWorld *world, Actor *target) {
    Actor **actors;
    Actor *actor;
    Actor *nearest;
    f32 best;
    f32 dx, dy, dz, dist;
    s32 count;
    s32 i;

    i = 0;
    nearest = NULL;
    count = world->m_nList7;
    best = 3.4028235e38f;
    actors = (Actor **)world->m_pList7;
    for (; i < count; i++) {
        actor = actors[i];
        if (actor->def->kind != 5) {
            continue;
        }
        if (func_00264C80(actor, actor->ai) != 0) {
            continue;
        }
        dx = target->x - actor->x;
        dy = target->y - actor->y;
        dz = target->z - actor->z;
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < best) {
            best = dist;
            nearest = actor;
        }
    }
    return nearest;
}
