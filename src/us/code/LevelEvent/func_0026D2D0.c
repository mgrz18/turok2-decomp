#include "common.h"

/* A node of a doubly linked list: next at 0, prev at 4. */
typedef struct DLNode {
    /* 0x0 */ struct DLNode *next;
    /* 0x4 */ struct DLNode *prev;
} DLNode;

typedef struct {
    /* 0x0 */ DLNode *tail;
    /* 0x4 */ DLNode *head;
    /* 0x8 */ s32 count;
} DLList;

/* Unlinks arg1 from the list. */
void DList_Remove(DLList *arg0, DLNode *arg1) {
    DLNode *temp_v1;
    DLNode *temp_v1_2;

    temp_v1 = arg1->prev;
    if (temp_v1 != NULL) {
        temp_v1->next = arg1->next;
    }
    temp_v1_2 = arg1->next;
    if (temp_v1_2 != NULL) {
        temp_v1_2->prev = arg1->prev;
    }
    if (arg0->tail == arg1) {
        arg0->tail = arg1->prev;
    }
    if (arg0->head == arg1) {
        arg0->head = arg1->next;
    }
    arg0->count = (s32) (arg0->count - 1);
}
