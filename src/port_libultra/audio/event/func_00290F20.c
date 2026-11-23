#include "common.h"

/* the host layout: the queue and its items through the real types (structs/libaudio.h) */
#include "structs/libaudio.h"
void alLink(ALLink *ln, ALLink *to);
void alEvtqNew(ALEventQueue *evtq, ALEventListItem *items, s32 itemCount) {
    s32 i;
    evtq->eventCount = 0;
    evtq->allocList.next = 0;
    evtq->allocList.prev = 0;
    evtq->freeList.next = 0;
    evtq->freeList.prev = 0;
    for (i = 0; i < itemCount; i++) {
        alLink(&items[i].node, &evtq->freeList);
    }
}
