/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802BA5F0_23.CPP, func_802BA4C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
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
struct OSThread_s;
typedef struct OSThread_s OSThread_s;
struct OSThreadContext;
typedef struct OSThreadContext OSThreadContext;
struct OSThreadContext;
struct OSThreadContext {
    u64 at;
    u64 v0;
    u64 v1;
    u64 a0;
    u64 a1;
    u64 a2;
    u64 a3;
    u64 t0;
    u64 t1;
    u64 t2;
    u64 t3;
    u64 t4;
    u64 t5;
    u64 t6;
    u64 t7;
    u64 s0;
    u64 s1;
    u64 s2;
    u64 s3;
    u64 s4;
    u64 s5;
    u64 s6;
    u64 s7;
    u64 t8;
    u64 t9;
    u64 gp;
    u64 sp;
    u64 s8;
    u64 ra;
    u64 lo;
    u64 hi;
    u32 sr;
    u32 pc;
    u32 cause;
    u32 badvaddr;
    u32 rcp;
    u32 fpcsr;
};
struct OSThread_s;
struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
    int fp;
    OSThreadContext context;
};
struct OSMesgQueue;
struct OSThread_s;
struct OSPiHandle_s;
typedef struct OSDevMgr OSDevMgr;
struct OSDevMgr {
    s32 active;
    struct OSThread_s *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(struct OSPiHandle_s *, s32, u32, void *, u32);
};
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
struct OSThread_s;
struct OSMesgQueue;
struct OSThread_s;
struct OSPiHandle_s;
typedef struct OSPiHandle_s OSPiHandle_s;
struct OSPiHandle_s {
    struct OSPiHandle_s *next;
    u8 type;
    u8 latency;
    u8 pageSize;
    u8 relDuration;
    u8 pulse;
    u8 domain;
    u32 baseAddress;
    u32 speed;
    u8 transferInfo[0x60];
};
struct OSPiHandle_s;
typedef void *OSMesg;
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
extern u64 D_80122A28;
extern s32 D_80122A20;
extern s32 D_80122A24;
struct OSIoMesg;
typedef struct OSIoMesg OSIoMesg;
struct OSIoMesgHdr;
typedef struct OSIoMesgHdr OSIoMesgHdr;
struct OSPiHandle_s;
struct __OSViContext;
typedef struct __OSViContext __OSViContext;
struct Opaque_OSMesgQueue;
typedef struct Opaque_OSMesgQueue Opaque_OSMesgQueue;
struct OSIoMesgHdr;
struct OSIoMesgHdr {
    u16 type;
    u8 pri;
    u8 status;
    Opaque_OSMesgQueue *retQueue;
};
struct OSIoMesg;
struct OSIoMesg {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
};
struct __OSViContext;
struct __OSViContext {
    u16 state;
    u16 retraceCount;
    void *framep;
    void *modep;
    u32 control;
    Opaque_OSMesgQueue *msgq;
    void * msg;
};
extern void func_0029ACE0();
extern u16 D_801216D0;
extern struct __OSViContext *func_0029AA10(void);
extern u32 func_0029DFC0(void);
extern void func_0029BC70(void);
static __inline__ u16 *viRetrace(void) {
    return &D_801216D0;
}
void func_0029A890(struct OSDevMgr *arg)
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
    vc = func_0029AA10();
    retraceInit = &D_801216D0;
    *retraceInit = vc->retraceCount;
    if (*retraceInit == 0) {
        *retraceInit = 1;
    }
    dm = arg;
    while (1) {
        osRecvMesg(dm->evtQueue, (OSMesg *)&mb, 1);
        switch (mb->hdr.type) {
            case 13:
                func_0029ACE0();
                if (--*viRetrace() == 0) {
                    vc = func_0029AA10();
                    if (vc->msgq != 0) {
                        osSendMesg(vc->msgq, vc->msg, 0);
                    }
                    *viRetrace() = vc->retraceCount;
                }
                intrCount = &D_80122A24;
                (*intrCount)++;
                if (first) {
                    count = func_0029DFC0();
                    D_80122A28 = count;
                    first = 0;
                }
                baseCounter = &D_80122A20;
                count = *baseCounter;
                *baseCounter = func_0029DFC0();
                count = *baseCounter - count;
                currentTime = &D_80122A28;
                *currentTime = *currentTime + count;
                break;
            case 14:
                func_0029BC70();
                break;
            default:
                break;
        }
    }
}
