#include "common.h"

s32 __osPackReadData(void);
s32 __osSiGetAccess(void);
s32 __osSiRawStartDma(s32, s32);
s32 osRecvMesg(s32, s32, s32);

s32 __osSiRelAccess();                            /* extern */
extern s32 D_801213E0;
extern u8 D_80121420[];

s32 osContStartReadData(s32 arg0) {
    s32 temp_v0;

    __osSiGetAccess();
    if (D_80121420[0] != 1) {
        __osPackReadData();
        __osSiRawStartDma(1, (s32) &D_801213E0);
        osRecvMesg(arg0, 0, 1);
    }
    temp_v0 = __osSiRawStartDma(0, (s32) &D_801213E0);
    D_80121420[0] = 1;
    __osSiRelAccess();
    return temp_v0;
}
