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
extern s32 func_0029D190();     /* osEPiRawStartDma */
extern void func_0029D690(void *arg); /* __osDevMgrMain */

extern void func_0029B030(void *mq, void *msg, s32 count);  /* osCreateMesgQueue */
extern void func_0029DD10(void);                            /* __osPiCreateAccessQueue */
extern void func_0029B950(s32 event, void *mq, void *msg);  /* osSetEventMesg */
extern s32 func_0029B150(void *thread);                     /* osGetThreadPri */
extern void func_0029B9B0(void *thread, s32 pri);           /* osSetThreadPri */
extern u32 func_0029DFF0(void);                             /* __osDisableInt */
extern void func_0029E010(u32 mask);                        /* __osRestoreInt */
extern void func_0029B060(void *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri); /* osCreateThread */
extern void func_0029BB10(void *thread);                    /* osStartThread */

void func_0029D450(s32 pri, void *cmdQ, void *cmdBuf, s32 cmdMsgCnt) {
    u32 savedMask;
    s32 oldPri;
    s32 myPri;

    if (!D_800B8960.initialized) {
        func_0029B030(cmdQ, cmdBuf, cmdMsgCnt);
        func_0029B030(D_80124D10, D_80124D28, 1);

        if (!D_800B8990) {
            func_0029DD10();
        }

        func_0029B950(8, D_80124D10, (void *)0x22222222);

        oldPri = -1;
        myPri = func_0029B150(NULL);
        if (myPri < pri) {
            oldPri = myPri;
            func_0029B9B0(NULL, pri);
        }

        savedMask = func_0029DFF0();

        D_800B8960.initialized = 1;
        D_800B8960.acsQueue = D_80124DB8;
        D_800B8960.dma_func = osPiRawStartDma;
        D_800B8960.edma_func = func_0029D190;
        D_800B8960.evtQueue = D_80124D10;
        D_800B8960.thread = D_80123AE0;
        D_800B8960.cmdQueue = cmdQ;

        func_0029B060(D_80123AE0, 0, func_0029D690, &D_800B8960, &D_80123D10[0x1000], pri);
        func_0029BB10(D_80123AE0);

        func_0029E010(savedMask);

        if (oldPri != -1) {
            func_0029B9B0(NULL, oldPri);
        }
    }
}
