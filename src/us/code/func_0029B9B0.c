/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_25.c, func_802BB5F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern OSThread_s *D_800B8918;
extern OSThread_s *D_800B8920;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 mask);
extern void __osDequeueThread(OSThread_s **queue, OSThread_s *t);
extern void func_0029C878(OSThread_s **queue, OSThread_s *t);
extern void func_0029C6DC(OSThread_s **queue);
void func_0029B9B0(OSThread_s *t, s32 pri)
{
    register u32 saveMask;
    OSThread_s **runQueue;
    saveMask = func_0029DFF0();
    if (t == 0) {
        t = D_800B8920;
    }
    if (t->priority != pri) {
        t->priority = pri;
        if (t != D_800B8920 && t->state != 1) {
            __osDequeueThread(t->queue, t);
            func_0029C878(t->queue, t);
        }
        runQueue = &D_800B8918;
        if (D_800B8920->priority < (*runQueue)->priority) {
            D_800B8920->state = 2;
            func_0029C6DC(runQueue);
        }
    }
    func_0029E010(saveMask);
}
