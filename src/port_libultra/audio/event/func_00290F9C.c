#include "common.h"

/* the host layout: the queue and its items through the real types (structs/libaudio.h) */
#include "structs/libaudio.h"
void alLink(ALLink *ln, ALLink *to);
void alUnlink(ALLink *ln);
void alCopy(void *src, void *dst, s32 size);
s32 osSetIntMask(s32);
ALMicroTime alEvtqNextEvent(ALEventQueue *evtq, ALEvent *evt) {
    ALEventListItem *item;
    ALMicroTime delta;
    s32 mask = osSetIntMask(1);
    item = (ALEventListItem *)evtq->allocList.next;
    if (item != 0) {
        alUnlink(&item->node);
        alCopy(&item->evt, evt, sizeof(*evt));
        alLink(&item->node, &evtq->freeList);
        delta = item->delta;
    } else {
        evt->type = -1;
        delta = 0;
    }
    osSetIntMask(mask);
    return delta;
}
