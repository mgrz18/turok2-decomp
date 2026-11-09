/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259918_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#ifndef TARGET_PORT
void SndQueue_CancelAllExcept(OwnerRecAtD8 *arg0, s32 arg1) {
    Node_func_80259918_de *node;
    Node_func_80259918_de *next;
    node = arg0->activeNext;
    while (node != (Node_func_80259918_de *) &arg0->activePrev) {
        next = node->next;
        if (arg1 == -1 || node->key != arg1) {
            node->prev->next = next;
            node->next->prev = node->prev;
            do { Node_func_80259918_de *tail_ = (arg0)->otherPrev; (node)->next = (Node_func_80259918_de *) &(arg0)->otherPrev; (node)->prev = tail_; (arg0)->otherPrev->next = (node); (arg0)->otherPrev = (node); } while (0);
        }
        node = next;
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (node key = body.soundId; moved to the free list tail). */
void SndQueue_CancelAllExcept(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if (arg1 == -1 || ((SndNode *)node)->body.soundId != arg1) {
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
