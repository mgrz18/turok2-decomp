#include "common.h"

#include "structs/swoosh.h"

/* The port: a DList (structs/swoosh.h SwooshLinks) with host pointers. */
void DList_Init(SwooshLinks *arg0) {
    arg0->head = NULL;
    arg0->tail = NULL;
    arg0->count = 0;
}
