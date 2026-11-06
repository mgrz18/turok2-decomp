#include "common.h"
#include "structs/level_actor.h"

extern void CLevelActorPool__Free(CLevelActorPool *pool, CPooledLevelActor *node);

CPooledLevelActor *CLevelActorPool__Alloc(CLevelActorPool *pool) {
    CPooledLevelActor *node;

    if (pool->m_pFree == NULL) {
        CLevelActorPool__Free(pool, pool->m_pTail);
    }
    node = pool->m_pFree;
    pool->m_pFree = node->m_pNext;
    if (pool->m_pHead != NULL) {
        pool->m_pHead->m_pPrev = node;
    }
    node->m_pNext = pool->m_pHead;
    node->m_pPrev = NULL;
    pool->m_pHead = node;
    if (pool->m_pTail == NULL) {
        pool->m_pTail = node;
    }
    node->m_Actor.m_vVelocity.x = 0;
    node->m_Actor.m_vVelocity.y = 0;
    node->m_Actor.m_vVelocity.z = 0;
    node->m_Actor.unk24[0] = 0;
    node->m_Actor.unk24[1] = 0;
    node->m_Actor.unk24[2] = 0;
    node->m_Actor.unk24[3] = 0;
    node->m_Lifetime = 0;
    node->m_PoolFlags |= 1;
    return node;
}
