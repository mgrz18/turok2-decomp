/* From decompals/ultralib (libultra 2.0I): src/os/recvmesg.c, osRecvMesg. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_STATE_WAITING 8

u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(OSThread **);
OSThread *__osPopThread(OSThread **);

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags) {
    register u32 saveMask;

    saveMask = __osDisableInt();

    while (mq->validCount == 0) {
        if (flags == OS_MESG_NOBLOCK) {
            __osRestoreInt(saveMask);
            return -1;
        } else {
            __osRunningThread->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->mtqueue);
        }
    }

    if (msg != 0) {
        *msg = mq->msg[mq->first];
    }

    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;

    if (mq->fullqueue->next != 0) {
        osStartThread(__osPopThread(&mq->fullqueue));
    }

    __osRestoreInt(saveMask);
    return 0;
}
