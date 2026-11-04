#include "common.h"

s32 func_00202EEC(s32);                     /* extern */
s32 func_00203330(s32, s32);            /* extern */
s32 func_00206380(s32, s32, s32);       /* extern */
s32 osRecvMesg(s32 *, s32, s32); /* extern */
s32 osSendMesg(s32 *, s32, s32); /* extern */
s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */
extern s32 D_800D8DB0;
extern s32 D_800D8DCC;

void func_002053A8(void) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&D_800D8DB0, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    func_00202EEC(0);
    do {

    } while (func_00206380(0, 0, 0) != 0);
    func_00203330(0, 1);
    temp_v0 = __osDisableInt();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&D_800D8DB0, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}
