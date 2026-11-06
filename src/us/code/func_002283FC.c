#include "common.h"
#include "structs/game_object.h"

extern s32 CList__AddHead(CList *, CListedInstance *);
extern s32 CList__Remove(CList *, CListedInstance *);
extern s32 gScene_m_bStreamAll;

/* Takes a free object (when fewer than three are active, or always while
 * gScene_m_bStreamAll is set), makes it active and counts it in *arg1. */
CListedInstance *CInstancePool__Alloc(CInstancePool *arg0, s32 *arg1) {
    CListedInstance *node;

    if (gScene_m_bStreamAll != 0 || (u32) arg0->m_ActiveList.m_nCount < 3) {
        node = arg0->m_FreeList.m_pHead;
        if (node != 0) {
            CList__Remove(&arg0->m_FreeList, node);
            CList__AddHead(&arg0->m_ActiveList, node);
            node->m_pOwnerCount = arg1;
            if (arg1 != NULL) {
                *arg1 += 1;
            }
        }
        return node;
    }
    return 0;
}
