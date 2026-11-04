#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/collision.h"

/* A level actor spawned at run time (a pickup), kept in the world's pool. */
typedef struct PoolActor {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ CBox bounds;
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

extern CBox D_800F5A10;               /* visible bounds */

s32 func_00267090(PoolActor *actor);

/* Adds to the world's draw list the pool actors with flags 0x20000 (of 0x30000) that
 * func_00267090 accepts and that are inside the visible bounds. */
void func_002279A4(ActorPool *pool, CWorld *world) {
    PoolActor *actor;

    for (actor = pool->head; actor != NULL; actor = actor->next) {
        if ((actor->flags & 0x30000) == 0x20000 && func_00267090(actor)
            && D_800F5A10.m_vMax.x > actor->bounds.m_vMin.x && D_800F5A10.m_vMin.x < actor->bounds.m_vMax.x
            && D_800F5A10.m_vMax.z > actor->bounds.m_vMin.z && D_800F5A10.m_vMin.z < actor->bounds.m_vMax.z
            && D_800F5A10.m_vMax.y > actor->bounds.m_vMin.y && D_800F5A10.m_vMin.y < actor->bounds.m_vMax.y) {
            if (world->m_nDrawActors != 0x200) {
                world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)actor;
            }
        }
    }
}
