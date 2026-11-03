#include "common.h"
#include "m2c_macros.h"

s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern M2C_UNK D_800D8DB0;
extern s32 D_800D8DCC;

void func_002051F4(s32 arg0, void *arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = __osDisableInt();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg((s32) &D_800D8DB0, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_v0 = M2C_FIELD(arg1, s32 *, 8) - 1;
    M2C_FIELD(arg1, s32 *, 8) = temp_v0;
    if (temp_v0 == 0) {
        M2C_FIELD(arg1, s32 *, 0xC) = (s32) (M2C_FIELD(arg1, s32 *, 0xC) & ~0x100);
    }
    temp_v0_2 = __osDisableInt();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0_2);
        osSendMesg((s32) &D_800D8DB0, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0_2);
}
