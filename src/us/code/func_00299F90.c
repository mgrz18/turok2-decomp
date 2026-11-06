#include "common.h"
#include "globals/libultra.h"

void osCreateMesgQueue(void *, void *, s32); /* extern */
void osSendMesg(void *, s32, s32); /* extern */

void __osSiCreateAccessQueue(void) {
    __osSiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osSiAccessQueue, __osSiAccessBuf, 1);
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
