#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);

/* libultra's osViSwapBuffer. */
void osViSwapBuffer(void *frameBufPtr) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    __osViNext->framep = frameBufPtr;
    __osViNext->state = (u16) (__osViNext->state | VI_STATE_BUFFER_UPDATED);
    __osRestoreInt(temp_v0);
}
