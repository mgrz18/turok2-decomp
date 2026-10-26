#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Bounds {
    Vec3 min;                   /* 0x00 */
    Vec3 max;                   /* 0x0C */
} Bounds;

/* A level actor spawned at run time (a pickup), kept in the world's pool. */
typedef struct PoolActor {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ Bounds bounds;
    /* 0x164 */ u8 pad164[0x16C - 0x164];
    /* 0x16C */ s32 flags;              /* the u16 at 0x16C in its top half */
    /* 0x170 */ u8 pad170[0x1AC - 0x170];
    /* 0x1AC */ struct PoolActor *next;
} PoolActor;                            /* 0x1B0 bytes */

typedef struct ActorPool {
    /* 0x0000 */ PoolActor items[32];
    /* 0x3600 */ PoolActor *free;
    /* 0x3604 */ PoolActor *head;
    /* 0x3608 */ PoolActor *tail;
} ActorPool;

typedef struct World {
    /* 0x000 */ u8 pad0[0xEC];
    /* 0x0EC */ void *draw[0x200];
    /* 0x8EC */ s32 drawCount;
} World;

extern Bounds D_800F5A10;               /* visible bounds */

s32 func_00267090(PoolActor *actor);

/* Adds to the world's draw list the pool actors with flags 0x20000 (of 0x30000) that
 * func_00267090 accepts and that are inside the visible bounds. */
void func_002279A4(ActorPool *pool, World *world) {
    PoolActor *actor;

    for (actor = pool->head; actor != NULL; actor = actor->next) {
        if ((actor->flags & 0x30000) == 0x20000 && func_00267090(actor)
            && D_800F5A10.max.x > actor->bounds.min.x && D_800F5A10.min.x < actor->bounds.max.x
            && D_800F5A10.max.z > actor->bounds.min.z && D_800F5A10.min.z < actor->bounds.max.z
            && D_800F5A10.max.y > actor->bounds.min.y && D_800F5A10.min.y < actor->bounds.max.y) {
            if (world->drawCount != 0x200) {
                world->draw[world->drawCount++] = actor;
            }
        }
    }
}
