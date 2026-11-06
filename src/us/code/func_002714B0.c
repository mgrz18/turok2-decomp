#include "common.h"
#include "ultra_types.h"

s32 osRecvMesg(void *, s32, s32);    /* extern */
s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

void CAudioWorld__Lock(s32 arg0) {
    s32 temp_a0;
    s32 temp_v1;
    CMutex *temp_s0;

    temp_s0 = (CMutex *) (arg0 + 0x110);
    temp_a0 = __osDisableInt();
    temp_v1 = temp_s0->count + 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        return;
    }
    __osRestoreInt(temp_a0);
}
