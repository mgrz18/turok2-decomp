#include "common.h"
#include "globals/libultra.h"
#include "protos/ultra.h"


void __osPiCreateAccessQueue(void) {
    __osPiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osPiAccessQueue, __osPiAccessBuf, 1);
    osSendMesg(&__osPiAccessQueue, 0, 0);
}
