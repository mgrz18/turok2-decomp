#include "common.h"
#include "structs/sound.h"

s32 osSendMesg(void *, s32, s32);    /* extern */
s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

/* Releases the sound manager lock. */
void CAudioWorld__Unlock(CAudioWorld *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    CMutex *temp_s0;

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
