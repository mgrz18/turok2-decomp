#include "common.h"
#include "structs/swoosh.h"

s32 DList_AddTail(void *, void *);              /* extern */
s32 DList_Remove(void *, void *);               /* extern */

/* Moves a free swoosh point to the end of swoosh arg1's points. */
SwooshNode *Swoosh_TakeNode(CSwooshSystem *arg0, Swoosh *arg1) {
    SwooshNode *temp_s0;

    temp_s0 = arg0->m_FreeNodes.head;
    if (temp_s0 != NULL) {
        DList_Remove(&arg0->m_FreeNodes, temp_s0);
        DList_AddTail(&arg1->nodes, temp_s0);
    }
    return temp_s0;
}
