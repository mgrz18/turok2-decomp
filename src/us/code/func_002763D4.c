#include "common.h"
#include "m2c_macros.h"

typedef struct Node { struct Node *next, *prev; } Node;

Node *func_002763D4(Node *arg0) {
    arg0->next->prev = arg0->prev;
    arg0->prev->next = arg0->next;
    return arg0;
}
