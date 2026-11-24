#include "common.h"
#include "structs/swoosh.h"

s32 DList_AddTail(void *list, void *node);
void DList_Remove(void *list, void *node);

/* Takes point arg2 out of swoosh arg1 (and its length out of the swoosh's), back to the free points. */
void SwooshGroup_Remove(CSwooshSystem *arg0, Swoosh *arg1, SwooshNode *arg2) {
    arg1->length = (f32) (arg1->length - arg2->length);
    DList_Remove(&arg1->nodes, arg2);
    DList_AddTail(&arg0->m_FreeNodes, arg2);
}
