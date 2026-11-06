/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802BA5F0_22.CPP, func_802BA350_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
struct OSIoMesgHdr_func_802BA350_de;
struct OSIoMesgHdr_func_802BA350_de {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
};
struct OSDevMgr_func_802BA350_de;
struct OSDevMgr_func_802BA350_de {
    u32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(void *, s32, u32, void *, u32);
};
struct OSIoMesgCode802BA23C;
typedef struct OSIoMesgCode802BA23C OSIoMesgCode802BA23C;
struct OSIoMesgHdr_func_802BA350_de;
typedef struct OSIoMesgHdr_func_802BA350_de OSIoMesgHdr_func_802BA350_de;
struct OSIoMesgCode802BA23C;
struct OSIoMesgCode802BA23C {
    OSIoMesgHdr_func_802BA350_de hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
};
struct OSDevMgr_func_802BA350_de;
typedef struct OSDevMgr_func_802BA350_de OSDevMgr_func_802BA350_de;
typedef u32 OSIntMask;
extern void __osRestoreInt(OSIntMask mask);
extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 msgCount);
extern OSIntMask __osDisableInt(void);
extern u64 __osViThreadStack[];
extern void viMgrMain(void *arg);
extern OSIoMesgCode802BA23C __osViCounterMsg;
extern OSIoMesgCode802BA23C __osViRetraceMsg;
extern u32 __osDisableInt(void);
extern void __osTimerServicesInit();
extern void __osViInit(void);
extern void osCreateViManager(s32 pri);
extern OSThread __osViThread;
extern u64 __osViThreadStack[];
extern OSMesgQueue __osViEventQueue;
extern OSMesg __osViEventBuf[5];
extern OSIoMesgCode802BA23C __osViRetraceMsg;
extern OSIoMesgCode802BA23C __osViCounterMsg;
extern void viMgrMain(void *arg);
extern void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg);
extern s32 osGetThreadPri(OSThread *t);
extern void osSetThreadPri(OSThread *t, s32 pri);
extern void osCreateThread(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
extern void osStartThread(OSThread *t);
extern u32 __osDisableInt(void);
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
