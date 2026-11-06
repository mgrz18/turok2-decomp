#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "structs/level_actor.h"
#include "globals/engine.h"

extern CBox gCameraVisibleBox;               /* visible bounds */

void CPooledLevelActor__Update(CPooledLevelActor *actor);

/* Releases an actor in use: uncounts it, unlinks it and puts it on the free list. */
static __inline__ void releaseActor(CLevelActorPool *pool, CPooledLevelActor *actor) {
    if (actor->m_PoolFlags & 1) {
        actor->m_PoolFlags &= ~1;
        if (actor->m_pOwnerCount != NULL) {
            (*actor->m_pOwnerCount)--;
        }
        if (actor->m_pPrev != NULL) {
            actor->m_pPrev->m_pNext = actor->m_pNext;
        }
        if (actor->m_pNext != NULL) {
            actor->m_pNext->m_pPrev = actor->m_pPrev;
        }
        if (pool->m_pHead == actor) {
            pool->m_pHead = actor->m_pNext;
        }
        if (pool->m_pTail == actor) {
            pool->m_pTail = actor->m_pPrev;
        }
        actor->m_pNext = pool->m_pFree;
        actor->m_pPrev = NULL;
        pool->m_pFree = actor;
    }
}

/* Updates the pool actors (CPooledLevelActor__Update); releases those whose lifetime ran out this frame
 * or that left the visible bounds. */
void CLevelActorPool__Update(CLevelActorPool *pool) {
    CPooledLevelActor *actor;
    CPooledLevelActor *next;
    s32 expired;

    next = pool->m_pHead;
    while (next != NULL) {
        actor = next;
        next = next->m_pNext;
        expired = 0;
        if (actor->m_Lifetime > 0.0f) {
            actor->m_Lifetime -= gFrameIncrement;
            if (actor->m_Lifetime <= 0.0f) {
                expired = 1;
            }
        }
        if (!(gCameraVisibleBox.m_vMax.x > actor->m_Actor.m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < actor->m_Actor.m_Bounds.m_vMax.x
              && gCameraVisibleBox.m_vMax.z > actor->m_Actor.m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < actor->m_Actor.m_Bounds.m_vMax.z
              && gCameraVisibleBox.m_vMax.y > actor->m_Actor.m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < actor->m_Actor.m_Bounds.m_vMax.y)) {
            expired = 1;
        }
        if (expired) {
            releaseActor(pool, actor);
        } else {
            CPooledLevelActor__Update(actor);
        }
    }
}
