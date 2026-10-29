/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802BA5F0_22.CPP, func_802BA350_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
struct OSIoMesgHdr_func_802BA350_de;
struct OSMesgQueue;
struct OSIoMesgHdr_func_802BA350_de {
    u16 type;
    u8 pri;
    u8 status;
    struct OSMesgQueue *retQueue;
};
struct OSDevMgr_func_802BA350_de;
struct OSMesgQueue;
struct OSThread;
struct OSDevMgr_func_802BA350_de {
    u32 active;
    struct OSThread *thread;
    struct OSMesgQueue *cmdQueue;
    struct OSMesgQueue *evtQueue;
    struct OSMesgQueue *acsQueue;
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
struct OSMesgQueue;
struct OSMesgQueue;
typedef void *OSMesg;
typedef u32 OSIntMask;
extern void func_0029E010(OSIntMask mask);
extern void func_0029B030(OSMesgQueue *mq, OSMesg *msg, s32 msgCount);
extern OSIntMask func_0029DFF0(void);
struct OSMesgQueue;
extern u64 D_80121910[];
extern void D_0029A890(void *arg);
extern OSIoMesgCode802BA23C D_80122970;
extern OSIoMesgCode802BA23C D_80122950;
extern OSDevMgr_func_802BA350_de D_800B7AD0;
extern u32 func_0029DFF0(void);
extern void func_0029BDB8();
extern void func_0029A5E0(void);
extern void osCreateViManager(s32 pri);
typedef struct OSThread OSThread;
extern OSDevMgr_func_802BA350_de D_800B7AD0;
extern OSThread D_801216E0;
extern u64 D_80121910[];
extern OSMesgQueue D_80122910;
extern OSMesg D_80122930[5];
extern OSIoMesgCode802BA23C D_80122950;
extern OSIoMesgCode802BA23C D_80122970;
extern void D_0029A890(void *arg);
extern void func_0029B950(s32 event, OSMesgQueue *mq, OSMesg msg);
extern s32 func_0029B150(OSThread *t);
extern void func_0029B9B0(OSThread *t, s32 pri);
extern void func_0029B060(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
extern void func_0029BB10(OSThread *t);
extern u32 func_0029DFF0(void);
void osCreateViManager(s32 pri)
{
    u32 savedMask;
    s32 oldPri;
    s32 myPri;
    if (D_800B7AD0.active) {
        return;
    }
    func_0029BDB8();
    func_0029B030(&D_80122910, D_80122930, 5);
    D_80122950.hdr.type = 13;
    D_80122950.hdr.pri = 0;
    D_80122950.hdr.retQueue = 0;
    D_80122970.hdr.type = 14;
    D_80122970.hdr.pri = 0;
    D_80122970.hdr.retQueue = 0;
    func_0029B950(7, &D_80122910, &D_80122950);
    func_0029B950(3, &D_80122910, &D_80122970);
    oldPri = -1;
    myPri = func_0029B150(0);
    if (myPri < pri) {
        oldPri = myPri;
        func_0029B9B0(0, pri);
    }
    savedMask = func_0029DFF0();
    D_800B7AD0.active = 1;
    D_800B7AD0.thread = &D_801216E0;
    D_800B7AD0.cmdQueue = &D_80122910;
    D_800B7AD0.evtQueue = &D_80122910;
    D_800B7AD0.acsQueue = 0;
    D_800B7AD0.dma = 0;
    D_800B7AD0.edma = 0;
    func_0029B060(&D_801216E0, 0, D_0029A890, &D_800B7AD0, D_80121910 + 0x1000 / sizeof(u64), pri);
    func_0029A5E0();
    func_0029BB10(&D_801216E0);
    func_0029E010(savedMask);
    if (oldPri != -1) {
        func_0029B9B0(0, oldPri);
    }
}
