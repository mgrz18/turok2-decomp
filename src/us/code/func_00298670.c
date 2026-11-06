#include "common.h"

s32 __osPackRequestData(s32);
s32 __osSiGetAccess(void);
s32 __osSiRawStartDma(s32, s32);
s32 osRecvMesg(s32, s32, s32);

s32 __osSiRelAccess();                            /* extern */
extern s32 D_801213E0;
extern u8 D_80121420[];

s32 osContStartQuery(s32 arg0) {
    s32 temp_s0;

    __osSiGetAccess();
    if (D_80121420[0] != 0) {
        __osPackRequestData(0);
        __osSiRawStartDma(1, (s32) &D_801213E0);
        osRecvMesg(arg0, 0, 1);
    }
    temp_s0 = __osSiRawStartDma(0, (s32) &D_801213E0);
    D_80121420[0] = 0;
    __osSiRelAccess();
    return temp_s0;
}
