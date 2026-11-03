#include "common.h"
#include "m2c_macros.h"

M2C_UNK osRecvMesg(void *, M2C_UNK, M2C_UNK);    /* extern */
s32 __osDisableInt();                                /* extern */
M2C_UNK __osRestoreInt(s32);                         /* extern */

void func_002714B0(s32 arg0) {
    s32 temp_a0;
    s32 temp_v1;
    void *temp_s0;

    temp_s0 = arg0 + 0x110;
    temp_a0 = __osDisableInt();
    temp_v1 = M2C_FIELD(temp_s0, s32 *, 0x1C) + 1;
    M2C_FIELD(temp_s0, s32 *, 0x1C) = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        return;
    }
    __osRestoreInt(temp_a0);
}
