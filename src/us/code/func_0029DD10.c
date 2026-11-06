#include "common.h"
#include "globals/libultra.h"

s32 osCreateMesgQueue(s32 *, s32 *, s32); /* extern */
s32 osSendMesg(s32 *, s32, s32); /* extern */

void __osPiCreateAccessQueue(void) {
    __osPiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osPiAccessQueue, __osPiAccessBuf, 1);
    osSendMesg(&__osPiAccessQueue, 0, 0);
}
