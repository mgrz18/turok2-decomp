/* From decompals/ultralib (libultra 2.0I): src/os/sendmesg.c, osSendMesg. */

#include "common.h"
#include "ultra_types.h"
#define OS_MESG_BLOCK 1
#define OS_STATE_WAITING 8

extern OSThread *D_800B8920; /* __osRunningThread */
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(OSThread **);
OSThread *__osPopThread(OSThread **);
void func_0029BB10(OSThread *);             /* osStartThread */

s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags) {
    register u32 saveMask;
    register s32 last;

    saveMask = __osDisableInt();

    while (mq->validCount >= mq->msgCount) {
        if (flags == OS_MESG_BLOCK) {
            D_800B8920->state = OS_STATE_WAITING;
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
        func_0029BB10(__osPopThread(&mq->mtqueue));
    }

    __osRestoreInt(saveMask);
    return 0;
}
