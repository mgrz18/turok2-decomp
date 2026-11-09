#include "common.h"
#include "structs/swoosh.h"

void DList_AddTail(void *, SwooshNode *);           /* extern */
void DList_Remove(void *, SwooshNode *);            /* extern */

/* Takes point arg2 out of swoosh arg1 (and its length out of the swoosh's), back to the free points. */
void SwooshGroup_Remove(CSwooshSystem *arg0, Swoosh *arg1, SwooshNode *arg2) {
    arg1->length = (f32) (arg1->length - arg2->length);
    DList_Remove(&arg1->nodes, arg2);
    DList_AddTail(&arg0->m_FreeNodes, arg2);
}
