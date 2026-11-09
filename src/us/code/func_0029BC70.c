/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/UTIL/802C09A0.CPP, func_802BB8B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"
extern void __osSetTimerIntr(u64 tim);
extern u64 __osInsertTimer(OSTimer *t);
extern void __osSetCompare(u32 compare);
void __osTimerInterrupt(void)
{
    OSTimer *t;
    u32 count;
    u32 elapsed_cycles;
    u32 *counter;
    if (((OSTimer *)__osTimerList)->next == __osTimerList) {
        return;
    }
    counter = &__osTimerCounter;
    for (;;) {
        t = ((OSTimer *)__osTimerList)->next;
        if (t == __osTimerList) {
            __osSetCompare(0);
            __osTimerCounter = 0;
            break;
        }
        count = osGetCount();
        elapsed_cycles = count - *counter;
        *counter = count;
        if (elapsed_cycles < t->value) {
            t->value -= elapsed_cycles;
            __osSetTimerIntr(t->value);
            break;
        }
        t->prev->next = t->next;
        t->next->prev = t->prev;
        t->next = 0;
        t->prev = 0;
        if (t->mq != 0) {
            osSendMesg(t->mq, t->msg, 0);
        }
        if (t->interval != 0) {
            t->value = t->interval;
            __osInsertTimer(t);
        }
    }
}
void __osSetTimerIntr(u64 interval);
