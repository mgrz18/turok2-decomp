#include "common.h"

/* A node of a doubly linked list: next at 0, prev at 4. */
typedef struct DLNode {
    /* 0x0 */ struct DLNode *next;
    /* 0x4 */ void *prev;
} DLNode;

/* Insert at the head of the list *arg0. */
void func_002767F8(DLNode **arg0, DLNode *arg1) {
    arg1->next = *arg0;
    arg1->prev = arg0;
    (*arg0)->prev = arg1;
    *arg0 = arg1;
}
