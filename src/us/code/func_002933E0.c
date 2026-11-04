#include "common.h"

/* A node of a doubly linked list: next at 0, then the address of whatever points to it. */
typedef struct ListNode {
    /* 0x00 */ struct ListNode *next;
    /* 0x04 */ struct ListNode **prev;
} ListNode;

/* Insert at the head of a doubly linked list: next at 0, prev at 4. */
void func_002933E0(ListNode *arg0, ListNode **arg1) {
    arg0->next = *arg1;
    arg0->prev = arg1;
    if (*arg1 != NULL) {
        (*arg1)->prev = &arg0->next;
    }
    *arg1 = arg0;
}
