/* From decompals/ultralib (libultra 2.0I): src/os/thread.c, __osDequeueThread.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

#include "common.h"
#include "ultra_types.h"
extern void __osDequeueThread(OSThread **, OSThread *);
void __osDequeueThread(register OSThread** queue, register OSThread* t) {
    register OSThread* pred;
    register OSThread* succ;
    pred = (OSThread*)queue;
    succ = pred->next;
    while (succ != 0) {
        if (succ == t) {
            pred->next = t->next;
            return;
        }
        pred = succ;
        succ = pred->next;
    }
}
