#include "common.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 __osPiAccessQueue;

void __osPiRelAccess(void) {
    osSendMesg(&__osPiAccessQueue, 0, 0);
}
