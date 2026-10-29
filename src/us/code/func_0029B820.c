/* From decompals/ultralib (libultra 2.0I): src/os/sendmesg.c, osSendMesg. */

typedef unsigned long u32;
typedef long s32;
typedef unsigned short u16;
typedef void *OSMesg;

typedef struct OSThread {
    /* 0x00 */ struct OSThread *next;
    /* 0x04 */ s32 priority;
    /* 0x08 */ struct OSThread **queue;
    /* 0x0C */ struct OSThread *tlnext;
    /* 0x10 */ u16 state;
} OSThread;

typedef struct {
    /* 0x00 */ OSThread *mtqueue;
    /* 0x04 */ OSThread *fullqueue;
    /* 0x08 */ s32 validCount;
    /* 0x0C */ s32 first;
    /* 0x10 */ s32 msgCount;
    /* 0x14 */ OSMesg *msg;
} OSMesgQueue;

#define OS_MESG_BLOCK 1
#define OS_STATE_WAITING 8

extern OSThread *D_800B8920; /* __osRunningThread */
u32 func_0029DFF0(void);                    /* __osDisableInt */
void func_0029E010(u32);                    /* __osRestoreInt */
void func_0029C6DC(OSThread **);             /* __osEnqueueAndYield */
OSThread *func_0029C8C8(OSThread **);       /* __osPopThread */
void func_0029BB10(OSThread *);             /* osStartThread */

s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags) {
    register u32 saveMask;
    register s32 last;

    saveMask = func_0029DFF0();

    while (mq->validCount >= mq->msgCount) {
        if (flags == OS_MESG_BLOCK) {
            D_800B8920->state = OS_STATE_WAITING;
            func_0029C6DC(&mq->fullqueue);
        } else {
            func_0029E010(saveMask);
            return -1;
        }
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;

    if (mq->mtqueue->next != 0) {
        func_0029BB10(func_0029C8C8(&mq->mtqueue));
    }

    func_0029E010(saveMask);
    return 0;
}
