/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_802599EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0027672C(OwnerRecAtD8 *arg0, s32 arg1) {
    ListNode *node;
    ListNode *next;
    ListNode *sentinel;
    ListNode *loop_sentinel;
    ListNode *append_sentinel;
    ListNode *tail;
    node = (ListNode *)arg0->activeNext;
    sentinel = (ListNode *)&arg0->activePrev;
    append_sentinel = (ListNode *)&arg0->otherPrev;
    if (node != sentinel) {
        loop_sentinel = sentinel;
        do {
            next = node->next;
            if (node->valueBC == arg1) {
                node->prev->next = next;
                node->next->prev = node->prev;
                tail = (ListNode *)arg0->otherPrev;
                node->next = append_sentinel;
                node->prev = tail;
                ((ListNode *)arg0->otherPrev)->next = node;
                arg0->otherPrev = (Node_func_80259918_de *)node;
            }
            node = next;
        } while (node != loop_sentinel);
    }
}
