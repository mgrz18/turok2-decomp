#include "common.h"

typedef struct Link {
    /* 0x0 */ struct Link *next;
    /* 0x4 */ struct Link *prev;
} Link;

typedef struct {
    /* 0x0 */ Link *head;
    /* 0x4 */ Link *tail;
} LinkList;

/* Appends arg1 to the list. */
void VM_ListAddTail(LinkList *arg0, Link *arg1) {
    Link *temp_v0;

    arg1->next = NULL;
    arg1->prev = arg0->tail;
    temp_v0 = arg0->tail;
    if (temp_v0 != NULL) {
        temp_v0->next = arg1;
    }
    arg0->tail = arg1;
    if (arg0->head == NULL) {
        arg0->head = arg1;
    }
}
