/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802BA5F0_23.CPP, func_802BA4C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
struct OSPiHandle_s;
typedef struct OSDevMgr OSDevMgr;
struct OSDevMgr {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(struct OSPiHandle_s *, s32, u32, void *, u32);
};
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
struct OSIoMesgHdr;
struct OSIoMesgHdr {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
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
    OSMesgQueue *msgq;
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
