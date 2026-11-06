#include "common.h"
#include "globals/pak.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */

void Controller_Unlock(void) {
    osSendMesg(&gPakAccessQueue, 0, 0);
}
