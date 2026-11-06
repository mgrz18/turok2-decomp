#include "common.h"
#include "ultra_types.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */
extern __OSViContext *__osViNext; /* __osViNext */

/* libultra's osViSetEvent. */
void osViSetEvent(OSMesgQueue *mq, OSMesg m, u32 retraceCount) {
    s32 temp_a0;

    temp_a0 = __osDisableInt();
    __osViNext->msgq = mq;
    __osViNext->msg = m;
    __osViNext->retraceCount = retraceCount;
    __osRestoreInt(temp_a0);
}
