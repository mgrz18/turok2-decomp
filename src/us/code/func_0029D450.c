/* Boundary fixed in versions/cuts.us.txt: splat starts this function one word early; the word at
 * 0x29D44C (lw v0,0x14(a0)) is never executed and the real function,
 * libultra osCreatePiManager, starts at 0x29D450 (0x178 bytes). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/osCreatePiManager.c, osCreatePiManager, with Turok 2's layouts. */

#include "common.h"
#include "globals/libultra.h"

extern u8 __osPiThread[];         /* __osPiThread */
extern u8 __osPiEventQueue[];         /* __osPiEventQueue */
extern u8 __osPiEventBuf[];         /* __osPiEventBuf */
extern u8 __osPiAccessQueue[];         /* __osPiAccessQueue */
extern u8 __osPiThreadStack[];         /* __osPiThreadStack (its end is __osPiEventQueue) */
extern s32 osPiRawStartDma();
extern s32 osEPiRawStartDma();     /* osEPiRawStartDma */
extern void __osDevMgrMain(void *arg); /* __osDevMgrMain */

extern void osCreateMesgQueue(void *mq, void *msg, s32 count);  /* osCreateMesgQueue */
extern void __osPiCreateAccessQueue(void);                            /* __osPiCreateAccessQueue */
extern void osSetEventMesg(s32 event, void *mq, void *msg);  /* osSetEventMesg */
extern s32 osGetThreadPri(void *thread);                     /* osGetThreadPri */
extern void osSetThreadPri(void *thread, s32 pri);           /* osSetThreadPri */
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
extern void osCreateThread(void *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri); /* osCreateThread */
extern void osStartThread(void *thread);                    /* osStartThread */

void osCreatePiManager(s32 pri, void *cmdQ, void *cmdBuf, s32 cmdMsgCnt) {
    u32 savedMask;
    s32 oldPri;
    s32 myPri;

    if (!__osPiDevMgr.active) {
        osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
        osCreateMesgQueue(__osPiEventQueue, __osPiEventBuf, 1);

        if (!__osPiAccessQueueEnabled) {
            __osPiCreateAccessQueue();
        }

        osSetEventMesg(8, __osPiEventQueue, (void *)0x22222222);

        oldPri = -1;
        myPri = osGetThreadPri(NULL);
        if (myPri < pri) {
            oldPri = myPri;
            osSetThreadPri(NULL, pri);
        }

        savedMask = __osDisableInt();

        __osPiDevMgr.active = 1;
        __osPiDevMgr.acsQueue = (OSMesgQueue *)__osPiAccessQueue;
        __osPiDevMgr.dma = osPiRawStartDma;
        __osPiDevMgr.edma = osEPiRawStartDma;
        __osPiDevMgr.evtQueue = (OSMesgQueue *)__osPiEventQueue;
        __osPiDevMgr.thread = (OSThread *)__osPiThread;
        __osPiDevMgr.cmdQueue = (OSMesgQueue *)cmdQ;

        osCreateThread(__osPiThread, 0, __osDevMgrMain, &__osPiDevMgr, &__osPiThreadStack[0x1000], pri);
        osStartThread(__osPiThread);

        __osRestoreInt(savedMask);

        if (oldPri != -1) {
            osSetThreadPri(NULL, oldPri);
        }
    }
}
