/* WRONG CUT: REAL SceneActorPool_Construct size 0x110 (splat cut it into SceneActorPool_Construct + func_0042CFC0 + func_0042D040). */
#include "common.h"
#include "structs/intelligence.h"
#include "structs/level_actor.h"
#include "globals/scene.h"

extern void func_0027BD10(CIntelligence *def);
extern void func_00227664(CPooledLevelActor *actor);

/* Links the 32 actors of the pool into its free list and sets up the two default definitions. */
void SceneActorPool_Construct(CLevelActorPool *pool) {
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

    func_0027BD10(&gLevelActorIntelligence);
    gLevelActorIntelligence.m_Type = 8;
    gLevelActorIntelligence.m_dwTypeFlags = 1;
    gLevelActorIntelligence.m_nTexture = 0;
    gLevelActorIntelligence.m_dwFlags |= 4;

    func_0027BD10(&gLevelActorIntelligenceMP);
    gLevelActorIntelligenceMP.m_Type = 8;
    gLevelActorIntelligenceMP.m_dwTypeFlags = 0;
    gLevelActorIntelligenceMP.m_nTexture = 0;
    gLevelActorIntelligenceMP.m_dwFlags |= 4;
    gLevelActorIntelligenceMP.m_Pickup.m_CollisionHeight = gLevelActorIntelligenceMP.m_Pickup.m_CollisionRadius = 122.88f;

    for (i = 0; i < 32; i++) {
        func_00227664(&pool->m_Actors[i]);
    }
}
