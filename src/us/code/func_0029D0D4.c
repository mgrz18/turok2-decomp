/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* From decompals/ultralib (libultra 2.0I): src/io/leointerrupt.c, __osLeoResume (static there), with Turok 2's symbols. */

typedef long s32;
typedef void *OSMesg;

#define NULL 0

typedef struct OSThread_s {
    /* 0x00 */ struct OSThread_s *next;
    /* 0x04 */ s32 priority;
} OSThread;

typedef struct {
    /* 0x00 */ OSThread *mtqueue;
    /* 0x04 */ OSThread *fullqueue;
    /* 0x08 */ s32 validCount;
    /* 0x0C */ s32 first;
    /* 0x10 */ s32 msgCount;
    /* 0x14 */ OSMesg *msg;
} OSMesgQueue;

typedef struct {
    /* 0x00 */ OSMesgQueue *messageQueue;
    /* 0x04 */ OSMesg message;
} __OSEventState;

#define MQ_IS_FULL(mq) ((mq)->validCount >= (mq)->msgCount)

extern __OSEventState D_801229E0; /* __osEventStateTab[OS_EVENT_PI] */
extern OSThread *D_800B8918;      /* __osRunQueue */

extern void __osEnqueueThread(OSThread **queue, OSThread *thread);
extern OSThread *__osPopThread(OSThread **queue);

void __osLeoResume(void) {
    __OSEventState *es = &D_801229E0;
    OSMesgQueue *mq = es->messageQueue;
    s32 last;

    if (mq == NULL || MQ_IS_FULL(mq)) {
        return;
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = es->message;
    mq->validCount++;

    if (mq->mtqueue->next != NULL) {
        __osEnqueueThread(&D_800B8918, __osPopThread(&mq->mtqueue));
    }
}
