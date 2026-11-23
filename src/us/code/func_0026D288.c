/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_80279508_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#include "structs/swoosh.h"

/* The port: the list words at their host size (structs/swoosh.h: an element links to its
 * previous element in its first word, to its next in its second). */
typedef struct T2ListNode {
    struct T2ListNode *prev;
    struct T2ListNode *next;
} T2ListNode;

s32 DList_AddTail(void *arg0, void *arg1) {
    SwooshLinks *l = arg0;
    T2ListNode *n = arg1;

    if (l->count == 0) {
        l->head = n;
        l->tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->prev = l->tail;
        n->next = NULL;
        ((T2ListNode *)l->tail)->next = n;
        l->tail = n;
    }
    return ++l->count;
}
