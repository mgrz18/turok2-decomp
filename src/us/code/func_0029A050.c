#include "common.h"
#include "globals/libultra.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */

void __osSiRelAccess(void) {
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
