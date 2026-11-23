/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259A50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#ifndef TARGET_PORT
void SndQueue_CancelFlags(Lists *arg0, s32 arg1) {
    NodeRecAtAC *var_a2;
    NodeRecAtAC *temp_a3;
    NodeRecAtAC *temp_v0;
    NodeRecAtAC *temp_t0;
    NodeRecAtAC *temp_t1;
    var_a2 = arg0->first.next;
    temp_v0 = &arg0->first;
    if (var_a2 != temp_v0) {
        temp_t1 = &arg0->second;
        temp_t0 = temp_v0;
        do {
            temp_a3 = var_a2->next;
            if ((var_a2->flags & arg1) != 0) {
                NodeRecAtAC *temp_tail;
                var_a2->prev->next = temp_a3;
                var_a2->next->prev = var_a2->prev;
                temp_tail = arg0->second.prev;
                var_a2->next = temp_t1;
                var_a2->prev = temp_tail;
                arg0->second.prev->next = var_a2;
                arg0->second.prev = var_a2;
            }
            var_a2 = temp_a3;
        } while (var_a2 != temp_t0);
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (the view above has N64 offsets; the node's flags (+0xAC)
 * is body.flags). Matching active requests move to the free list's tail. */
void SndQueue_CancelFlags(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if ((((SndNode *)node)->body.flags & arg1) != 0) {
            node->prev->next = next;
            node->next->prev = node->prev;
            node->next = &q->free.link;
            node->prev = q->free.link.prev;
            q->free.link.prev->next = node;
            q->free.link.prev = node;
        }
        node = next;
    }
}
#endif
