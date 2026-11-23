#include "common.h"
#include "structs/level_actor.h"
#include "globals/scene.h"

/* Reset the world's pool of run-time actors (all on the free list) and the two
 * pickup effect emitters (gLevelActorIntelligence, gLevelActorIntelligenceMP). */

void CIntelligence__Init(CIntelligence *emitter);
void SceneActor_Init(CPooledLevelActor *actor);

void SceneActorPool_Construct_2(CLevelActorPool *pool) {
    s32 i;

    pool->m_pFree = pool->m_Actors;
    pool->m_pTail = NULL;
    pool->m_pHead = NULL;
    for (i = 0; i < 31; i++) {
        pool->m_Actors[i].m_pNext = &pool->m_Actors[i + 1];
        pool->m_Actors[i].m_pPrev = &pool->m_Actors[i - 1];
    }
    pool->m_Actors[0].m_pPrev = NULL;
    pool->m_Actors[31].m_pNext = NULL;

    CIntelligence__Init(&gLevelActorIntelligence);
    gLevelActorIntelligence.m_Type = 8;
    gLevelActorIntelligence.m_dwTypeFlags = 1;
    gLevelActorIntelligence.m_nTexture = 0;
    gLevelActorIntelligence.m_dwFlags |= 4;
    CIntelligence__Init(&gLevelActorIntelligenceMP);
    gLevelActorIntelligenceMP.m_Type = 8;
    gLevelActorIntelligenceMP.m_dwTypeFlags = 0;
    gLevelActorIntelligenceMP.m_nTexture = 0;
    gLevelActorIntelligenceMP.m_dwFlags |= 4;
    gLevelActorIntelligenceMP.m_Pickup.m_CollisionRadius = 122.88f;
    gLevelActorIntelligenceMP.m_Pickup.m_CollisionHeight = 122.88f;

    for (i = 0; i < 32; i++) {
        SceneActor_Init(&pool->m_Actors[i]);
    }
}

#include "m2c_macros.h"
#include "protos/list.h"
#include "structs/game_object.h"

/* The N64 body below walks the pool by its N64 offsets (0x5300, 0x298, links at 0x288). */
void CInstancePool__Construct_2(CInstancePool *pool) {
    s32 i;

    CList__Construct(&pool->m_FreeList, T2_OFFSETOF(CListedInstance, m_pPrev), T2_OFFSETOF(CListedInstance, m_pNext));
    CList__Construct(&pool->m_ActiveList, T2_OFFSETOF(CListedInstance, m_pPrev), T2_OFFSETOF(CListedInstance, m_pNext));
    for (i = 0; i < 32; i++) {
        CList__AddTail(&pool->m_FreeList, &pool->m_Instances[i]);
    }
}
