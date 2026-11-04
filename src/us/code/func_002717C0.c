#include "common.h"

typedef struct {
    /* 0x00 */ u8 q[0x18];          /* OSMesgQueue */
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 count;
} SndLock;

/* The sound manager (see func_00270240): the parts this touches. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x110];
    /* 0x0110 */ SndLock lock;
    /* 0x0130 */ u8 pad130[8];
    /* 0x0138 */ u8 q[0x1D64 - 0x138];
    /* 0x1D64 */ u8 pad1D64[0x1DB8 - 0x1D64];
    /* 0x1DB8 */ u8 pool[0x2B88 - 0x1DB8];
    /* 0x2B88 */ u8 pad2B88[0x2B9C - 0x2B88];
    /* 0x2B9C */ s32 unk2B9C;
    /* 0x2BA0 */ u8 pad2BA0[0x2BC0 - 0x2BA0];
    /* 0x2BC0 */ u8 fx[1];
} SndMgr;

s32 func_0027392C(void *, s32);                /* extern */
s32 func_00276658(void *, s32);                /* extern */
s32 func_00276DFC(void *);                      /* extern */
s32 osRecvMesg(void *, s32, s32);    /* extern */
s32 osSendMesg(void *, s32, s32);    /* extern */
s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

void func_002717C0(SndMgr *arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    void *var_a0;
    SndLock *temp_s0;
    SndLock *temp_s0_2;

    temp_s0 = &arg0->lock;
    temp_a0 = __osDisableInt();
    temp_v1 = temp_s0->count + 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        var_a0 = arg0->pool;
    } else {
        __osRestoreInt(temp_a0);
        var_a0 = arg0->pool;
    }
    func_0027392C(var_a0, arg1);
    func_00276658(arg0->q, arg1);
    temp_s0_2 = &arg0->lock;
    arg0->unk2B9C = 0;
    temp_v0 = __osDisableInt();
    temp_v1_2 = temp_s0_2->count - 1;
    temp_s0_2->count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(temp_s0_2, 0, 1);
    } else {
        __osRestoreInt(temp_v0);
    }
    func_00276DFC(arg0->fx);
}
