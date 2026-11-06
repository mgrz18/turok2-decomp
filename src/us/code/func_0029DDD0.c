#include "common.h"
#include "globals/libultra.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */

void __osPiRelAccess(void) {
    osSendMesg(&__osPiAccessQueue, 0, 0);
}
