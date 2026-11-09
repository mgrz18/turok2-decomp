#include "common.h"
#include "m2c_macros.h"
#include "protos/list.h"


#ifdef TARGET_PORT
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
#else
void CInstancePool__Construct_2(s32 arg0) {
    sptr var_s0;
    s32 var_s1;

    CList__Construct(arg0 + 0x5300, 0x288, 0x28C);
    CList__Construct(arg0 + 0x5314, 0x288, 0x28C);
    var_s1 = 0;
    var_s0 = arg0;
    do {
        CList__AddTail(arg0 + 0x5300, var_s0);
        var_s1 += 1;
        var_s0 += 0x298;
    } while (var_s1 < 0x20);
}
#endif
