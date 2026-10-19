/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/seqplayer.c, func_802B156C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002924CC(List802B663C *arg0, void *arg1);
void func_002924CC(List802B663C *arg0, void *arg1) {
    Node_func_80239AF4_de *node;
    Node_func_80239AF4_de *prev;
    prev = 0;
    node = arg0->head;
    while (node != 0) {
        if ((void *)(node + 1) == arg1) {
            if (prev != 0) {
                prev->next = node->next;
            } else {
                arg0->head = node->next;
            }
            if (node == arg0->tail) {
                arg0->tail = prev;
            }
            node->next = arg0->free;
            arg0->free = node;
            return;
        }
        prev = node;
        node = node->next;
    }
}
