#include "rw/junk_types.h"

/* Unlinks arg1 from the list (DLNode: prev at 0, next at 4; DLList: head at 0, tail at 4). */
void DList_Remove(DLList *arg0, DLNode *arg1) {
    DLNode *temp_v1;
    DLNode *temp_v1_2;

    temp_v1 = arg1->next;
    if (temp_v1 != NULL) {
        temp_v1->prev = arg1->prev;
    }
    temp_v1_2 = arg1->prev;
    if (temp_v1_2 != NULL) {
        temp_v1_2->next = arg1->next;
    }
    if (arg0->head == arg1) {
        arg0->head = arg1->next;
    }
    if (arg0->tail == arg1) {
        arg0->tail = arg1->prev;
    }
    arg0->count = (s32) (arg0->count - 1);
}
