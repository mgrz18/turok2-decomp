#include "common.h"
#include "ultra_types.h"
#include "protos/ultra.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

void CMutex__Wait(CMutex *arg0) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    if (arg0->count != 0) {
        temp_a0 = __osDisableInt();
        temp_v1 = arg0->count + 1;
        arg0->count = temp_v1;
        if (temp_v1 != 1) {
            __osRestoreInt(temp_a0);
            osRecvMesg(arg0, 0, 1);
        } else {
            __osRestoreInt(temp_a0);
        }
        temp_v0 = __osDisableInt();
        temp_v1_2 = arg0->count - 1;
        arg0->count = temp_v1_2;
        if (temp_v1_2 != 0) {
            __osRestoreInt(temp_v0);
            osSendMesg(arg0, 0, 1);
            return;
        }
        __osRestoreInt(temp_v0);
    }
}
