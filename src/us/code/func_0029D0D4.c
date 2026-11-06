/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* From decompals/ultralib (libultra 2.0I): src/io/leointerrupt.c, __osLeoResume (static there), with Turok 2's symbols. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#define NULL 0



#define MQ_IS_FULL(mq) ((mq)->validCount >= (mq)->msgCount)


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
