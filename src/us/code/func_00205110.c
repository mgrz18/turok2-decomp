#include "common.h"

s32 func_002065CC(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 D_800D8DB0;
extern s32 D_800D8DCC;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 flags;
} Request;

void func_00205110(s32 arg0, Request *arg1) {
    s32 temp_a0;
    s32 temp_v0;
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
    func_002065CC(0, (s32) arg1);
    arg1->count = (s32) (arg1->count + 1);
    arg1->flags = (s32) (arg1->flags | 0x100);
    temp_v0 = __osDisableInt();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg((s32) &D_800D8DB0, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}
