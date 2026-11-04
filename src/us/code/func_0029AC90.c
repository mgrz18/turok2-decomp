#include "common.h"
#include "ultra_types.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         extern __OSViContext *D_800B7AC4; /* __osViNext */

/* libultra's osViSwapBuffer. */
void func_0029AC90(void *frameBufPtr) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    D_800B7AC4->framep = frameBufPtr;
    D_800B7AC4->state = (u16) (D_800B7AC4->state | VI_STATE_BUFFER_UPDATED);
    __osRestoreInt(temp_v0);
}
