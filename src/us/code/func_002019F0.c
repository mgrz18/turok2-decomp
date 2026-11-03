#include "common.h"
#include "m2c_macros.h"

M2C_UNK osRecvMesg(void *, M2C_UNK, M2C_UNK);    /* extern */
M2C_UNK osSendMesg(void *, M2C_UNK, M2C_UNK);    /* extern */
s32 __osDisableInt();                                /* extern */
M2C_UNK __osRestoreInt(s32);                         /* extern */

void func_002019F0(void *arg0) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    if (M2C_FIELD(arg0, s32 *, 0x1C) != 0) {
        temp_a0 = __osDisableInt();
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x1C) + 1;
        M2C_FIELD(arg0, s32 *, 0x1C) = temp_v1;
        if (temp_v1 != 1) {
            __osRestoreInt(temp_a0);
            osRecvMesg(arg0, 0, 1);
        } else {
            __osRestoreInt(temp_a0);
        }
        temp_v0 = __osDisableInt();
        temp_v1_2 = M2C_FIELD(arg0, s32 *, 0x1C) - 1;
        M2C_FIELD(arg0, s32 *, 0x1C) = temp_v1_2;
        if (temp_v1_2 != 0) {
            __osRestoreInt(temp_v0);
            osSendMesg(arg0, 0, 1);
            return;
        }
        __osRestoreInt(temp_v0);
    }
}
