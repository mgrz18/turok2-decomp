#include "common.h"
#include "globals/libultra.h"
#include "protos/ultra.h"


void __osSiCreateAccessQueue(void) {
    __osSiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osSiAccessQueue, __osSiAccessBuf, 1);
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
