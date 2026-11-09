/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_802794C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#ifdef TARGET_PORT
#include "structs/swoosh.h"

/* The port: the list words at their host size (structs/swoosh.h: an element links to its
 * previous element in its first word, to its next in its second). */
typedef struct T2ListNode {
    struct T2ListNode *prev;
    struct T2ListNode *next;
} T2ListNode;

void DList_AddHead(void *arg0, void *arg1) {
    SwooshLinks *l = arg0;
    T2ListNode *n = arg1;

    if (l->count == 0) {
        l->head = n;
        l->tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->next = l->head;
        n->prev = NULL;
        ((T2ListNode *)l->head)->prev = n;
        l->head = n;
    }
    l->count++;
}
#else
void DList_AddHead(void *arg0, void *arg1) {
    if ((((struct ListHeader *) ((s8 *) arg0))->count) == 0) {
        (((struct ListHeader *) ((s8 *) arg0))->head) = arg1;
        (((struct ListHeader *) ((s8 *) arg0))->tail) = arg1;
        (((struct Shape_typemap_3 *) ((s8 *) arg1))->field_0) = 0;
        (((struct func_80203E78_S1 *) ((s8 *) arg1))->unk4) = 0;
    } else {
        (((struct Field_void_4 *) ((s8 *) arg1))->value) = (((struct ListHeader *) ((s8 *) arg0))->head);
        (((struct Shape_typemap_3 *) ((s8 *) arg1))->field_0) = 0;
        (((struct func_80284AF4_G2 *) ((s8 *) ((struct ListHeader *) ((s8 *) arg0))->head))->unk0) = arg1;
        (((struct ListHeader *) ((s8 *) arg0))->head) = arg1;
    }
    (((struct ListHeader *) ((s8 *) arg0))->count) = (((struct ListHeader *) ((s8 *) arg0))->count) + 1;
}
#endif
