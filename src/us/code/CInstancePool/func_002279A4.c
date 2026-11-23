#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/collision.h"
#include "structs/level_actor.h"
#include "globals/camera.h"


s32 GameObject_IsSolid(CPooledLevelActor *actor);

/* Adds to the world's draw list the pool actors with flags 0x20000 (of 0x30000) that
 * GameObject_IsSolid accepts and that are inside the visible bounds. */
void CLevelActorPool__CollectVisible(CLevelActorPool *pool, CScene *world) {
    CPooledLevelActor *actor;

    for (actor = pool->m_pHead; actor != NULL; actor = actor->m_pNext) {
        if ((*(s32 *) &actor->m_Actor.m_wFlags & 0x30000) == 0x20000 && GameObject_IsSolid(actor)
            && gCameraVisibleBox.m_vMax.x > actor->m_Actor.m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < actor->m_Actor.m_Bounds.m_vMax.x
            && gCameraVisibleBox.m_vMax.z > actor->m_Actor.m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < actor->m_Actor.m_Bounds.m_vMax.z
            && gCameraVisibleBox.m_vMax.y > actor->m_Actor.m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < actor->m_Actor.m_Bounds.m_vMax.y) {
            if (world->m_nDrawActors != 0x200) {
                world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)actor;
            }
        }
    }
}
