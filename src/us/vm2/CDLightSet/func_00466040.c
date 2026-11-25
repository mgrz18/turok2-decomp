#include "common.h"
#include "protos/list.h"

#include "structs/particle.h"

/* The vm2 copy of CDLightSet__Construct: an empty used list and the 8 lights on the free list,
 * each with no light record, full colour and disabled. */
void CDLightSet__Construct_2(CDLightSet *pool) {
    s32 i;
    CDLight *node;

    CList__Construct(&pool->m_UsedList, 0, sizeof(void *));
    CList__Construct(&pool->m_FreeList, 0, sizeof(void *));
    for (i = 0; i < 8; i++) {
        node = &pool->m_Lights[i];
        CList__AddTail(&pool->m_FreeList, node);
        node->m_pROMLight = 0;
        node->m_ColorFade = 1.0f;
        node->m_bEnabled = 0;
    }
}
