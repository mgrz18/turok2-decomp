#include "common.h"
#include "ultra_types.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */
extern __OSViContext *D_800B7AC4; /* __osViNext */

/* libultra's osViSetEvent. */
void osViSetEvent(OSMesgQueue *mq, OSMesg m, u32 retraceCount) {
    s32 temp_a0;

    temp_a0 = __osDisableInt();
    D_800B7AC4->msgq = mq;
    D_800B7AC4->msg = m;
    D_800B7AC4->retraceCount = retraceCount;
    __osRestoreInt(temp_a0);
}
