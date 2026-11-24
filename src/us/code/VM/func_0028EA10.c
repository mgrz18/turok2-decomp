#include "common.h"
#include "structs/pager.h"

/* Pushes node at the head of the chain. */
void VM_ListAddHead(PageChain *chain, PageMapping *node) {
    PageMapping *head;

    node->prev = NULL;
    node->next = chain->head;
    head = chain->head;
    if (head != NULL) {
        head->prev = node;
    }
    chain->head = node;
    if (chain->tail == NULL) {
        chain->tail = node;
    }
}
