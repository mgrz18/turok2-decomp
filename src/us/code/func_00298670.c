#include "common.h"
#include "protos/ultra.h"

s32 __osPackRequestData(s32);
s32 __osSiGetAccess(void);
s32 __osSiRawStartDma(s32, sptr);

s32 __osSiRelAccess();                            /* extern */
extern s32 __osContBuffer; /* local type: globals/libultra.h declares it (OSPifRam); this file keeps its own view with the __osContLastCmd array */
extern u8 __osContLastCmd[]; /* local type: globals/libultra.h declares it (u8); as an array GCC 2 keeps its address in a register, as the ROM does */

sptr osContStartQuery(s32 arg0) {
    sptr temp_s0;

    __osSiGetAccess();
    if (__osContLastCmd[0] != 0) {
        __osPackRequestData(0);
        __osSiRawStartDma(1, (sptr) &__osContBuffer);
        osRecvMesg(arg0, 0, 1);
    }
    temp_s0 = __osSiRawStartDma(0, (sptr) &__osContBuffer);
    __osContLastCmd[0] = 0;
    __osSiRelAccess();
    return temp_s0;
}
