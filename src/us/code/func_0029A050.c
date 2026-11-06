#include "common.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 __osSiAccessQueue;

void __osSiRelAccess(void) {
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
