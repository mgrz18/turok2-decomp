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
void __osPackRequestData(u8 cmd);
void __osContGetInitData(u8 *valid, EntryRecAt3 *data);
extern void osSetTimer(void *arg0, u64 arg1, u64 arg2, void *arg3, void *arg4);

#ifndef TARGET_PORT /* the port defines osContInit in port/runtime/t2ultra.c */
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
    __osContNumControllers[0] = 4;
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
#endif /* !TARGET_PORT */

void __osPackRequestData(u8 cmd) {
    u8 *ptr;
    __OSContRequesFormat requestformat;
    s32 i;
    u8 *count;
    OSPifRam *pifram;
    for (i = 0; i < 15; i++) {
        ((u32 *)&__osContBuffer)[i] = 0;
    }
    pifram = &__osContBuffer;
    pifram->pifstatus = 1;
    ptr = (u8 *)pifram->ramarray;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    count = __osContNumControllers;
    for (i = 0; i < *count; i++) {
        *(__OSContRequesFormat *)ptr = requestformat;
        ptr += sizeof(requestformat);
    }
    *ptr = 0xFE;
}

void __osContGetInitData(u8 *valid, EntryRecAt3 *data) {
    u8 *ptr;
    __OSContRequesFormat readformat;
    s32 i;
    u8 mask = 0;
    ptr = (u8 *)&__osContBuffer;
    for (i = 0; i < __osContNumControllers[0]; i++, ptr += sizeof(readformat), data++) {
        readformat = *(__OSContRequesFormat *)ptr;
        data->slot = (readformat.rxsize & 0xC0) >> 4;
        if (data->slot != 0) {
            continue;
        }
        data->id = (readformat.typel << 8) | readformat.typeh;
        data->team = readformat.status;
        mask |= 1 << i;
    }
    *valid = mask;
}
