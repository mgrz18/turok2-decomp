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
    /* 0x164 */ u8 pad164[0x198 - 0x164];
    /* 0x198 */ f32 lifetime;
    /* 0x19C */ f32 timer;
    /* 0x1A0 */ s32 poolFlags;          /* 1: in use */
    /* 0x1A4 */ s32 *refCount;
    /* 0x1A8 */ struct PoolActor *prev;
    /* 0x1AC */ struct PoolActor *next;
} PoolActor;                            /* 0x1B0 bytes */

typedef struct ActorPool {
    /* 0x0000 */ PoolActor items[32];
    /* 0x3600 */ PoolActor *free;
    /* 0x3604 */ PoolActor *head;       /* newest */
    /* 0x3608 */ PoolActor *tail;       /* oldest */
} ActorPool;

extern f32 D_800B6D28;                  /* the frame's time step */
extern Bounds D_800F5A10;               /* visible bounds */

void func_00227680(PoolActor *actor);

/* Releases an actor in use: uncounts it, unlinks it and puts it on the free list. */
static __inline__ void releaseActor(ActorPool *pool, PoolActor *actor) {
    if (actor->poolFlags & 1) {
        actor->poolFlags &= ~1;
        if (actor->refCount != NULL) {
            (*actor->refCount)--;
        }
        if (actor->prev != NULL) {
            actor->prev->next = actor->next;
        }
        if (actor->next != NULL) {
            actor->next->prev = actor->prev;
        }
        if (pool->head == actor) {
            pool->head = actor->next;
        }
        if (pool->tail == actor) {
            pool->tail = actor->prev;
        }
        actor->next = pool->free;
        actor->prev = NULL;
        pool->free = actor;
    }
}

/* Updates the pool actors (func_00227680); releases those whose lifetime ran out this frame
 * or that left the visible bounds. */
void func_002274A4(ActorPool *pool) {
    PoolActor *actor;
    PoolActor *next;
    s32 expired;

    next = pool->head;
    while (next != NULL) {
        actor = next;
        next = next->next;
        expired = 0;
        if (actor->lifetime > 0.0f) {
            actor->lifetime -= D_800B6D28;
            if (actor->lifetime <= 0.0f) {
                expired = 1;
            }
        }
        if (!(D_800F5A10.max.x > actor->bounds.min.x && D_800F5A10.min.x < actor->bounds.max.x
              && D_800F5A10.max.z > actor->bounds.min.z && D_800F5A10.min.z < actor->bounds.max.z
              && D_800F5A10.max.y > actor->bounds.min.y && D_800F5A10.min.y < actor->bounds.max.y)) {
            expired = 1;
        }
        if (expired) {
            releaseActor(pool, actor);
        } else {
            func_00227680(actor);
        }
    }
}
