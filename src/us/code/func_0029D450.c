/* Boundary fixed in versions/cuts.us.txt: splat starts this function one word early; the word at
 * 0x29D44C (lw v0,0x14(a0)) is never executed and the real function,
 * libultra osCreatePiManager, starts at 0x29D450 (0x178 bytes). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/osCreatePiManager.c, osCreatePiManager, with Turok 2's layouts. */

#include "common.h"

typedef struct {
    /* 0x00 */ s32 initialized;
    /* 0x04 */ void *thread;
    /* 0x08 */ void *cmdQueue;
    /* 0x0C */ void *evtQueue;
    /* 0x10 */ void *acsQueue;
    /* 0x14 */ void *dma_func;
    /* 0x18 */ void *edma_func;
} OSDevMgr; /* __osPiDevMgr */

extern OSDevMgr D_800B8960;     /* __osPiDevMgr */
extern s32 D_800B8990;          /* __osPiAccessQueueEnabled */
extern u8 D_80123AE0[];         /* __osPiThread */
extern u8 D_80124D10[];         /* __osPiEventQueue */
extern u8 D_80124D28[];         /* __osPiEventBuf */
extern u8 D_80124DB8[];         /* __osPiAccessQueue */
extern u8 D_80123D10[];         /* __osPiThreadStack (its end is __osPiEventQueue) */
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

    if (!D_800B8960.initialized) {
        osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
        osCreateMesgQueue(D_80124D10, D_80124D28, 1);

        if (!D_800B8990) {
            __osPiCreateAccessQueue();
        }

        osSetEventMesg(8, D_80124D10, (void *)0x22222222);

        oldPri = -1;
        myPri = osGetThreadPri(NULL);
        if (myPri < pri) {
            oldPri = myPri;
            osSetThreadPri(NULL, pri);
        }

        savedMask = __osDisableInt();

        D_800B8960.initialized = 1;
        D_800B8960.acsQueue = D_80124DB8;
        D_800B8960.dma_func = osPiRawStartDma;
        D_800B8960.edma_func = osEPiRawStartDma;
        D_800B8960.evtQueue = D_80124D10;
        D_800B8960.thread = D_80123AE0;
        D_800B8960.cmdQueue = cmdQ;

        osCreateThread(D_80123AE0, 0, __osDevMgrMain, &D_800B8960, &D_80123D10[0x1000], pri);
        osStartThread(D_80123AE0);

        __osRestoreInt(savedMask);

        if (oldPri != -1) {
            osSetThreadPri(NULL, oldPri);
        }
    }
}
