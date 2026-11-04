#include "common.h"
#include "structs/sound.h"

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
    CMutex *temp_s0;
    CMutex *temp_s0_2;

    temp_s0 = &arg0->lock;
    temp_a0 = __osDisableInt();
    temp_v1 = temp_s0->count + 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        var_a0 = &arg0->pool;
    } else {
        __osRestoreInt(temp_a0);
        var_a0 = &arg0->pool;
    }
    func_0027392C(var_a0, arg1);
    func_00276658(&arg0->queue, arg1);
    temp_s0_2 = &arg0->lock;
    arg0->fxMix = 0;
    temp_v0 = __osDisableInt();
    temp_v1_2 = temp_s0_2->count - 1;
    temp_s0_2->count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(temp_s0_2, 0, 1);
    } else {
        __osRestoreInt(temp_v0);
    }
    func_00276DFC(&arg0->fx);
}
