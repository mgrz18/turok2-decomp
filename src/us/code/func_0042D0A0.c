#include "common.h"
#include "m2c_macros.h"
#include "protos/list.h"


#include "structs/game_object.h"

/* The N64 body below walks the pool by its N64 offsets (0x5300, 0x298, links at 0x288). */
void CInstancePool__Construct(CInstancePool *pool) {
    s32 i;

    CList__Construct(&pool->m_FreeList, T2_OFFSETOF(CListedInstance, m_pPrev), T2_OFFSETOF(CListedInstance, m_pNext));
    CList__Construct(&pool->m_ActiveList, T2_OFFSETOF(CListedInstance, m_pPrev), T2_OFFSETOF(CListedInstance, m_pNext));
    for (i = 0; i < 32; i++) {
        CList__AddTail(&pool->m_FreeList, &pool->m_Instances[i]);
    }
}
