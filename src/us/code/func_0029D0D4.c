/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* From decompals/ultralib (libultra 2.0I): src/io/leointerrupt.c, __osLeoResume (static there), with Turok 2's symbols. */

#include "common.h"
#include "ultra_types.h"
#define NULL 0



typedef struct {
    /* 0x00 */ OSMesgQueue *messageQueue;
    /* 0x04 */ OSMesg message;
} __OSEventState;

#define MQ_IS_FULL(mq) ((mq)->validCount >= (mq)->msgCount)

extern __OSEventState __osPiEventState; /* __osEventStateTab[OS_EVENT_PI] */
extern OSThread *__osRunQueue;      /* __osRunQueue */

extern void __osEnqueueThread(OSThread **queue, OSThread *thread);
extern OSThread *__osPopThread(OSThread **queue);

void __osLeoResume(void) {
    __OSEventState *es = &__osPiEventState;
    OSMesgQueue *mq = es->messageQueue;
    s32 last;

    if (mq == NULL || MQ_IS_FULL(mq)) {
        return;
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = es->message;
    mq->validCount++;

    if (mq->mtqueue->next != NULL) {
        __osEnqueueThread(&__osRunQueue, __osPopThread(&mq->mtqueue));
    }
}
