/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_14.c, func_802B7880_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int D_800B7A30;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern s32 __osSiRawStartDma(s32, s32);
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern void __osSiCreateAccessQueue();
extern s32 D_801213D8;
extern s32 D_801213C0;
extern u8 D_80121420;
extern void osCreateMesgQueue(void *, s32, s32);
extern u64 osGetTime(void);
extern s8 D_801213DC;
extern void __osPackRequestData(s32 arg0);
extern void __osContGetInitData(s32 *arg0, s32 arg1);
extern void osSetTimer(void *arg0, u64 arg1, u64 arg2, void *arg3, void *arg4);
s32 osContInit(s32 arg0, s32 *arg1, s32 arg2) {
    extern char D_801213E0;
    s32 sp20[8];
    s32 sp40[6];
    s32 sp58[2];
    u64 value;
    s32 result;
    s32 *initialized = &D_800B7A30;
    if (*initialized != 0) {
        return 0;
    }
    *initialized = 1;
    value = osGetTime();
    if (value <= 0x0165A0BBULL) {
        osCreateMesgQueue(sp40, (s32)sp58, 1);
        osSetTimer(sp20, 0x0165A0BCULL - value, 0, sp40, sp58);
        osRecvMesg(sp40, (s32)sp58, 1);
    }
    D_801213DC = 4;
    __osPackRequestData(0);
    __osSiRawStartDma(1, &D_801213E0);
    osRecvMesg(arg0, (s32)sp58, 1);
    result = __osSiRawStartDma(0, &D_801213E0);
    osRecvMesg(arg0, (s32)sp58, 1);
    __osContGetInitData(arg1, arg2);
    D_80121420 = 0;
    __osSiCreateAccessQueue();
    osCreateMesgQueue(&D_801213C0, (s32)&D_801213D8, 1);
    return result;
}
