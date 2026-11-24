#include "common.h"
#include "structs/level_actor.h"

/* Releases arg1 back to the pool's free list. */
void CLevelActorPool__Free(CLevelActorPool *arg0, CPooledLevelActor *arg1) {
    s32 *temp_v1;
    s32 temp_a2;
    CPooledLevelActor *temp_v1_2;
    CPooledLevelActor *temp_v1_3;

    temp_a2 = arg1->m_PoolFlags;
    if (temp_a2 & 1) {
        temp_v1 = arg1->m_pOwnerCount;
        arg1->m_PoolFlags = temp_a2 & ~1;
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = arg1->m_pPrev;
        if (temp_v1_2 != NULL) {
            temp_v1_2->m_pNext = arg1->m_pNext;
        }
        temp_v1_3 = arg1->m_pNext;
        if (temp_v1_3 != NULL) {
            temp_v1_3->m_pPrev = arg1->m_pPrev;
        }
        if (arg0->m_pHead == arg1) {
            arg0->m_pHead = arg1->m_pNext;
        }
        if (arg0->m_pTail == arg1) {
            arg0->m_pTail = arg1->m_pPrev;
        }
        arg1->m_pNext = arg0->m_pFree;
        arg1->m_pPrev = NULL;
        arg0->m_pFree = arg1;
    }
}
