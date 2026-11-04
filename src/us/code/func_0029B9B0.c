/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_25.c, func_802BB5F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern OSThread *D_800B8918;
extern OSThread *D_800B8920;
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
extern void __osDequeueThread(OSThread **queue, OSThread *t);
extern void __osEnqueueThread(OSThread **queue, OSThread *t);
extern void __osEnqueueAndYield(OSThread **queue);
void func_0029B9B0(OSThread *t, s32 pri)
{
    register u32 saveMask;
    OSThread **runQueue;
    saveMask = __osDisableInt();
    if (t == 0) {
        t = D_800B8920;
    }
    if (t->priority != pri) {
        t->priority = pri;
        if (t != D_800B8920 && t->state != 1) {
            __osDequeueThread(t->queue, t);
            __osEnqueueThread(t->queue, t);
        }
        runQueue = &D_800B8918;
        if (D_800B8920->priority < (*runQueue)->priority) {
            D_800B8920->state = 2;
            __osEnqueueAndYield(runQueue);
        }
    }
    __osRestoreInt(saveMask);
}
