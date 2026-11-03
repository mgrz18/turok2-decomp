/* From decompals/ultralib (libultra 2.0I): src/os/recvmesg.c, osRecvMesg. */

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

#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_STATE_WAITING 8

extern OSThread *D_800B8920; /* __osRunningThread */
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(OSThread **);
OSThread *__osPopThread(OSThread **);
void func_0029BB10(OSThread *);             /* osStartThread */

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags) {
    register u32 saveMask;

    saveMask = __osDisableInt();

    while (mq->validCount == 0) {
        if (flags == OS_MESG_NOBLOCK) {
            __osRestoreInt(saveMask);
            return -1;
        } else {
            D_800B8920->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->mtqueue);
        }
    }

    if (msg != 0) {
        *msg = mq->msg[mq->first];
    }

    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;

    if (mq->fullqueue->next != 0) {
        func_0029BB10(__osPopThread(&mq->fullqueue));
    }

    __osRestoreInt(saveMask);
    return 0;
}
