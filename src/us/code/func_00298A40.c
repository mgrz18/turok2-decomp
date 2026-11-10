/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_14.c, func_802B7880_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern s32 __osSiRawStartDma(s32, sptr);
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern void __osSiCreateAccessQueue();
extern void osCreateMesgQueue(void *, sptr, s32);
extern u64 osGetTime(void);
extern void __osPackRequestData(s32 arg0);
extern void __osContGetInitData(s32 *arg0, sptr arg1);
extern void osSetTimer(void *arg0, u64 arg1, u64 arg2, void *arg3, void *arg4);
s32 osContInit(s32 arg0, s32 *arg1, s32 arg2) {
    s32 sp20[8];
    s32 sp40[6];
    s32 sp58[2];
    u64 value;
    s32 result;
    s32 *initialized = &__osContinitialized;
    if (*initialized != 0) {
        return 0;
    }
    *initialized = 1;
    value = osGetTime();
    if (value <= 0x0165A0BBULL) {
        osCreateMesgQueue(sp40, (sptr)sp58, 1);
        osSetTimer(sp20, 0x0165A0BCULL - value, 0, sp40, sp58);
        osRecvMesg(sp40, sp58, 1);
    }
    __osContNumControllers = 4;
    __osPackRequestData(0);
    __osSiRawStartDma(1, &__osContBuffer);
    osRecvMesg(arg0, sp58, 1);
    result = __osSiRawStartDma(0, &__osContBuffer);
    osRecvMesg(arg0, sp58, 1);
    __osContGetInitData(arg1, arg2);
    __osContLastCmd = 0;
    __osSiCreateAccessQueue();
    osCreateMesgQueue(&__osContMesgQueue, (sptr)&__osContMesgBuff, 1);
    return result;
}
