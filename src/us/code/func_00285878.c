#include "common.h"
#include "globals/pak.h"

s32 osRecvMesg(void *, s32, s32);         /* extern */

s32 Controller_Lock(s32 arg0) {
    return osRecvMesg(&gPakAccessQueue, 0, arg0) == 0;
}
