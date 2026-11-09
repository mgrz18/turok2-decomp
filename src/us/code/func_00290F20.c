#include "common.h"

#ifdef TARGET_PORT
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
#else

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
} List;

s32 alLink(s32, List *);                 /* extern */

/* Clears the list and adds arg2 0x1C-byte entries from arg1. */
void alEvtqNew(List *arg0, s32 arg1, s32 arg2) {
    s32 var_s0;
    s32 var_s1;

    var_s1 = 0;
    arg0->unk10 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    if (arg2 > 0) {
        var_s0 = arg1;
        do {
            alLink(var_s0, arg0);
            var_s1 += 1;
            var_s0 += 0x1C;
        } while (var_s1 < arg2);
    }
}
#endif
