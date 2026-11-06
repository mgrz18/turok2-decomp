#include "common.h"

typedef struct Link {
    /* 0x0 */ struct Link *next;
    /* 0x4 */ struct Link *prev;
} Link;

typedef struct {
    /* 0x0 */ Link *head;
    /* 0x4 */ Link *tail;
} LinkList;

/* Pushes the node at the head of the list. */
void VM_ListAddHead(LinkList *arg0, Link *arg1) {
    Link *temp_v0;

    arg1->prev = 0;
    arg1->next = arg0->head;
    temp_v0 = arg0->head;
    if (temp_v0 != NULL) {
        temp_v0->prev = arg1;
    }
    arg0->head = arg1;
    if (arg0->tail == NULL) {
        arg0->tail = arg1;
    }
}
