#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00202EEC(M2C_UNK);                     /* extern */
M2C_UNK func_00203330(M2C_UNK, M2C_UNK);            /* extern */
s32 func_00206380(M2C_UNK, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK osRecvMesg(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK osSendMesg(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
s32 __osDisableInt();                                /* extern */
M2C_UNK __osRestoreInt(s32);                         /* extern */
extern M2C_UNK D_800D8DB0;
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
