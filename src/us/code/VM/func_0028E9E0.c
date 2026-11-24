#include "common.h"
#include "structs/pager.h"

/* Appends node to the chain. */
void VM_ListAddTail(PageChain *chain, PageMapping *node) {
    PageMapping *tail;

    node->next = NULL;
    node->prev = chain->tail;
    tail = chain->tail;
    if (tail != NULL) {
        tail->next = node;
    }
    chain->tail = node;
    if (chain->head == NULL) {
        chain->head = node;
    }
}
