#include "common.h"

s32 func_002026EC(s32, s32);
s32 func_002062F8(s32, s32);
s32 func_002065F8(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

extern s32 D_800D1CC0;
extern s32 D_800D8DB0;
extern s32 D_800D8DCC;

/* A queued request on the D_800D81F4 list (protected by the D_800D8DB0
 * message queue): sorted by key, with a busy count and state flags. */
typedef struct CRequest {
    /* 0x00 */ u32 key;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 flags;
    /* 0x10 */ u8 pad10[0x24 - 0x10];
    /* 0x24 */ struct CRequest *next;
} CRequest;

void func_002057B0(s32 arg0, CRequest **arg1, CRequest *arg2) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    CRequest *temp_s0;

    temp_a0 = __osDisableInt();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg((s32) &D_800D8DB0, 0, 1);
    } else {
        __osRestoreInt(temp_a0);
    }
    temp_s0 = *arg1;
    temp_s0->flags = (temp_s0->flags & ~2);
    if (temp_s0->count != 0) {
loop_4:
        do {
            temp_v0 = temp_s0->count - 1;
            temp_s0->count = temp_v0;
            if (temp_v0 != 0) {
                goto loop_4;
            }
            temp_s0->flags = (temp_s0->flags & ~0x100);
        } while (temp_s0->count != 0);
    }
    if (!(temp_s0->flags & 0x702)) {
        func_002065F8(0, (s32) temp_s0);
        func_002026EC((s32) &D_800D1CC0, temp_s0->key);
        func_002062F8(0, (s32) temp_s0);
    }
    *arg1 = arg2;
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
