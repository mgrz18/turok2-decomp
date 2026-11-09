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
extern void osCreateViManager(s32 pri);
extern void viMgrMain(void *arg);
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
