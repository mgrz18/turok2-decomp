/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AFE30_de_20.c, func_802B00D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00291270(unsigned char *source, unsigned char *destination, int count);
extern s32 func_0029E230(s32);
void func_00291034(ALEventQueue *evtq, ALEvent *evt, s32 delta)
{
    extern void func_002933E0(ListLink *, ListLink *);
    extern void func_002933B0(ListLink *);
    ALEventListItem *item;
    ALEventListItem *nextItem;
    ListLink *node;
    s32 postAtEnd = 0;
    s32 mask;
    mask = func_0029E230(1);
    item = (ALEventListItem *)evtq->freeList.next;
    if (!item) {
        func_0029E230(mask);
        return;
    }
    func_002933B0((ListLink *)item);
    ((void (*)(void *, void *, s32))func_00291270)(evt, &item->evt, sizeof(*evt));
    if (delta == 0x7FFFFFFF)
        postAtEnd = -1;
    for (node = &evtq->allocList; node != 0; node = node->next) {
        if (!node->next) {
            if (postAtEnd)
                item->delta = 0;
            else
                item->delta = delta;
            func_002933E0((ListLink *)item, node);
            break;
        } else {
            nextItem = (ALEventListItem *)node->next;
            if (delta < nextItem->delta) {
                item->delta = delta;
                nextItem->delta -= delta;
                func_002933E0((ListLink *)item, node);
                break;
            }
            delta -= nextItem->delta;
        }
    }
    func_0029E230(mask);
}
