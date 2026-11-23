/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259988_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#ifndef TARGET_PORT
void SndQueue_CancelSoundId(void *arg0, s32 arg1)
{
    OwnerRecAtD8 *owner = arg0;
    Node_func_80259918_de *node = owner->activeNext;
    Node_func_80259918_de *head = (Node_func_80259918_de *)&owner->activePrev;
    Node_func_80259918_de *other = (Node_func_80259918_de *)&owner->otherPrev;
    if (node != head) {
        Node_func_80259918_de *sentinel = head;
        do {
            Node_func_80259918_de *next = node->next;
            if (node->key == arg1) {
                Node_func_80259918_de *tail;
                node->prev->next = next;
                node->next->prev = node->prev;
                tail = owner->otherPrev;
                node->next = other;
                node->prev = tail;
                owner->otherPrev->next = node;
                owner->otherPrev = node;
            }
            node = next;
        } while (node != sentinel);
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (the view above has N64 offsets; the node's key (+0xB0)
 * is body.soundId). Matching active requests move to the free list's tail. */
void SndQueue_CancelSoundId(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if (((SndNode *)node)->body.soundId == arg1) {
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
