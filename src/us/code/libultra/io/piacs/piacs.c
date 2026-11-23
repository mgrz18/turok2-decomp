#include "common.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

void __osPiCreateAccessQueue(void) {
    __osPiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osPiAccessQueue, __osPiAccessBuf, 1);
    osSendMesg(&__osPiAccessQueue, 0, 0);
}

#include "ultra_types.h"

void __osPiGetAccess(void);

static __inline__ void create_access_queue(void) {
    ((s32 *) &__osPiAccessQueueEnabled)[0] = 1;
    osCreateMesgQueue(&__osPiAccessQueue, __osPiAccessBuf, 1);
    osSendMesg(&__osPiAccessQueue, 0, 0);
}

void __osPiGetAccess(void) {
    OSMesg token;
    if (!((s32 *) &__osPiAccessQueueEnabled)[0]) {
        create_access_queue();
    }
    osRecvMesg(&__osPiAccessQueue, &token, 1);
}

void __osPiRelAccess(void) {
    osSendMesg(&__osPiAccessQueue, 0, 0);
}
