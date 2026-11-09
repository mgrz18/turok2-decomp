#include "common.h"
#include "protos/ultra.h"

s32 __osPackResetData(void);
s32 __osPackRequestData(s32);
s32 __osContGetInitData(s32, s32);
s32 __osSiGetAccess(void);
s32 __osSiRawStartDma(s32, sptr);

s32 __osSiRelAccess();                            /* extern */
extern s32 __osContBuffer; /* local type: globals/libultra.h declares it (OSPifRam); this file keeps its own view with the __osContLastCmd array */
extern u8 __osContLastCmd[]; /* local type: globals/libultra.h declares it (u8); as an array GCC 2 keeps its address in a register, as the ROM does */

sptr osContReset(s32 arg0, s32 arg1) {
    s32 sp10;
    sptr temp_s0;

    __osSiGetAccess();
    if (__osContLastCmd[0] != 0xFF) {
        __osPackResetData();
        __osSiRawStartDma(1, (sptr) &__osContBuffer);
        osRecvMesg(arg0, 0, 1);
        __osSiRawStartDma(0, (sptr) &__osContBuffer);
        osRecvMesg(arg0, 0, 1);
        __osPackRequestData(0xFF);
        __osSiRawStartDma(1, (sptr) &__osContBuffer);
        osRecvMesg(arg0, 0, 1);
        __osContLastCmd[0] = 0xFF;
    }
    temp_s0 = __osSiRawStartDma(0, (sptr) &__osContBuffer);
    osRecvMesg(arg0, 0, 1);
    __osContGetInitData((sptr) &sp10, arg1);
    __osSiRelAccess();
    return temp_s0;
}
