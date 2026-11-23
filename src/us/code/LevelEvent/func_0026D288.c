/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_80279508_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#include "structs/swoosh.h"

s32 DList_AddTail(void *arg0, void *arg1) {
    SwooshLinks *l = arg0;
    DListLink *n = arg1;

    if (l->count == 0) {
        l->head = n;
        l->tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->prev = l->tail;
        n->next = NULL;
        ((DListLink *)l->tail)->next = n;
        l->tail = n;
    }
    return ++l->count;
}
