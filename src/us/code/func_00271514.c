#include "common.h"

typedef struct {
    /* 0x00 */ u8 q[0x18];          /* OSMesgQueue */
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 count;
} SndLock;

/* The sound manager (see func_00270240): its lock. */
typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ SndLock lock;
} SndMgr;

s32 osSendMesg(void *, s32, s32);    /* extern */
s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

/* Releases the sound manager lock. */
void func_00271514(SndMgr *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    SndLock *temp_s0;

    temp_s0 = &arg0->lock;
    temp_v0 = __osDisableInt();
    temp_v1 = temp_s0->count - 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(temp_s0, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}
