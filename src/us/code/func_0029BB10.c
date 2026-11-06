/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_14.c, func_802BB750_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
extern void __osDispatchThread();
extern void __osEnqueueThread(ThreadNode **queue, ThreadNode *node);
extern ThreadNode *__osPopThread(ThreadNode **queue);
extern void __osEnqueueAndYield(ThreadNode **queue, ThreadNode *node);
void osStartThread(ThreadNode *arg0) {
    ThreadNode **sentinel;
    u32 token;
    token = __osDisableInt();
    if (arg0->state == 1) {
        goto state_one;
    }
    if (arg0->state != 8) {
        goto state_done;
    }
    arg0->state = 2;
    __osEnqueueThread((ThreadNode **)&__osRunQueue, arg0);
    goto state_done;
state_one:
    if (arg0->queue == 0) {
        goto insert_arg;
    }
    sentinel = (ThreadNode **)&__osRunQueue;
    if (arg0->queue == sentinel) {
insert_arg:
        arg0->state = 2;
        __osEnqueueThread((ThreadNode **)&__osRunQueue, arg0);
    } else {
        arg0->state = 8;
        __osEnqueueThread(arg0->queue, arg0);
        __osEnqueueThread(sentinel, __osPopThread(arg0->queue));
    }
state_done:
    if (__osRunningThread == 0) {
        __osDispatchThread();
    } else if (__osRunningThread->priority < (*(ThreadNode **)&__osRunQueue)->priority) {
        __osRunningThread->state = 2;
        __osEnqueueAndYield((ThreadNode **)&__osRunQueue, __osRunningThread);
    }
    __osRestoreInt(token);
}
