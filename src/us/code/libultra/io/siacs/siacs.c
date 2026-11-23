#include "common.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

void __osSiCreateAccessQueue(void) {
    __osSiAccessQueueEnabled = 1;
    osCreateMesgQueue(&__osSiAccessQueue, __osSiAccessBuf, 1);
    osSendMesg(&__osSiAccessQueue, 0, 0);
}

#include "ultra_types.h"

void __osSiGetAccess(void);

static __inline__ void create_access_queue(void) {
    ((s32 *) &__osSiAccessQueueEnabled)[0] = 1;
    osCreateMesgQueue(&__osSiAccessQueue, __osSiAccessBuf, 1);
    osSendMesg(&__osSiAccessQueue, 0, 0);
}

void __osSiGetAccess(void) {
    OSMesg token;
    if (!((s32 *) &__osSiAccessQueueEnabled)[0]) {
        create_access_queue();
    }
    osRecvMesg(&__osSiAccessQueue, &token, 1);
}

void __osSiRelAccess(void) {
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
