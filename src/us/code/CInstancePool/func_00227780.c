#include "common.h"
#include "structs/level_actor.h"
#include "globals/scene.h"

/* Unlinks an active node and pushes it on the free list. */
void CPooledLevelActor__Release(CPooledLevelActor *arg0) {
    s32 *temp_v1;
    s32 temp_a1;
    CPooledLevelActor *temp_v1_2;
    CPooledLevelActor *temp_v1_3;
    CPooledLevelActor **nextp;

    temp_a1 = arg0->m_PoolFlags;
    if (temp_a1 & 1) {
        temp_v1 = arg0->m_pOwnerCount;
        arg0->m_PoolFlags = (s32) (temp_a1 & ~1);
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = arg0->m_pPrev;
        if (temp_v1_2 != NULL) {
            temp_v1_2->m_pNext = arg0->m_pNext;
        }
        temp_v1_3 = arg0->m_pNext;
        if (temp_v1_3 != NULL) {
            temp_v1_3->m_pPrev = arg0->m_pPrev;
        }
        if (gScene_m_ActorPool_m_pHead == arg0) {
            gScene_m_ActorPool_m_pHead = arg0->m_pNext;
        }
        if (gScene_m_ActorPool_m_pTail == arg0) {
            gScene_m_ActorPool_m_pTail = arg0->m_pPrev;
        }
        nextp = &arg0->m_pNext;
        *nextp = gScene_m_ActorPool_m_pFree;
        arg0->m_pPrev = NULL;
        gScene_m_ActorPool_m_pFree = arg0;
    }
}
