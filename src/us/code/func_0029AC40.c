#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

/* libultra's osViSetYScale. */
void osViSetYScale(f32 value) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    __osViNext->y.factor = value;
    __osViNext->state |= VI_STATE_YSCALE_UPDATED;
    __osRestoreInt(temp_v0);
}
