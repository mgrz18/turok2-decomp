/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AFE30_de_20.c, func_802B00D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void alCopy(unsigned char *source, unsigned char *destination, int count);
extern s32 osSetIntMask(s32);
void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, s32 delta)
{
    extern void alLink(ListLink *, ListLink *);
    extern void alUnlink(ListLink *);
    ALEventListItem *item;
    ALEventListItem *nextItem;
    ListLink *node;
    s32 postAtEnd = 0;
    s32 mask;
    mask = osSetIntMask(1);
    item = (ALEventListItem *)evtq->freeList.next;
    if (!item) {
        osSetIntMask(mask);
        return;
    }
    alUnlink((ListLink *)item);
    ((void (*)(void *, void *, s32))alCopy)(evt, &item->evt, sizeof(*evt));
    if (delta == 0x7FFFFFFF)
        postAtEnd = -1;
    for (node = &evtq->allocList; node != 0; node = node->next) {
        if (!node->next) {
            if (postAtEnd)
                item->delta = 0;
            else
                item->delta = delta;
            alLink((ListLink *)item, node);
            break;
        } else {
            nextItem = (ALEventListItem *)node->next;
            if (delta < nextItem->delta) {
                item->delta = delta;
                nextItem->delta -= delta;
                alLink((ListLink *)item, node);
                break;
            }
            delta -= nextItem->delta;
        }
    }
    osSetIntMask(mask);
}
