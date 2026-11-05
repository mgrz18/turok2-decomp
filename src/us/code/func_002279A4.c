#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/collision.h"
#include "structs/level_actor.h"

extern CBox D_800F5A10;               /* visible bounds */

s32 func_00267090(CPooledLevelActor *actor);

/* Adds to the world's draw list the pool actors with flags 0x20000 (of 0x30000) that
 * func_00267090 accepts and that are inside the visible bounds. */
void func_002279A4(CLevelActorPool *pool, CWorld *world) {
    CPooledLevelActor *actor;

    for (actor = pool->m_pHead; actor != NULL; actor = actor->m_pNext) {
        if ((*(s32 *) &actor->m_Actor.m_wFlags & 0x30000) == 0x20000 && func_00267090(actor)
            && D_800F5A10.m_vMax.x > actor->m_Actor.m_Bounds.m_vMin.x && D_800F5A10.m_vMin.x < actor->m_Actor.m_Bounds.m_vMax.x
            && D_800F5A10.m_vMax.z > actor->m_Actor.m_Bounds.m_vMin.z && D_800F5A10.m_vMin.z < actor->m_Actor.m_Bounds.m_vMax.z
            && D_800F5A10.m_vMax.y > actor->m_Actor.m_Bounds.m_vMin.y && D_800F5A10.m_vMin.y < actor->m_Actor.m_Bounds.m_vMax.y) {
            if (world->m_nDrawActors != 0x200) {
                world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)actor;
            }
        }
    }
}
