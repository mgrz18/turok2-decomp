/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802BA5F0_23.CPP, func_802BA4C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
extern void __osViSwapContext();
extern struct __OSViContext *__osViGetCurrentContext(void);
extern u32 osGetCount(void);
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
