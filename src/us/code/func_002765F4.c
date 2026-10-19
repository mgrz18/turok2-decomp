/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_802598B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002765F4(struct ChannelSet *arg0, s32 arg1);
void func_002765F4(ChannelSet *arg0, s32 arg1) {
    Node_func_802598B4_de *node;
    Node_func_802598B4_de *next;
    Node_func_802598B4_de *end;
    Node_func_802598B4_de *initial_end;
    Node_func_802598B4_de *tail_end;
    node = (Node_func_802598B4_de *)arg0->free.prev;
    tail_end = (Node_func_802598B4_de *)&arg0->active;
    initial_end = (Node_func_802598B4_de *)&arg0->free;
    if (node != initial_end) {
        end = initial_end;
        do {
            next = node->next;
            if (node->value == arg1) {
                node->prev->next = next;
                node->next->prev = node->prev;
                node->prev = (Node_func_802598B4_de *)arg0->active.next;
                node->next = tail_end;
                ((Node_func_802598B4_de *)arg0->active.next)->next = node;
                arg0->active.next = (ListLink *)node;
            }
            node = next;
        } while (node != end);
    }
}
