/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802BA5F0_22.CPP, func_802BA350_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void __osRestoreInt(OSIntMask mask);
extern OSIntMask __osDisableInt(void);
extern void __osTimerServicesInit();
extern void __osViInit(void);
void osCreateViManager(s32 pri);
void viMgrMain(struct OSDevMgr *arg);

void osCreateViManager(s32 pri)
{
    u32 savedMask;
    s32 oldPri;
    s32 myPri;
    if (__osViDevMgr.active) {
        return;
    }
    __osTimerServicesInit();
    osCreateMesgQueue(&__osViEventQueue, __osViEventBuf, 5);
    __osViRetraceMsg.hdr.type = 13;
    __osViRetraceMsg.hdr.pri = 0;
    __osViRetraceMsg.hdr.retQueue = 0;
    __osViCounterMsg.hdr.type = 14;
    __osViCounterMsg.hdr.pri = 0;
    __osViCounterMsg.hdr.retQueue = 0;
    osSetEventMesg(7, &__osViEventQueue, &__osViRetraceMsg);
    osSetEventMesg(3, &__osViEventQueue, &__osViCounterMsg);
    oldPri = -1;
    myPri = osGetThreadPri(0);
    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(0, pri);
    }
    savedMask = __osDisableInt();
    __osViDevMgr.active = 1;
    __osViDevMgr.thread = &__osViThread;
    __osViDevMgr.cmdQueue = &__osViEventQueue;
    __osViDevMgr.evtQueue = &__osViEventQueue;
    __osViDevMgr.acsQueue = 0;
    __osViDevMgr.dma = 0;
    __osViDevMgr.edma = 0;
    osCreateThread(&__osViThread, 0, viMgrMain, &__osViDevMgr, __osViThreadStack + 0x1000 / sizeof(u64), pri);
    __osViInit();
    osStartThread(&__osViThread);
    __osRestoreInt(savedMask);
    if (oldPri != -1) {
        osSetThreadPri(0, oldPri);
    }
}

extern void __osViSwapContext();
extern struct __OSViContext *__osViGetCurrentContext(void);
extern void __osTimerInterrupt(void);

static __inline__ u16 *viRetrace(void) {
    return &__osViRetraceCount;
}

void viMgrMain(struct OSDevMgr *arg)
{
    struct __OSViContext *vc;
    struct OSDevMgr *dm;
    struct OSIoMesg *mb;
    s32 first;
    u32 count;
    u16 *retraceInit;
    u32 *intrCount;
    u32 *baseCounter;
    u64 *currentTime;
    mb = 0;
    first = 0;
    vc = __osViGetCurrentContext();
    retraceInit = &__osViRetraceCount;
    *retraceInit = vc->retraceCount;
    if (*retraceInit == 0) {
        *retraceInit = 1;
    }
    dm = arg;
    while (1) {
        osRecvMesg(dm->evtQueue, (OSMesg *)&mb, 1);
        switch (mb->hdr.type) {
            case 13:
                __osViSwapContext();
                if (--*viRetrace() == 0) {
                    vc = __osViGetCurrentContext();
                    if (vc->msgq != 0) {
                        osSendMesg(vc->msgq, vc->msg, 0);
                    }
                    *viRetrace() = vc->retraceCount;
                }
                intrCount = &__osViIntrCount;
                (*intrCount)++;
                if (first) {
                    count = osGetCount();
                    __osCurrentTime = count;
                    first = 0;
                }
                baseCounter = &__osBaseCounter;
                count = *baseCounter;
                *baseCounter = osGetCount();
                count = *baseCounter - count;
                currentTime = &__osCurrentTime;
                *currentTime = *currentTime + count;
                break;
            case 14:
                __osTimerInterrupt();
                break;
            default:
                break;
        }
    }
}
