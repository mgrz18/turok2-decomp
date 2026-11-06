/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802BA5F0_22.CPP, func_802BA350_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
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
extern u64 D_80121910[];
extern void viMgrMain(void *arg);
extern OSIoMesgCode802BA23C D_80122970;
extern OSIoMesgCode802BA23C D_80122950;
extern OSDevMgr_func_802BA350_de D_800B7AD0;
extern u32 __osDisableInt(void);
extern void __osTimerServicesInit();
extern void __osViInit(void);
extern void osCreateViManager(s32 pri);
extern OSDevMgr_func_802BA350_de D_800B7AD0;
extern OSThread D_801216E0;
extern u64 D_80121910[];
extern OSMesgQueue D_80122910;
extern OSMesg D_80122930[5];
extern OSIoMesgCode802BA23C D_80122950;
extern OSIoMesgCode802BA23C D_80122970;
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
    if (D_800B7AD0.active) {
        return;
    }
    __osTimerServicesInit();
    osCreateMesgQueue(&D_80122910, D_80122930, 5);
    D_80122950.hdr.type = 13;
    D_80122950.hdr.pri = 0;
    D_80122950.hdr.retQueue = 0;
    D_80122970.hdr.type = 14;
    D_80122970.hdr.pri = 0;
    D_80122970.hdr.retQueue = 0;
    osSetEventMesg(7, &D_80122910, &D_80122950);
    osSetEventMesg(3, &D_80122910, &D_80122970);
    oldPri = -1;
    myPri = osGetThreadPri(0);
    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(0, pri);
    }
    savedMask = __osDisableInt();
    D_800B7AD0.active = 1;
    D_800B7AD0.thread = &D_801216E0;
    D_800B7AD0.cmdQueue = &D_80122910;
    D_800B7AD0.evtQueue = &D_80122910;
    D_800B7AD0.acsQueue = 0;
    D_800B7AD0.dma = 0;
    D_800B7AD0.edma = 0;
    osCreateThread(&D_801216E0, 0, viMgrMain, &D_800B7AD0, D_80121910 + 0x1000 / sizeof(u64), pri);
    __osViInit();
    osStartThread(&D_801216E0);
    __osRestoreInt(savedMask);
    if (oldPri != -1) {
        osSetThreadPri(0, oldPri);
    }
}
