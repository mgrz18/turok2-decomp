/* From decompals/ultralib (libultra 2.0I): src/os/sendmesg.c, osSendMesg. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"
#define OS_MESG_BLOCK 1
#define OS_STATE_WAITING 8

u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(OSThread **);
OSThread *__osPopThread(OSThread **);

s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags) {
    register u32 saveMask;
    register s32 last;

    saveMask = __osDisableInt();

    while (mq->validCount >= mq->msgCount) {
        if (flags == OS_MESG_BLOCK) {
            __osRunningThread->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->fullqueue);
        } else {
            __osRestoreInt(saveMask);
            return -1;
        }
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;

    if (mq->mtqueue->next != 0) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }

    __osRestoreInt(saveMask);
    return 0;
}
