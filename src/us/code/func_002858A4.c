#include "common.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 gPakAccessQueue;

void Controller_Unlock(void) {
    osSendMesg(&gPakAccessQueue, 0, 0);
}
