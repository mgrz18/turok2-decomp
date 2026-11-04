#include "common.h"
#include "ultra_types.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */
extern __OSViContext *D_800B7AC4; /* __osViNext */

/* libultra's osViSetYScale. */
void func_0029AC40(f32 value) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    D_800B7AC4->y.factor = value;
    D_800B7AC4->state |= VI_STATE_YSCALE_UPDATED;
    __osRestoreInt(temp_v0);
}
