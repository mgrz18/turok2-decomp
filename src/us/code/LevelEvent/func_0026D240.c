/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_802794C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#include "structs/swoosh.h"

void DList_AddHead(void *arg0, void *arg1) {
    SwooshLinks *l = arg0;
    DListLink *n = arg1;

    if (l->count == 0) {
        l->head = n;
        l->tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->next = l->head;
        n->prev = NULL;
        ((DListLink *)l->head)->prev = n;
        l->head = n;
    }
    l->count++;
}
